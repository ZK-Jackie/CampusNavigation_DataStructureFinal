# add iconv library
include_directories(./libiconv)
# find all C resources files
aux_source_directory(. DIR_LIB_SRCS)
# create sub-lib
add_library(utilsFunctions ${DIR_LIB_SRCS}
        printFileHead.c)