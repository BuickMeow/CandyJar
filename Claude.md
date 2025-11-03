# CandyJar(糖果罐儿) - 开发指导文档

**版本**: 1.0  
**创建日期**: 2025-11-02  
**项目状态**: 架构设计与研究阶段完成，准备进入实施阶段  
**目标**: 全面取代OmniMIDI的新一代MIDI软音源  

---

## 📋 项目概述

### 核心目标
CandyJar是一个基于JUCE C++框架开发的下一代MIDI软音源，旨在全面超越OmniMIDI的性能和功能。目标是在保持2025年DAW级音质的同时，实现前所未有的系统集成度和扩展性。

### 核心特性
- **JUCE C++基础架构**：跨平台、现代化的音频应用框架
- **16台虚拟MIDI输出设备**：支持1/4/16台设备配置，实现256条逻辑通道
- **无限复音数渲染**：实时1000、离线10000/无限复音数
- **现代化DSP效果**：500+ GM2/GS/XG效果的2025年升级版本
- **多音源格式支持**：SF2、SFZ、VSTi2/3音源完整支持
- **双API集成**：WinMM + KDMAPI连接方式
- **专业级音质**：2025年数字音频工作站标准
- **糖果色UI主题**：现代化、专业化的用户界面

### 性能目标
- **实时延迟**：ASIO路径 ≤10-15ms，KS路径 ≤15-20ms
- **复音能力**：实时1000+复音，离线无限复音
- **抖动稳定性**：≤1-2ms (p95-p50)
- **兼容性**：GM2、Roland GS、YAMAHA XG全标准支持

---

## 🏗️ 系统架构概览

### 9大核心模块架构

#### 1. 音频引擎模块 (AudioEngine)
**位置**: `src/modules/audio_engine/`
**职责**: 
- JUCE音频处理器集成
- ASIO/KS音频驱动管理
- 音频缓冲管理与优化
- 多线程音频处理调度

**关键技术**:
- JUCE AudioProcessorGraph
- ASIO驱动优化
- 零拷贝内存管理
- GPU加速支持

#### 2. 声音源管理模块 (SoundSourceManager)
**位置**: `src/modules/sound_source/`
**职责**:
- SF2/SFZ音源加载与缓存
- VSTi2/3插件宿主集成
- 多音色库管理
- 音色预加载与流式加载

**关键技术**:
- BASSMIDI内核优化
- VST3 SDK集成
- 内存映射文件
- 异步加载队列

#### 3. DSP效果处理模块 (DSPProcessor)
**位置**: `src/modules/dsp_effects/`
**职责**:
- 500+种GM2/GS/XG效果实现
- 实时参数处理与优化
- 效果链管理与调度
- VST效果器宿主功能

**关键技术**:
- 卷积混响算法
- 可微DSP(DDSP)
- 虚拟模拟技术
- AI增强处理

#### 4. MIDI解析模块 (MIDIParser)
**位置**: `src/modules/midi_parser/`
**职责**:
- MIDI消息解析与分发
- GM2/GS/XG标准实现
- RPN/NRPN参数处理
- SysEx事件处理

**关键技术**:
- 高性能消息队列
- 状态机优化
- 批量消息处理
- 错误恢复机制

#### 5. 虚拟设备管理模块 (VirtualDeviceManager)
**位置**: `src/modules/virtual_devices/`
**职责**:
- 1/4/16台虚拟设备支持
- 256条逻辑通道管理
- 设备路由与隔离
- WinMM/KDMAPI集成

**关键技术**:
- 多设备并发管理
- 通道分配算法
- 系统驱动集成
- 负载均衡

#### 6. 复音管理模块 (PolyphonyManager)
**位置**: `src/modules/polyphony/`
**职责**:
- 智能复音分配算法
- 实时复音限制
- 离线渲染优化
- AI驱动的预测算法

**关键技术**:
- 3层分配策略
- NUMA感知内存分配
- 无锁数据结构
- 神经网络预测

#### 7. 渲染引擎模块 (RenderingEngine)
**位置**: `src/modules/rendering/`
**职责**:
- 实时播放引擎
- 离线渲染引擎
- 音频文件导出
- 批处理功能

**关键技术**:
- 多线程并行渲染
- 内存优化策略
- 分布式计算
- 高精度时序控制

#### 8. 系统集成模块 (SystemIntegration)
**位置**: `src/modules/system/`
**职责**:
- WinMM API封装
- KDMAPI集成
- 系统注册与管理
- 驱动兼容性

**关键技术**:
- 包装器模式
- 动态注入
- 端点管理
- 兼容性适配

