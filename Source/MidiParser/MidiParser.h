//
// Created by 33478 on 2025/11/3.
//

#ifndef CANDYJAR_MIDIPARSER_H
#define CANDYJAR_MIDIPARSER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <atomic>
#include <thread>
#include <future>

// MIDI文件统计信息结构
struct MidiStatistics
{
    int totalTracks = 0;
    int totalEvents = 0;
    int totalNotes = 0;
    double totalDuration = 0.0; // 以秒为单位
    int fileType = 0;
    int timeFormat = 0;
};

class MidiParser
{
public:
    MidiParser();
    ~MidiParser();

    // 定义进度回调函数类型
    using ProgressCallback = std::function<void(int progressPercentage, const juce::String& message)>;
    
    // 异步加载MIDI文件
    std::future<bool> loadMidiFileAsync(const juce::File& midiFile, ProgressCallback progressCallback = nullptr);
    
    // 同步加载MIDI文件（改进版）
    bool loadMidiFile(const juce::File& midiFile, ProgressCallback progressCallback = nullptr);
    
    // 获取MIDI文件
    const juce::MidiFile& getMidiFile() const { return midiFile; }
    
    // 获取统计信息
    const MidiStatistics& getStatistics() const { return statistics; }
    
    // 获取最后错误信息
    const juce::String& getLastErrorMessage() const { return lastErrorMessage; }
    
    // 取消加载操作
    void cancelLoading() { shouldCancel = true; }

private:
    juce::MidiFile midiFile;
    MidiStatistics statistics;
    juce::String lastErrorMessage;
    std::atomic<bool> shouldCancel {false};
    
    // 内部方法
    bool parseMidiData(juce::InputStream& stream, ProgressCallback progressCallback);
    void calculateStatistics(ProgressCallback progressCallback);
    void resetParser();
};

#endif //CANDYJAR_MIDIPARSER_H