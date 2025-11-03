//
// Created by 33478 on 2025/11/3.
//

#ifndef CANDYJAR_MIDIPARSER_H
#define CANDYJAR_MIDIPARSER_H

#include "../JuceLibraryCode/JuceHeader.h"

class MidiParser
{
public:
    MidiParser();
    ~MidiParser();

    // 定义进度回调函数类型
    using ProgressCallback = std::function<void(int progressPercentage, const juce::String& message)>;
    
    bool loadMidiFile(const juce::File& midiFile, ProgressCallback progressCallback = nullptr);
    const juce::MidiFile& getMidiFile() const { return midiFile; }
    const juce::String& getLastErrorMessage() const { return lastErrorMessage; }

private:
    juce::MidiFile midiFile;
    juce::String lastErrorMessage;
};

#endif //CANDYJAR_MIDIPARSER_H