# CandyJar

一个使用 JUCE 框架开发的 MIDI 软音源/波表。

## 简介

这是一个基于 JUCE 框架的简单应用程序，演示了基本的 GUI 组件使用方法。

## 构建说明

### 先决条件

- JUCE 8.0.10
- CMake 3.22 或更高版本
- 支持 C++17 的编译器
- Git

### 克隆仓库

```bash
git clone https://github.com/BuickMeow/CandyJar.git
cd CandyJar
git submodule update --init --recursive
```

### 构建项目

#### Windows (Visual Studio)

```bash
cmake -B cmake-build-debug -S .
cmake --build cmake-build-debug --config Debug
```

#### macOS (Xcode)

```bash
cmake -G Xcode -B Builds
cmake --build Builds --config Debug
```

#### Linux

```bash
cmake -B cmake-build-debug -S .
cmake --build cmake-build-debug
```

## 项目结构

- `Source/` - 源代码文件
- `JUCE/` - JUCE 框架作为子模块
- `CMakeLists.txt` - 主 CMake 构建脚本

## 许可证

本项目基于 JUCE 框架开发，JUCE 是一个商业/开源框架。请确保遵守 JUCE 许可协议。

## 以下是节能酱（人类）写的：

如果要运行项目，同文件夹下应该放入完整的`JUCE`文件夹。所需版本`8.0.10`。
呜呜呜呜好麻烦但是好幸福。我终于又重新开始写这玩意了。