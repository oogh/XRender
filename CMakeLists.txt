cmake_minimum_required(VERSION 3.4.1)

project(XRender LANGUAGES C CXX)

add_definitions("-DUSE_HARDWARE_DECODER")
add_definitions("-DUSE_HARDWARE_ENCODER")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DIR_SOURCE ./corerender)
set(DIR_MIDDLE_MAC ${DIR_SOURCE}/Mac)
set(DIR_EXSAMPLE ./examples)

# 添加头文件目录
include_directories(${DIR_SOURCE})
include_directories(${DIR_MIDDLE_MAC})

# 添加源码目录
aux_source_directory(${DIR_SOURCE} SRC_CORE)
aux_source_directory(${DIR_MIDDLE_MAC} SRC_MIDDLE)

#### （Mac 平台） ###################################################################################
if (APPLE)
    add_definitions("-DMAC")
    set(LibraryName "XRender")
    set(DIR_DEMO_MAC ${DIR_EXSAMPLE}/Mac/XRender)

    # FFmpeg
    find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
    find_library(AVFORMAT_LIBRARY avformat)

    find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
    find_library(AVCODEC_LIBRARY avcodec)

    find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
    find_library(AVUTIL_LIBRARY avutil)

    find_path(AVFILTER_INCLUDE_DIR libavfilter/avfilter.h)
    find_library(AVFILTER_LIBRARY avfilter)

    find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
    find_library(SWSCALE_LIBRARY swscale)

    find_path(SWRESAMPLE_INCLUDE_DIR libswresample/swresample.h)
    find_library(SWRESAMPLE_LIBRARY swresample)

    # sox
    find_path(SOX_INCLUDE_DIR sox.h)
    find_library(SOX_LIBRARY sox)

    # soundtouch
    set(SOUNDTOUCH_INCLUDE_DIR 3rdparty/Mac/soundtouch/include)
    # 使用dylib运行时报错: https://www.jianshu.com/p/5bf7795db50d
    # find_library(SOUNDTOUCH_LIBRARY NAME SoundTouch PATHS 3rdparty/Mac/soundtouch/lib)
    add_library(soundtouch STATIC IMPORTED)
    set_target_properties(soundtouch
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Mac/soundtouch/lib/libSoundTouch.a)

    # libyuv
    set(YUV_INCLUDE_DIR 3rdparty/Mac/libyuv/include)
    add_library(yuv
            STATIC
            IMPORTED)

    set_target_properties(yuv
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Mac/libyuv/lib/libyuv.a)

    # glad
    add_subdirectory(3rdparty/Mac/glad)
    set(GLAD_INCLUDE_DIR 3rdparty/Mac/glad/include)

    # glfw
    find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h)
    find_library(GLFW_LIBRARY glfw)

    set(EXTRAL_INCLUDE
            ${AVFORMAT_INCLUDE_DIR}
            ${AVCODEC_INCLUDE_DIR}
            ${AVUTIL_INCLUDE_DIR}
            ${AVFILTER_INCLUDE_DIR}
            ${SWSCALE_INCLUDE_DIR}
            ${SWRESAMPLE_INCLUDE_DIR}
	        ${SOUNDTOUCH_INCLUDE_DIR}
	        ${SOX_INCLUDE_DIR}
            ${YUV_INCLUDE_DIR}
            ${GLAD_INCLUDE_DIR}
            ${GLFW_INCLUDE_DIR})
    include_directories(${EXTRAL_INCLUDE})

    set(EXTRAL_LIBRARY
            ${AVFORMAT_LIBRARY}
            ${AVCODEC_LIBRARY}
            ${AVUTIL_LIBRARY}
            ${AVFILTER_LIBRARY}
            ${SWSCALE_LIBRARY}
            ${SWRESAMPLE_LIBRARY}
	        ${SOX_LIBRARY}
            soundtouch
            yuv
            glad
            ${GLFW_LIBRARY})

    SET(SRC_MEDIA ${SRC_CORE} ${SRC_MIDDLE})

    # 生成动态库
    add_library(${LibraryName} SHARED ${SRC_MEDIA})
    target_link_libraries(${LibraryName}
            ${EXTRAL_LIBRARY}
            "-framework OpenAL"
            "-framework OpenGL"
            "-framework VideoToolbox")

    # 生成可执行文件
    add_executable(gl_render ${SRC_MEDIA} ${DIR_DEMO_MAC}/gl_render.cpp)
    target_link_libraries(gl_render ${LibraryName})

    add_executable(xplayer ${SRC_MEDIA} ${DIR_DEMO_MAC}/xplayer.cpp)
    target_link_libraries(xplayer ${LibraryName})

#### （Android 平台） ###############################################################################
elseif (ANDROID)
    add_definitions("-DANDROID")
    set(LibraryName "corerender")
    set(DIR_DEMO_ANDROID ${DIR_EXSAMPLE}/Android/XRender)

    aux_source_directory(${DIR_SOURCE}/Android SRC_CORE_ANDROID)

    # 第三方库 -- 头文件
    include_directories(${DIR_DEMO_ANDROID}/corerender/includes/ffmpeg/${ANDROID_ABI}/include)
    include_directories(${DIR_DEMO_ANDROID}/corerender/includes/libyuv/${ANDROID_ABI}/include)
    include_directories(${DIR_DEMO_ANDROID}/corerender/includes/sox/${ANDROID_ABI}/include)
    include_directories(${DIR_DEMO_ANDROID}/corerender/includes/openal/${ANDROID_ABI}/include)

    SET(SRC_MEDIA ${SRC_CORE} ${SRC_CORE_ANDROID})

    # 生成链接库
    add_library(${LibraryName}
            SHARED
            ${SRC_MEDIA})

    # 第三方库 -- 库文件
    add_library(ffmpeg
            SHARED
            IMPORTED)

    add_library(yuv
            STATIC
            IMPORTED)

    add_library(sox
            STATIC
            IMPORTED)

    add_library(openal
            SHARED
            IMPORTED)

    set_target_properties(ffmpeg
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libffmpeg.so)

    set_target_properties(yuv
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libyuv.a)

    set_target_properties(sox
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libsox.a)

    set_target_properties(openal
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libopenal.so)

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
            GLESv3)

    SET(EXTRAL_LIBRARY
            ${log-lib}
            ${open-gl}
            ${android-egl}
            ${android})

    # 添加链接库
    target_link_libraries(${LibraryName}
            ffmpeg
            yuv
            sox
            openal
            ${EXTRAL_LIBRARY})
else ()
    message(STATUS, "WARNNING: unknown platform!")
endif ()
