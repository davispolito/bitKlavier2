# 3.24.1 is bundled in Visual Studio 2022 v17.4
# 3.24.1 is also bundled in CLion as of 2023
cmake_minimum_required(VERSION 3.24.1)
# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")


# This tells cmake we have goodies in the /cmake folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (PamplejuceVersion)
add_compile_options(-w)

# Change me!
# This is the internal name of the project and the name of JUCE's shared code "target"
# Note: This cannot have spaces (it may be 2023, but you can't have it all!)
# Worry not, JUCE's PRODUCT_NAME can have spaces (and is DAWs display)
set(PROJECT_NAME "bitKlavier2")

# Worry not, JUCE's PRODUCT_NAME can have spaces (and is what DAWs will display)
# You can also just have it be the same thing as PROJECT_NAME
# You may want to append the major version on the end of this (and PROJECT_NAME) ala:
#   set(PROJECT_NAME "MyPlugin_v${MAJOR_VERSION}")
# Doing so enables major versions to show up in IDEs and DAWs as separate plugins
# allowing you to change parameters and behavior without breaking existing user projects
set(PRODUCT_NAME "bitKlavier2 Demo")

# Change me! Used for the MacOS bundle name and Installers
set(COMPANY_NAME "bitKlavier2 Company")

# Change me! Used for the MacOS bundle identifier (and signing)
set(BUNDLE_ID "com.bitKlavier2.bitKlavier2")

# Change me! Set the plugin formats you want built
# Valid choices: AAX Unity VST VST3 AU AUv3 Standalone
set(FORMATS Standalone AU VST3 AUv3)

# For simplicity, the name of the CMake project is also the name of the target
project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

# Configures universal binaries in CI
include(PamplejuceMacOS)

# Couple tweaks that IMO should be JUCE defaults
include(JUCEDefaults)

# JUCE is setup as a submodule in the /JUCE folder
# Locally, you must run `git submodule update --init --recursive` once
# and later `git submodule update --remote --merge` to keep it up to date
# On Github Actions, this is done as a part of actions/checkout
add_subdirectory(JUCE)

# Add any other modules you want modules here, before the juce_add_plugin call

add_subdirectory(third_party/chowdsp_utils)
#add_subdirectory(third_party/readerwriterqueue)
#add_subdirectory(third_party/tracktion_engine)
# This adds the melatonin inspector module
add_subdirectory (modules/melatonin_inspector)
# If you only include this third party in PRIVATE source files, you do not
# need to install it when your main project gets installed.
# set(JSON_Install OFF CACHE INTERNAL "")


# See `docs/CMake API.md` in the JUCE repo for all config options
juce_add_plugin("${PROJECT_NAME}"
    # Icons for the standalone app
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"

    # Change me!
    COMPANY_NAME "${COMPANY_NAME}"
    BUNDLE_ID "${BUNDLE_ID}"

    # On MacOS, plugin is copied to ~/Users/yourname/Library/Audio/Plug-Ins/
    COPY_PLUGIN_AFTER_BUILD TRUE

    # Change me!
    # A four-character plugin id
    # First character MUST be uppercase for AU formats
    PLUGIN_MANUFACTURER_CODE bitK

    # Change me!
    # A unique four-character plugin id
    # Note: this must have at least one upper-case character
    PLUGIN_CODE B303
    FORMATS "${FORMATS}"

    # The name of your final executable
    # This is how it's listed in the DAW
    # This can be different from PROJECT_NAME and can have spaces!
    # You might want to use v${MAJOR_VERSION} here once you go to v2...
    PRODUCT_NAME "${PRODUCT_NAME}")

# This lets us use our code in both the JUCE targets and our Test target
# Without running into ODR violations
add_library(SharedCode INTERFACE
        source/interface/open_gl_line_renderer.h
        source/interface/open_gl_line_renderer.cpp
        source/interface/FullInterface.cpp
        source/interface/FullInterface.h
        source/synthesis/framework/common.h
        source/synthesis/framework/note_handler.h
        source/synthesis/lookups/lookup_table.h
        source/standalone/synth_editor.h
        source/standalone/synth_editor.h
        source/standalone/main.cpp
        source/interface/header_section.h
        source/interface/header_section.cpp
        source/interface/popup_browser.cpp
        source/interface/look_and_feel/text_look_and_feel.cpp
        source/interface/Preparations/DirectPreparation.cpp
        source/interface/Preparations/DirectPreparation.h
        source/interface/BKitems/BKItem.cpp
        source/interface/BKitems/BKItem.h
        source/interface/synth_slider.h
        source/interface/Cable/CableView.cpp
        source/interface/Cable/CableView.h
)

# C++20, please
# Use cxx_std_23 for C++23 (as of CMake v 3.20)
target_compile_features(SharedCode INTERFACE cxx_std_20)

