cmake_minimum_required(VERSION 3.22)

project(CloudMakingMachineVST3 VERSION 1.0.0)

if(NOT DEFINED JUCE_DIR)
    message(FATAL_ERROR "JUCE_DIR is not set")
endif()

if(NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "JUCE was not found at: ${JUCE_DIR}")
endif()

add_subdirectory("${JUCE_DIR}" JUCE)


juce_add_plugin(CloudMakingMachineVST3
    COMPANY_NAME "LowMana"
    BUNDLE_ID "com.lowmana.cloudmakingmachine"
    PLUGIN_MANUFACTURER_CODE Lm
    PLUGIN_CODE LmC1
    FORMATS VST3
    PRODUCT_NAME "Cloud Making Machine"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(CloudMakingMachineVST3
    PRIVATE
        PluginProcessor.cpp
        PluginEditor.cpp
        PluginEditor.h
)

juce_generate_juce_header(CloudMakingMachineVST3)

target_compile_features(CloudMakingMachineVST3 PRIVATE cxx_std_17)

target_compile_definitions(CloudMakingMachineVST3 PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(CloudMakingMachineVST3 PRIVATE
    
    juce::juce_audio_utils
    juce::juce_dsp
)
