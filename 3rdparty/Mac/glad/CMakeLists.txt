include_directories(include)
aux_source_directory(src DIR_LIB_SRCS)
add_library(glad SHARED ${DIR_LIB_SRCS})