#pragma once

#include <JuceHeader.h>
#include "MidiParser/MidiParser.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component,
                       public juce::Button::Listener,
                       private juce::Thread
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button* button) override;

private:
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<juce::TextButton> openMidiButton;
    std::unique_ptr<juce::TextEditor> outputText;
    std::unique_ptr<MidiParser> midiParser;
    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::ProgressBar> progressBar;
    double progressValue;
    bool isLoading;
    juce::File fileToLoad;
    
    // Thread methods
    void run() override;
    void loadMidiFile(const juce::File& file);
    void updateProgress(int percentage, const juce::String& message);
    void loadingFinished(bool success, const juce::String& message, double elapsedTime);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};