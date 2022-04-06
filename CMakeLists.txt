cmake_minimum_required(VERSION 3.15)

project(AUDIO_PLUGINS)

add_subdirectory(deps/juce)
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "sainsay")
set_directory_properties(PROPERTIES JUCE_COMPANY_WEBSITE "https://github.com/sainsay/AudioPlugins" )
set_directory_properties(PROPERTIES JUCE_COPY_PLUGIN_AFTER_BUILD TRUE )
set_directory_properties(PROPERTIES JUCE_VST3_COPY_DIR ${CMAKE_BINARY_DIR}/artefacts/VST3/ )
set_directory_properties(PROPERTIES JUCE_AU_COPY_DIR ${CMAKE_BINARY_DIR}/artefacts/AU/ )

add_subdirectory(common)

target_compile_features(Common PRIVATE cxx_std_20)

add_subdirectory(infinight)
target_compile_features(Infinight PRIVATE cxx_std_20)

add_subdirectory(chordsplitter)
target_compile_features(ChordSplitter PRIVATE cxx_std_20)