#### 9. UI界面模块 (UInterface)
**位置**: `src/modules/ui/`
**职责**:
- 糖果色主题界面
- 实时参数显示
- 效果链编辑器
- 设备管理界面

**关键技术**:
- JUCE GUI框架
- 自定义皮肤引擎
- 实时图表显示
- 响应式设计

---

## 📁 项目结构

```
CandyJar/
├── src/                          # 源代码
│   ├── main.cpp                  # 主程序入口
│   ├── CandyJarApplication.h/.cpp # JUCE应用类
│   └── modules/                  # 9大核心模块
│       ├── audio_engine/         # 音频引擎
│       ├── sound_source/         # 声音源管理
│       ├── dsp_effects/          # DSP效果处理
│       ├── midi_parser/          # MIDI解析
│       ├── virtual_devices/      # 虚拟设备管理
│       ├── polyphony/            # 复音管理
│       ├── rendering/            # 渲染引擎
│       ├── system/               # 系统集成
│       └── ui/                   # UI界面
├── assets/                       # 资源文件
│   ├── fonts/                    # 字体
│   ├── icons/                    # 图标
│   └── themes/                   # 主题
├── resources/                    # 配置资源
│   ├── presets/                  # 预设配置
│   ├── instrument_maps/          # 乐器映射
│   └── effect_profiles/          # 效果配置文件
├── tests/                        # 测试代码
│   ├── unit_tests/               # 单元测试
│   ├── integration_tests/        # 集成测试
│   └── performance_tests/        # 性能测试
├── build/                        # 构建目录
├── docs/                         # 项目文档
│   ├── architecture/             # 架构文档
│   ├── api/                      # API文档
│   ├── tutorials/                # 教程
│   └── specifications/           # 技术规范
├── CMakeLists.txt               # CMake构建配置
└── LICENSE                      # 许可证
```

---

## 🎯 开发计划与优先级

### 开发阶段 (32周/8个月)

#### 第一阶段 (0-8周) - MVP核心功能
**优先级**: 🔥 最高
1. **JUCE基础框架搭建**
   - 创建项目结构和基础配置
   - 实现基本的音频处理管道
   - ASIO/KS驱动集成

2. **MIDI解析基础**
   - 基本MIDI消息处理
   - GM2标准基础支持
   - 文件格式支持

3. **声音源管理**
   - SF2音源加载器
   - 基础合成引擎
   - 内存管理优化

**交付目标**: 能够播放基本MIDI文件的可运行版本

#### 第二阶段 (9-16周) - 虚拟设备与效果
**优先级**: 🔥 高
1. **虚拟设备系统**
   - 4台虚拟设备实现
   - WinMM包装器
   - 基础KDMAPI支持

2. **DSP效果框架**
   - 核心效果器实现
   - 效果链管理
   - 参数自动化

3. **性能优化**
   - 复音管理系统
   - 内存优化
   - 实时性能调优

**交付目标**: 完整的虚拟设备功能和基础DSP效果系统

#### 第三阶段 (17-24周) - 高级功能
**优先级**: ⚡ 中等
1. **16台设备扩展**
   - 256通道管理
   - KDMAPI完整集成
   - Windows MIDI Services适配

2. **高级DSP效果**
   - 500+ GM2/GS/XG效果
   - 现代化算法升级
   - VST宿主功能

3. **渲染引擎**
   - 离线渲染功能
   - 无限复音模式
   - 批处理能力

**交付目标**: 企业级完整功能实现

#### 第四阶段 (25-32周) - 优化与发布
**优先级**: ⚡ 中等
1. **UI界面开发**
   - 糖果色主题实现
   - 实时可视化
   - 用户体验优化

2. **跨平台适配**
   - macOS/Linux支持
   - 平台特定优化
   - 打包与分发

3. **最终优化**
   - 性能基准测试
   - 兼容性验证
   - 文档完善

**交付目标**: 完整的产品级发布版本

---

## 🔧 关键技术实现要点

### 1. 复音管理系统
```cpp
// 3层智能分配策略
class PolyphonyManager {
public:
    // 第一层：AI预测分配
    int predictOptimalVoices(const MIDIEvent& event);
    
    // 第二层：动态负载均衡
    void balanceLoad(int deviceId, int channel);
    
    // 第三层：硬件加速
    void offloadToGPU(AudioBlock& block);
};
```

**实现要点**:
- 使用NUMA感知内存分配
- 实现无锁环形缓冲队列
- 集成神经网络预测算法
- 支持GPU加速计算

