#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : 
    Thread("MidiLoaderThread"),
    progressValue(0.0), 
    isLoading(false)
{
    // 创建打开MIDI文件按钮
    openMidiButton = std::make_unique<juce::TextButton>("Open MIDI File");
    openMidiButton->addListener(this);
    addAndMakeVisible(openMidiButton.get());
    
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
    outputText->setText("MIDI file reader is ready.\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("Click 'Open MIDI File' button to load a MIDI file.\n");
    outputText->moveCaretToEnd();
    outputText->insertTextAtCaret("MIDI reading functionality is operational.\n");
    
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    stopThread(1000);
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
    auto buttonArea = area.removeFromTop(40).withSizeKeepingCentre(150, 30);
    openMidiButton->setBounds(buttonArea);
    
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
            outputText->insertTextAtCaret("Already loading a file. Please wait.\n");
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
}

void MainComponent::loadMidiFile(const juce::File& file)
{
    isLoading = true;
    progressValue = 0.0;
    progressBar->setVisible(true);
    fileToLoad = file;
    
    outputText->clear();
    outputText->setText("Loading MIDI file: " + file.getFileName() + "\n");
    outputText->moveCaretToEnd();
    outputText->repaint();
    
    // 启动后台线程加载MIDI文件
    startThread();
}

void MainComponent::run()
{
    // 记录开始时间
    auto startTime = juce::Time::getMillisecondCounterHiRes();
    
    // 加载MIDI文件
    bool success = false;
    juce::String errorMessage = "Unknown error";
    
    try 
    {
        // 使用带进度回调的加载函数
        success = midiParser->loadMidiFile(fileToLoad, [this](int progress, const juce::String& message) {
            // 在主线程中更新进度
            juce::MessageManager::callAsync([this, progress, message]() {
                updateProgress(progress, message);
            });
        });
        errorMessage = midiParser->getLastErrorMessage();
    }
    catch (const std::exception& ex)
    {
        errorMessage = "Exception occurred: " + juce::String(ex.what());
    }
    catch (...)
    {
        errorMessage = "Unknown exception occurred";
    }
    
    // 记录结束时间
    auto endTime = juce::Time::getMillisecondCounterHiRes();
    auto elapsedTime = endTime - startTime;
    
    // 在主线程中完成加载
    juce::MessageManager::callAsync([this, success, errorMessage, elapsedTime]() {
        loadingFinished(success, errorMessage, elapsedTime);
    });
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
    
    // 强制处理消息队列
    juce::Timer::callPendingTimersSynchronously();
}

void MainComponent::loadingFinished(bool success, const juce::String& message, double elapsedTime)
{
    if (success)
    {
        const juce::MidiFile& loadedMidi = midiParser->getMidiFile();
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Successfully loaded MIDI file!\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Number of tracks: " + juce::String(loadedMidi.getNumTracks()) + "\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Time format: " + juce::String(loadedMidi.getTimeFormat()) + "\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Load time: " + juce::String(elapsedTime, 2) + " ms\n");
    }
    else
    {
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Failed to load MIDI file!\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Error: " + message + "\n");
        outputText->moveCaretToEnd();
        outputText->insertTextAtCaret("Load time: " + juce::String(elapsedTime, 2) + " ms\n");
    }
    
    // 保持进度条可见，显示最终结果
    progressValue = success ? 1.0 : 0.0; // 成功显示100%，失败显示0%
    isLoading = false;
    
    // 强制刷新显示
    outputText->repaint();
    progressBar->repaint();
}