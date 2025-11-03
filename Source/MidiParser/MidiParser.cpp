//
// Created by 33478 on 2025/11/3.
//

#include "MidiParser.h"

MidiParser::MidiParser()
{
}

MidiParser::~MidiParser()
{
}

bool MidiParser::loadMidiFile(const juce::File& file, ProgressCallback progressCallback)
{
    // 检查文件是否存在
    if (!file.existsAsFile())
    {
        lastErrorMessage = "File does not exist: " + file.getFullPathName();
        if (progressCallback)
            progressCallback(0, "Error: File does not exist");
        return false;
    }

    // 检查文件大小
    auto fileSize = file.getSize();
    lastErrorMessage = "File size: " + juce::String(fileSize) + " bytes";
    
    if (progressCallback)
    {
        progressCallback(0, "Opening file: " + file.getFileName());
    }

    // 创建文件输入流
    juce::FileInputStream fileStream(file);
    if (!fileStream.openedOk())
    {
        lastErrorMessage = "Cannot open file: " + file.getFullPathName();
        if (progressCallback)
            progressCallback(5, "Error: Cannot open file");
        return false;
    }
    
    if (progressCallback)
    {
        progressCallback(10, "File opened successfully, size: " + juce::String(fileSize) + " bytes");
    }

    // 尝试读取MIDI文件
    if (progressCallback)
    {
        progressCallback(20, "Parsing MIDI file header");
    }
        
    int fileType = 0;
    bool result = midiFile.readFrom(fileStream, true, &fileType);
    
    if (progressCallback)
    {
        progressCallback(90, "Finishing parsing");
    }
    
    if (!result)
    {
        lastErrorMessage = "Failed to parse MIDI file: " + file.getFullPathName() + ". File type: " + juce::String(fileType);
        if (progressCallback)
            progressCallback(95, "Error: Failed to parse MIDI file");
        return false;
    }

    if (progressCallback)
    {
        progressCallback(100, "MIDI file loaded successfully");
    }
        
    lastErrorMessage = "MIDI file loaded successfully. File type: " + juce::String(fileType) + ", Tracks: " + juce::String(midiFile.getNumTracks());
    return true;
}