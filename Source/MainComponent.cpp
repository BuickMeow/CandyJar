#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : 
    progressValue(0.0), 
    isLoading(false)
{
    // 创建打开MIDI文件按钮
    openMidiButton = std::make_unique<juce::TextButton>("Open MIDI File");
    openMidiButton->addListener(this);
    addAndMakeVisible(openMidiButton.get());
    
    // 创建取消加载按钮
    cancelLoadButton = std::make_unique<juce::TextButton>("Cancel Load");
    cancelLoadButton->addListener(this);
    cancelLoadButton->setVisible(false);
    addAndMakeVisible(cancelLoadButton.get());
    
    // 创建进度条
    progressBar = std::make_unique<juce::ProgressBar>(progressValue);
    progressBar->setVisible(false);
    addAndMakeVisible(progressBar.get());
    
    // 创建文本输出区域
    outputText = std::make_unique<juce::TextEditor>();
    outputText->setMultiLine(true);
    outputText->setReadOnly(true);
    outputText->setFont(juce::FontOptions(14.0f));
    addAndMakeVisible(outputText.get());
    
    // 创建MIDI解析器
    midiParser = std::make_unique<MidiParser>();
    
    // 显示初始状态消息
    outputText->clear();
    outputText->setText("Enhanced MIDI file reader is ready.\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Click 'Open MIDI File' button to load a MIDI file.\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Features: Progress tracking, statistics, multi-threading.\n");
    
    setSize (600, 400);
    
    // 启动定时器检查加载状态
    startTimer(100); // 每100毫秒检查一次
}

MainComponent::~MainComponent()
{
    stopTimer();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    // 移除原来的Hello World文本，因为我们现在有按钮和文本区域了
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    // 设置按钮和文本区域的位置
    auto area = getLocalBounds();
    auto buttonArea = area.removeFromTop(40);
    auto openButtonArea = buttonArea.removeFromLeft(buttonArea.getWidth() / 2).withSizeKeepingCentre(150, 30);
    auto cancelButtonArea = buttonArea.withSizeKeepingCentre(150, 30);
    
    openMidiButton->setBounds(openButtonArea);
    cancelLoadButton->setBounds(cancelButtonArea);
    
    // 设置进度条位置
    auto progressArea = area.removeFromTop(30).reduced(10, 5);
    progressBar->setBounds(progressArea);
    
    // 设置文本区域占据剩余空间
    outputText->setBounds(area.reduced(10));
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == openMidiButton.get())
    {
        if (isLoading)
        {
            outputText->moveCaretToEnd();
            outputText->insertTextAtCaret("Already loading a file. Please wait or cancel.\n");
            return;
        }
        
        // 创建文件选择器
        fileChooser = std::make_unique<juce::FileChooser>("Select a MIDI file to load...",
                                                         juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                                                         "*.mid;*.midi");
        
        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::File midiFile (chooser.getResult());
            if (midiFile.existsAsFile())
            {
                loadMidiFile(midiFile);
            }
            else if (!midiFile.existsAsFile() && midiFile.getFullPathName() != "")
            {
                outputText->moveCaretToEnd();
                outputText->insertTextAtCaret("Selected file does not exist: " + midiFile.getFullPathName() + "\n");
            }
            else
            {
                outputText->moveCaretToEnd();
                outputText->insertTextAtCaret("No file selected.\n");
            }
        });
    }
    else if (button == cancelLoadButton.get())
    {
        if (isLoading)
        {
            midiParser->cancelLoading();
            outputText->moveCaretToEnd();
            outputText->insertTextAtCaret("Cancelling load operation...\n");
        }
    }
}

void MainComponent::loadMidiFile(const juce::File& file)
{
    isLoading = true;
    loadingCompleted = false;
    progressValue = 0.0;
    progressBar->setVisible(true);
    cancelLoadButton->setVisible(true);
    fileToLoad = file;
    
    outputText->clear();
    outputText->setText("Loading MIDI file: " + file.getFileName() + "\n");
    outputText->moveCaretToEnd();
    outputText->repaint();
    
    // 记录开始时间
    storedStartTime = juce::Time::getMillisecondCounterHiRes();
    
    // 启动异步加载
    loadFuture = midiParser->loadMidiFileAsync(file, [this](int progress, const juce::String& message) {
        juce::MessageManager::callAsync([this, progress, message]() {
            updateProgress(progress, message);
        });
    });
    
    // 启动线程等待异步操作完成
    std::thread([this]() {
        // 等待异步操作完成
        bool result = loadFuture.get();
        
        // 记录结束时间
        auto endTime = juce::Time::getMillisecondCounterHiRes();
        auto timeElapsed = endTime - storedStartTime;
        
        // 在主线程中完成加载
        juce::MessageManager::callAsync([this, result, timeElapsed]() {
            loadingCompleted = true;
            loadingResult = result;
            elapsedTime = timeElapsed;
        });
    }).detach();
}

void MainComponent::updateProgress(int percentage, const juce::String& message)
{
    // 更新进度条
    progressValue = percentage / 100.0;
    
    // 更新文本显示
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("[" + juce::String(percentage) + "%] " + message + "\n");
    
    // 强制刷新显示
    outputText->repaint();
    progressBar->repaint();
}

void MainComponent::checkLoadingStatus()
{
    if (loadingCompleted.load() && isLoading)
    {
        isLoading = false;
        cancelLoadButton->setVisible(false);
        
        if (loadingResult.load())
        {
            loadingFinished(true, "MIDI file loaded successfully", elapsedTime.load());
            displayStatistics();
        }
        else
        {
            loadingFinished(false, midiParser->getLastErrorMessage(), elapsedTime.load());
        }
        
        loadingCompleted = false;
    }
}

void MainComponent::loadingFinished(bool success, const juce::String& message, double timeElapsed)
{
    if (success)
    {
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("\nSuccessfully loaded MIDI file!\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Load time: " + juce::String(timeElapsed, 2) + " ms\n");
    }
    else
    {
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("\nFailed to load MIDI file!\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Error: " + message + "\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Load time: " + juce::String(timeElapsed, 2) + " ms\n");
    }
    
    // 保持进度条可见，显示最终结果
    progressValue = success ? 1.0 : 0.0; // 成功显示100%，失败显示0%
    
    // 强制刷新显示
    outputText->repaint();
    progressBar->repaint();
}

void MainComponent::displayStatistics()
{
    const MidiStatistics& stats = midiParser->getStatistics();
    
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("\n--- MIDI File Statistics ---\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Total Tracks: " + juce::String(stats.totalTracks) + "\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Total Events: " + juce::String(stats.totalEvents) + "\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Total Notes: " + juce::String(stats.totalNotes) + "\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Duration: " + juce::String(stats.totalDuration, 2) + " ticks\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("File Type: " + juce::String(stats.fileType) + "\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Time Format: " + juce::String(stats.timeFormat) + "\n");
}

void MainComponent::timerCallback()
{
    checkLoadingStatus();
}