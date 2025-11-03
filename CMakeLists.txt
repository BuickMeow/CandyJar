cmake_minimum_required(VERSION 3.22)

# 设置项目名称和版本
project(CandyJar VERSION 1.0.0)

# 添加JUCE子目录
add_subdirectory(JUCE)

# 创建GUI应用程序
juce_add_gui_app(CandyJar
        PRODUCT_NAME "CandyJar")

# 自动生成JuceHeader.h
juce_generate_juce_header(CandyJar)

# 添加源文件
target_sources(CandyJar
        PRIVATE
        Source/Main.cpp
        Source/MainComponent.cpp
        Source/MidiParser/MidiParser.cpp)

# 设置预处理器定义
target_compile_definitions(CandyJar
        PRIVATE
        # JUCE_WEB_BROWSER和JUCE_USE_CURL默认开启，但如果你不需要可以关闭
        JUCE_WEB_BROWSER=0  # 如果删除此行，请在上面的juce_add_gui_app调用中添加NEEDS_WEB_BROWSER TRUE
        JUCE_USE_CURL=0     # 如果删除此行，请在上面的juce_add_gui_app调用中添加NEEDS_CURL TRUE
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:CandyJar,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:CandyJar,JUCE_VERSION>")

# 链接所需的JUCE模块和推荐的编译标志
target_link_libraries(CandyJar
        PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_basics
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)