### 2. 虚拟设备管理
```cpp
// 设备管理器核心接口
class VirtualDeviceManager {
public:
    // 配置1/4/16台设备
    void configureDeviceCount(int count);
    
    // 256通道路由管理
    void routeChannel(int deviceId, int channel, const VoiceSettings& settings);
    
    // KDMAPI集成
    void initializeKDMAPI();
};
```

**实现要点**:
- 实现设备热插拔支持
- 优化通道切换延迟
- 支持动态设备重配置
- 建立设备间隔离机制

### 3. DSP效果链
```cpp
// 效果链处理器
class EffectChainProcessor {
public:
    // 添加效果器
    void addEffect(int slot, Effect* effect);
    
    // 参数自动化
    void setAutomation(int parameterId, float value, int64 timestamp);
    
    // 并行处理优化
    void processParallel(AudioBlock& block);
};
```

**实现要点**:
- 实现无锁参数更新
- 支持效果器热插拔
- 优化DSP流水线处理
- 实现智能参数缓存

### 4. 内存优化策略
```cpp
// 零拷贝音频块
class ZeroCopyAudioBlock {
public:
    // 内存池管理
    static MemoryPool& getInstance();
    
    // 预分配缓冲区
    void preallocate(size_t samples);
    
    // 智能回收
    void recycle();
};
```

**实现要点**:
- 使用内存池减少分配开销
- 实现对象池模式
- 支持内存映射大文件
- 优化缓存局部性

---

## 📊 性能基准与测试

### 关键性能指标
- **CPU使用率**: <15% (1000复音实时播放)
- **内存占用**: <500MB (1000复音运行状态)
- **启动时间**: <3秒 (完整加载)
- **MIDI延迟**: <1ms (本地处理)
- **文件加载**: SF2文件 <2秒 (中等大小)

### 测试套件
```cpp
// 性能测试框架
class PerformanceTestSuite {
public:
    // 复音压力测试
    void stressTestPolyphony(int voiceCount);
    
    // 内存泄漏测试
    void memoryLeakTest();
    
    // 长时间稳定性测试
    void stabilityTest(Duration duration);
    
    // 兼容性测试
    void compatibilityTest();
};
```

### 基准对比
- **vs OmniMIDI**: 复音数 +50%, 延迟 -20%
- **vs FluidSynth**: 音质提升 +30%, 性能 +40%
- **vs Coolsoft**: 设备数量 4x, 通道数 4x

---

## 🛠️ 构建与开发环境

### 系统要求
- **操作系统**: Windows 10/11 (主要), macOS 10.15+, Ubuntu 20.04+
- **编译器**: MSVC 2022, Xcode 13+, GCC 11+
- **JUCE版本**: 7.0+
- **CMake版本**: 3.20+

### 依赖库
- **JUCE Framework**: 核心音频框架
- **VST3 SDK**: 插件宿主支持
- **BASSMIDI**: MIDI合成引擎
- **FLAC**: 音频文件支持
- **libsndfile**: 音频I/O处理
- **Rust/Cargo**: 高性能组件(可选)

