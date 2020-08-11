cmake_minimum_required(VERSION 3.4.1)

project(XRender CXX)

add_definitions("-DUSE_HARDWARE_DECODER")
add_definitions("-DUSE_HARDWARE_ENCODER")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DIR_SOURCE ./corerender)
set(DIR_EXSAMPLE ./examples)

# 添加头文件目录
include_directories(${DIR_SOURCE})

# 添加源码目录
aux_source_directory(${DIR_SOURCE} SRC_CORE)

#### （Mac 平台） ###################################################################################
if (APPLE)
    add_definitions("-DMAC")
    set(ExecuteName "XRender")
    set(DIR_DEMO_MAC ${DIR_EXSAMPLE}/Mac/XRender)

    aux_source_directory(${DIR_DEMO_MAC} SRC_EXSAMPLE_MAC)

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

    set(EXTRAL_INCLUDE
            ${AVFORMAT_INCLUDE_DIR}
            ${AVCODEC_INCLUDE_DIR}
            ${AVUTIL_INCLUDE_DIR}
            ${AVFILTER_INCLUDE_DIR}
            ${SWSCALE_INCLUDE_DIR}
            ${SWRESAMPLE_INCLUDE_DIR})
    include_directories(${EXTRAL_INCLUDE})

    set(EXTRAL_LIBRARY
            ${AVFORMAT_LIBRARY}
            ${AVCODEC_LIBRARY}
            ${AVUTIL_LIBRARY}
            ${AVFILTER_LIBRARY}
            ${SWSCALE_LIBRARY}
            ${SWRESAMPLE_LIBRARY})

    SET(SRC_MEDIA ${SRC_CORE} ${SRC_EXSAMPLE_MAC})

    # 生成可执行文件
    add_executable(${ExecuteName} ${SRC_MEDIA})

    # 添加链接库
    target_link_libraries(${ExecuteName} ${EXTRAL_LIBRARY} "-framework OpenAL")

#### （Android 平台） ###############################################################################
elseif (ANDROID)
    add_definitions("-DANDROID")
    set(LibraryName "corerender")
    set(DIR_DEMO_ANDROID ${DIR_EXSAMPLE}/Android/XRender)

    aux_source_directory(${DIR_SOURCE}/Android SRC_CORE_ANDROID)

    # 第三方依赖库
    include_directories(${DIR_DEMO_ANDROID}/corerender/includes/ffmpeg/${ANDROID_ABI}/include)
    include_directories(${DIR_DEMO_ANDROID}/corerender/includes/libyuv/${ANDROID_ABI}/include)

    SET(SRC_MEDIA ${SRC_CORE} ${SRC_CORE_ANDROID})

    # 生成链接库
    add_library(${LibraryName}
            SHARED
            ${SRC_MEDIA})

    add_library(ffmpeg
            SHARED
            IMPORTED)

    add_library(yuv
            STATIC
            IMPORTED)

    set_target_properties(ffmpeg
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libffmpeg.so)

    set_target_properties(yuv
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_DEMO_ANDROID}/corerender/libs/${ANDROID_ABI}/libyuv.a)

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
            yuv
            ${EXTRAL_LIBRARY})
else ()
    message(STATUS, "WARNNING: unknown platform!")
endif ()
