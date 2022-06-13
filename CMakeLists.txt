cmake_minimum_required(VERSION 3.19)

#-------------------------------------------------------------------------------
# Common settings
#-------------------------------------------------------------------------------
project(Vesty
    # VERSION must be required because it is used in moduleinfotool
    VERSION 0.0.0
)
set(target Vesty)
set(VST3SDK_DIR ./vst3sdk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-------------------------------------------------------------------------------
# vst3sdk settings
#-------------------------------------------------------------------------------
# SDK_ROOT must be absolute path
get_filename_component(SDK_ROOT "${VST3SDK_DIR}" ABSOLUTE)

# used for pluginterfaces and public.sdk modules which provides only a subset of them for VST-SDK
set(VST_SDK TRUE)

# for SMTG_AddVST3AuV3
set(SMTG_ADD_VSTGUI ON)
set(vst3sdk_SOURCE_DIR "${SDK_ROOT}")

list(APPEND CMAKE_MODULE_PATH "${SDK_ROOT}/cmake/modules")

include(SMTG_VST3_SDK)

smtg_setup_platform_toolset()
smtg_enable_vst3_sdk()
setupCoreAudioSupport()

if(SMTG_ADD_VSTGUI)
    set(SMTG_VSTGUI_ROOT "${SDK_ROOT}")
    smtg_enable_vstgui_support()
endif(SMTG_ADD_VSTGUI)

add_subdirectory("${SDK_ROOT}/pluginterfaces" pluginterfaces)
add_subdirectory("${SDK_ROOT}/base" base)
add_subdirectory("${SDK_ROOT}/public.sdk" public.sdk)
add_subdirectory("${SDK_ROOT}/public.sdk/source/vst/interappaudio" interappaudio)

# Add utilities
set(SDK_IDE_UTILITIES_FOLDER FOLDER "Utilities")
add_subdirectory("${SDK_ROOT}/public.sdk/samples/vst-utilities" vst-utilities)

#-------------------------------------------------------------------------------
# Target settings
#-------------------------------------------------------------------------------
file(GLOB_RECURSE cpp_files source/*.cpp)
file(GLOB_RECURSE h_files source/*.h)
file(GLOB_RECURSE hpp_files source/*.hpp)

list(APPEND header_files ${h_files} ${hpp_files})
list(APPEND plug_sources ${cpp_files} ${header_files})

smtg_add_vst3plugin(${target} ${plug_sources})

target_link_libraries(${target}
    PRIVATE
        sdk
)

#-------------------------------------------------------------------------------
# AUv3 settings
#-------------------------------------------------------------------------------
if(SMTG_MAC)
    include(SMTG_AddVST3AuV3)

    add_subdirectory("${SDK_ROOT}/public.sdk/source/vst/auv3wrapper" auv3wrapper)

    smtg_target_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)

    set(vesty_mac_app_sources
        "macOS/Sources/ViewController.m"
        "macOS/Sources/ViewController.h"
        "macOS/Sources/AppDelegate.m"
        "macOS/Sources/AppDelegate.h"
        "audiounitconfig.h"
    )

    set(vesty_mac_app_ui_resources
        "macOS/Resources/Base.lproj/Main.storyboard"
        "macOS/Resources/vesty.icns"
        "Shared/drumLoop.wav"
    )

    smtg_add_auv3_app("${target}_auv3_app" "macOS" "vesty AUV3 macOS" "com.steinberg.sdk.auv3.vestymac" audiounitconfig.h "macOS/vesty.entitlements" "${vesty_mac_app_sources}" "${vesty_mac_app_ui_resources}" "macOS/Resources/Info.plist" "Shared/Info.plist" ${target})
endif()

if(SMTG_WIN)
    target_sources(${target}
        PRIVATE
            resource/plug.rc
    )
endif(SMTG_WIN)