# Manually list all .h and .cpp files for the plugin
# If you are like me, you'll use globs for your sanity.
# Just ensure you employ CONFIGURE_DEPENDS so the build system picks up changes
# If you want to appease the CMake gods and avoid globs, manually add files like so:
# set(SourceFiles Source/PluginEditor.h Source/PluginProcessor.h Source/PluginEditor.cpp Source/PluginProcessor.cpp)
file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
target_sources(SharedCode INTERFACE ${SourceFiles})

# Adds a BinaryData target for embedding assets into the binary
include(Assets)

# MacOS only: Cleans up folder and target organization on Xcode.
include(XcodePrettify)
include_directories(source/common)
include_directories(source/interface/look_and_feel)
include_directories(source/interface)
include_directories(source/interface/Preparations)
include_directories(source/interface/BKitems)
include_directories(source/synthesis)
include_directories(source/synthesis/framework/Synthesiser)
include_directories(source/synthesis/framework)
include_directories(source/synthesis/lookups)
include_directories(source/synthesis/synth_engine)
include_directories(third_party/tracktion_engine/modules/tracktion_engine)
# This is where you can set preprocessor definitions for JUCE and your plugin
set(JUCE_DEPENDENCIES
        chowdsp::chowdsp_plugin_base
        chowdsp::chowdsp_plugin_state
        chowdsp::chowdsp_sources
        chowdsp::chowdsp_plugin_utils
        chowdsp::chowdsp_gui
        chowdsp::chowdsp_dsp_utils
        chowdsp::chowdsp_data_structures
        juce::juce_audio_utils
        juce::juce_dsp
       #juce::juce_open_gl
)
target_compile_definitions(SharedCode
    INTERFACE
        #JUCE_USE_CUSTOM_PLUGIN_STANDALONE_ENTRYPOINT=1

        JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1
    # JUCE_WEB_BROWSER and JUCE_USE_CURL off by default
    JUCE_WEB_BROWSER=0  # If you set this to 1, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you set this to 1, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0

        JUCE_OPENGL3=1
    # Uncomment if you are paying for a an Indie/Pro license or releasing under GPLv3
    # JUCE_DISPLAY_SPLASH_SCREEN=0
        #JUCE_ENABLE_REPAINT_DEBUGGING=1
    # lets the app known if we're Debug or Release
    CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    VERSION="${CURRENT_VERSION}"
        JUCE_MODULE_AVAILABLE_chowdsp_plugin_state=1
    # JucePlugin_Name is for some reason doesn't use the nicer PRODUCT_NAME
    PRODUCT_NAME_WITHOUT_VERSION="bitKlavier2"
)



# Link to any other modules you added (with juce_add_module) here!
juce_add_module(third_party/tracktion_engine/modules/tracktion_engine)
juce_add_module(modules/melatonin_audio_sparklines)

#juce_add_module(third_party/chowdsp_utils/modules/gui/chowdsp_gui)
#juce_add_modules(chowdsp_utils)
#juce_add_module(third_party/chowdsp_utils/modules/plugin/chowdsp_plugin_state)
#juce_add_module(third_party/chowdsp_utils/modules/dsp/chowdsp_sources)
#juce_add_module(third_party/chowdsp_utils/modules/plugin/chowdsp_plugin_utils)
# Usually JUCE modules must have PRIVATE visibility
# See https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md#juce_add_module
# However, with bitKlavier2, you'll link modules to SharedCode with INTERFACE visibility
# This allows the JUCE plugin targets and the Tests target to link against it
target_link_libraries(SharedCode
    INTERFACE
    Assets

        ${JUCE_DEPENDENCIES}
    melatonin_inspector
    juce_audio_utils
    juce_audio_processors
    juce_dsp
    juce_gui_basics
    juce_gui_extra
        juce_opengl
        #chowdsp_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags


        #tracktion::engine
        )

#message(WARNING "The JUCE HEADERS WILL BE GENERATED FOR ${PROJECT_NAME} at ${CMAKE_BINARY_DIR}")
#juce_generate_juce_header(${PROJECT_NAME})
#include_directories(${CMAKE_BINARY_DIR}/${PROJECT_NAME}_artefacts/JuceLibraryCode)
include (FetchContent)

FetchContent_Declare (melatonin_perfetto
        GIT_REPOSITORY https://github.com/sudara/melatonin_perfetto.git
        GIT_TAG origin/main)

FetchContent_MakeAvailable (melatonin_perfetto)
# Link the JUCE plugin targets our SharedCode target
target_link_libraries("${PROJECT_NAME}" PRIVATE
        SharedCode

        Melatonin::Perfetto)

# IPP support, comment out to disable
include(PamplejuceIPP)

# Everything related to the tests target
#include(Tests)

# A separate target keeps the Tests target fast!
#include(Benchmarks)

# Pass some config to GA (like our PRODUCT_NAME)
#finclude(GitHubENV)
#juce_generate_juce_header("${PROJECT_NAME}")