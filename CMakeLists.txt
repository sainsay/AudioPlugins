cmake_minimum_required(VERSION 3.15)

project(AUDIO_PLUGINS VERSION 0.0.1)

add_subdirectory(deps/juce)
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "sainsay")
set_directory_properties(PROPERTIES JUCE_COMPANY_WEBSITE "https://github.com/sainsay/AudioPlugins" )

add_subdirectory(common)

target_compile_features(Common PRIVATE cxx_std_20)

add_subdirectory(infinight)
target_compile_features(Infinight PRIVATE cxx_std_20)
 