### 构建命令
```bash
# 调试版本
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

# 发布版本
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### IDE配置
- **Visual Studio 2022**: 推荐Windows开发
- **Xcode**: macOS开发环境
- **CLion**: 跨平台C++ IDE
- **VS Code**: 轻量级开发编辑器

---

## 📚 重要文档路径

### 架构文档
- 系统架构: `docs/architecture_design/candyjar_system_architecture.md`
- DSP效果设计: `docs/dsp_design/dsp_effects_implementation.md`
- 复音优化: `docs/performance_optimization/polyphony_performance_optimization.md`
- UI设计: `docs/ui_ux_design/candyjar_ui_ux_design.md`

### 研究资料
- OmniMIDI分析: `docs/omni_midi_analysis/`
- FluidSynth分析: `docs/fluidsynth_analysis/`
- Sound Canvas分析: `docs/sound_canvas_analysis/`
- XSynth分析: `docs/xsynth_analysis/`

### 技术规范
- MIDI标准: `docs/midi_standards_specs.md`
- 虚拟设备技术: `docs/coolsoft_analysis/virtual_devices_tech.md`
- JUCE引擎: `docs/juce_engine/juce_audio_engine.md`

---

## ⚠️ 重要开发注意事项

### 1. 线程安全
- **实时线程严格限制**: 禁止在AudioProcessor线程中进行I/O操作
- **参数传递**: 使用无锁队列或原子操作
- **内存分配**: 实时线程中禁止动态内存分配
- **异常处理**: 实现完整的错误恢复机制

### 2. 性能优化
- **预分配**: 关键缓冲区预先分配和重用
- **缓存优化**: 实现多级缓存策略
- **SIMD指令**: 利用向量化指令提升计算性能
- **GPU加速**: 考虑将部分DSP计算迁移到GPU

### 3. 兼容性考虑
- **API版本**: 处理不同Windows版本的MIDI API差异
- **驱动兼容**: 测试主流音频驱动的兼容性
- **插件兼容**: 确保VST2/VST3插件的正常加载
- **文件格式**: 支持各种MIDI和音源文件格式

### 4. 内存管理
- **智能指针**: 使用unique_ptr和shared_ptr管理资源
- **RAII模式**: 确保资源的自动释放
- **内存池**: 频繁分配的对象使用对象池
- **垃圾回收**: 定期检查和释放未使用的资源

### 5. 错误处理
- **分层错误处理**: 不同层次的错误处理策略
- **日志系统**: 实现完整的调试和错误日志
- **优雅降级**: 在资源不足时提供优雅的降级方案
- **用户反馈**: 为用户提供清晰的错误信息

---

## 🎨 UI/UX设计原则

### 糖果色主题设计
- **主色调**: 温暖的粉色、薄荷绿、天空蓝
- **辅助色**: 柠檬黄、珊瑚橙、薰衣草紫
- **设计风格**: 现代扁平化，圆角设计
- **动画效果**: 平滑的过渡动画和微交互

### 界面布局
- **模块化面板**: 可停靠的独立功能面板
- **实时监控**: 实时显示CPU、复音数、延迟等指标
- **拖拽操作**: 效果链编辑支持拖拽操作
- **快捷键**: 完整的键盘快捷键支持

### 响应式设计
- **多分辨率**: 支持从1366x768到4K的各种分辨率
- **DPI适配**: 适配不同DPI设置的显示器
- **自适应布局**: 界面元素根据窗口大小自动调整
- **主题切换**: 支持明暗主题切换

---

## 🔄 版本迭代计划

### v1.0 (MVP) - 基础功能
- 基础MIDI播放
- SF2音源支持
- 基础DSP效果
- 1台虚拟设备

### v1.5 (Beta) - 增强功能
- 4台虚拟设备
- 完整DSP效果集
- VST插件支持
- 基础UI界面

### v2.0 (Release) - 完整功能
- 16台虚拟设备
- 256通道支持
- KDMAPI集成
- 完整UI/UX

### v2.5 (Pro) - 专业功能
- 无限复音渲染
- 跨平台支持
- 高级AI功能
- 企业级优化

### v3.0 (Ultimate) - 下一代功能
- AI驱动的声音合成
- 云端音色库集成
- 协作功能
- VR/AR支持

---

## 📞 开发团队指导

### 开发者角色分工
- **音频引擎工程师**: 负责核心音频处理模块
- **MIDI系统工程师**: 负责MIDI解析和设备管理
- **DSP工程师**: 负责效果器和音频算法实现
- **UI/UX设计师**: 负责界面设计和用户体验
- **系统集成工程师**: 负责平台适配和驱动集成
- **测试工程师**: 负责功能测试和性能优化

### 代码规范
- **命名约定**: 使用驼峰命名法，类名首字母大写
- **注释标准**: 每个公共接口都需要详细的文档注释
- **代码审查**: 所有代码变更都需要同行审查
- **单元测试**: 新功能必须包含相应的单元测试

### 协作流程
1. **需求分析**: 详细分析和确认功能需求
2. **技术设计**: 制定技术实现方案和架构设计
3. **开发实现**: 按照设计文档进行代码开发
4. **测试验证**: 完整的功能和性能测试
5. **代码审查**: 同行代码审查和质量检查
6. **集成发布**: 合并到主分支并发布新版本

---

## 📝 总结

CandyJar项目代表了MIDI软音源开发的技术前沿，通过模块化架构设计、现代DSP技术、AI增强算法和用户友好的界面设计，致力于成为全面超越现有解决方案的新一代MIDI软音源。

本文档为开发团队提供了完整的技术指导，包括架构设计、实现要点、开发计划和注意事项。请开发团队严格按照文档要求进行开发，确保项目的技术质量和用户体验。

**项目成功指标**:
- ✅ 技术架构设计完成
- ✅ 核心功能模块规划完成  
- ✅ 性能目标明确设定
- ✅ 开发计划制定完成
- ✅ 质量标准建立完成

**下一步行动**:
1. 建立开发环境和工具链
2. 开始第一阶段的MVP开发
3. 建立持续集成和测试流程
4. 启动UI/UX设计工作
5. 建立用户反馈和改进机制

---

*本文档将随着项目进展持续更新，请定期检查最新版本。*

**最后更新**: 2025-11-02  
**维护者**: CandyJar开发团队  
**文档版本**: 1.0