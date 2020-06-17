cmake_minimum_required(VERSION 3.4.1)

project(FFBuildTest CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LibraryName "corerender")
add_definitions("-DANDROID")

set(DIR_SOURCE ./corerender)
set(DIR_DEMO_ANDROID ./examples/Android/XRender)

# 添加头文件目录
include_directories(${DIR_SOURCE})

# 添加源码目录
aux_source_directory(${DIR_SOURCE} SRC_CORE)
aux_source_directory(${DIR_SOURCE}/Android SRC_CORE_ANDROID)

# 第三方依赖库
include_directories(${DIR_DEMO_ANDROID}/corerender/includes/${ANDROID_ABI}/include)

SET(SRC_MEDIA ${SRC_CORE} ${SRC_CORE_ANDROID})

# 生成链接库
add_library(${LibraryName}
            SHARED
            ${SRC_MEDIA})

add_library(ffmpeg
	    SHARED
	    IMPORTED)

set_target_properties(ffmpeg
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libffmpeg.so)

find_library(
        log-lib
        log)

find_library(
        android
        android)

find_library(
        android-egl
        EGL)

find_library(
        open-gl
        GLESv2)

SET(EXTRAL_LIBRARY
        ${log-lib}
        ${open-gl}
        ${android-egl}
        ${android})

# 添加链接库
target_link_libraries(${LibraryName}
	ffmpeg
        ${EXTRAL_LIBRARY})
