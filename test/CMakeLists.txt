cmake_minimum_required(VERSION 3.4.1)

project(XRender LANGUAGES C CXX)

add_definitions("-DUSE_HARDWARE_DECODER")
add_definitions("-DUSE_HARDWARE_ENCODER")
add_definitions("-DMAC")

set(LibraryName "XRender")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DIR_SOURCE ../corerender)
set(DIR_MIDDLE_MAC ${DIR_SOURCE}/Mac)
set(DIR_TEST .)
set(DIR_THIRDPARTY ../3rdparty)

include_directories(${DIR_TEST})
aux_source_directory(${DIR_TEST} TEST_SRC)

add_subdirectory(../ MediaCore.OUT)

# 添加头文件目录
include_directories(${DIR_SOURCE})
include_directories(${DIR_MIDDLE_MAC})
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_path(AVFILTER_INCLUDE_DIR libavfilter/avfilter.h)
find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_path(SWRESAMPLE_INCLUDE_DIR libswresample/swresample.h)
find_path(SOX_INCLUDE_DIR sox.h)
set(SOUNDTOUCH_INCLUDE_DIR ${DIR_THIRDPARTY}/Mac/soundtouch/include)
set(YUV_INCLUDE_DIR ${DIR_THIRDPARTY}/Mac/libyuv/include)
set(GLAD_INCLUDE_DIR ${DIR_THIRDPARTY}/Mac/glad/include)
find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h)
set(GOOGLETEST_INCLUDE_DIR ${DIR_THIRDPARTY}/Mac/googletest/include)

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
        ${GLFW_INCLUDE_DIR}
        ${GOOGLETEST_INCLUDE_DIR})
include_directories(${EXTRAL_INCLUDE})


# gtest
add_library(gtest
        STATIC
        IMPORTED)

set_target_properties(gtest
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_THIRDPARTY}/Mac/googletest/lib/libgtest.a)

# gmock
add_library(gmock
        STATIC
        IMPORTED)

set_target_properties(gmock
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_THIRDPARTY}/Mac/googletest/lib/libgmock.a)

# 生成可执行文件
add_executable(unit_test ${TEST_SRC})
target_link_libraries(unit_test ${LibraryName} gtest gmock)
