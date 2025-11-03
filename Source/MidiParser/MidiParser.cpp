//
// Created by 33478 on 2025/11/3.
//

#include "MidiParser.h"
#include <chrono>
#include <thread>

MidiParser::MidiParser()
{
}

MidiParser::~MidiParser()
{
}

void MidiParser::resetParser()
{
    midiFile.clear();
    statistics = MidiStatistics();
    lastErrorMessage = "";
    shouldCancel = false;
}

std::future<bool> MidiParser::loadMidiFileAsync(const juce::File& file, ProgressCallback progressCallback)
{
    // 重置解析器状态
    resetParser();
    
    // 返回一个future对象，可以在其他线程中执行加载操作
    return std::async(std::launch::async, [this, file, progressCallback]() {
        return loadMidiFile(file, progressCallback);
    });
}

bool MidiParser::loadMidiFile(const juce::File& file, ProgressCallback progressCallback)
{
    // 重置解析器状态
    resetParser();
    
    // 检查是否需要取消
    if (shouldCancel.load())
    {
        if (progressCallback)
            progressCallback(0, "Loading cancelled");
        return false;
    }
    
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
        progressCallback(0, "Opening file: " + file.getFileName() + " (" + juce::String(fileSize) + " bytes)");
    }
    
    // 检查是否需要取消
    if (shouldCancel.load())
    {
        if (progressCallback)
            progressCallback(0, "Loading cancelled");
        return false;
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
        progressCallback(10, "File opened successfully");
    }
    
    // 检查是否需要取消
    if (shouldCancel.load())
    {
        if (progressCallback)
            progressCallback(10, "Loading cancelled");
        return false;
    }

    // 尝试读取MIDI文件
    if (progressCallback)
    {
        progressCallback(20, "Parsing MIDI file header");
    }
    
    // 添加一个小延迟，让用户能看到进度更新
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
    int fileType = 0;
    bool result = midiFile.readFrom(fileStream, true, &fileType);
    
    // 检查是否需要取消
    if (shouldCancel.load())
    {
        if (progressCallback)
            progressCallback(50, "Loading cancelled");
        return false;
    }
    
    if (progressCallback)
    {
        progressCallback(70, "Finishing parsing");
    }
    
    // 添加一个小延迟，让用户能看到进度更新
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    if (!result)
    {
        lastErrorMessage = "Failed to parse MIDI file: " + file.getFullPathName() + ". File type: " + juce::String(fileType);
        if (progressCallback)
            progressCallback(75, "Error: Failed to parse MIDI file");
        return false;
    }

    // 计算统计信息
    if (progressCallback)
    {
        progressCallback(80, "Calculating statistics");
    }
    
    calculateStatistics(progressCallback);
    
    if (progressCallback)
    {
        progressCallback(90, "Finalizing");
    }
    
    // 添加一个小延迟，让用户能看到进度更新
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if (progressCallback)
    {
        progressCallback(100, "MIDI file loaded successfully");
    }
        
    lastErrorMessage = "MIDI file loaded successfully. File type: " + juce::String(fileType) + 
                      ", Tracks: " + juce::String(statistics.totalTracks) + 
                      ", Events: " + juce::String(statistics.totalEvents) + 
                      ", Notes: " + juce::String(statistics.totalNotes);
    return true;
}

void MidiParser::calculateStatistics(ProgressCallback progressCallback)
{
    statistics.totalTracks = midiFile.getNumTracks();
    statistics.fileType = 1; // 默认值，实际应该从文件头获取
    statistics.timeFormat = midiFile.getTimeFormat();
    
    int totalEvents = 0;
    int totalNotes = 0;
    
    // 遍历所有轨道计算统计信息
    for (int trackIndex = 0; trackIndex < statistics.totalTracks; ++trackIndex)
    {
        // 检查是否需要取消
        if (shouldCancel.load())
        {
            if (progressCallback)
                progressCallback(80 + (10 * trackIndex / statistics.totalTracks), "Loading cancelled");
            return;
        }
        
        const juce::MidiMessageSequence* track = midiFile.getTrack(trackIndex);
        if (track != nullptr)
        {
            int numEvents = track->getNumEvents();
            totalEvents += numEvents;
            
            // 计算音符数量
            for (int eventIndex = 0; eventIndex < numEvents; ++eventIndex)
            {
                const auto* event = track->getEventPointer(eventIndex);
                if (event != nullptr && event->message.isNoteOn())
                {
                    totalNotes++;
                }
            }
        }
        
        // 更新进度
        if (progressCallback)
        {
            int progress = 80 + (10 * (trackIndex + 1) / statistics.totalTracks);
            progressCallback(progress, "Processing track " + juce::String(trackIndex + 1) + "/" + juce::String(statistics.totalTracks));
        }
    }
    
    statistics.totalEvents = totalEvents;
    statistics.totalNotes = totalNotes;
    
    // 计算总时长
    statistics.totalDuration = midiFile.getLastTimestamp();
    
    // 转换为秒（如果可能）
    midiFile.convertTimestampTicksToSeconds();
}