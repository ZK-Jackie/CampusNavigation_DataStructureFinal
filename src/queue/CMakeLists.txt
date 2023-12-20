# find all C resources files
aux_source_directory(. DIR_LIB_SRCS)
# create sub-lib
add_library(queueFunctions ${DIR_LIB_SRCS})