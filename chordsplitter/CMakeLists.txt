cmake_minimum_required(VERSION 3.15)

project(ChordSplitter VERSION 1.0.1)

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

juce_add_plugin(ChordSplitter
    PRODUCT_NAME "ChordSplitter-1_0_0"
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME "sainsay"                          # Specify the name of the plugin's author
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    PLUGIN_MANUFACTURER_CODE Sain               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Chsp                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    FORMATS VST3 AU Standalone                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    # PRODUCT_NAME "Audio Plugin Example"        # The name of the final executable, which can differ from the target name
    VST3_CATEGORIES Fx Delay Stereo
)

# juce_generate_juce_header(Infinight)

file (GLOB_RECURSE chordsplitter_private_list "${CMAKE_CURRENT_LIST_DIR}/src/*.h" "${CMAKE_CURRENT_LIST_DIR}/src/*hpp")
file (GLOB_RECURSE chordsplitter_source_list "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

target_sources(ChordSplitter
    PRIVATE
        ${chordsplitter_private_list}
        ${chordsplitter_source_list} )

target_compile_definitions(ChordSplitter
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        )

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(AudioPluginData SOURCES ...)

# If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.



target_include_directories(ChordSplitter PUBLIC ../common/include)

target_link_libraries(ChordSplitter
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        Common
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
