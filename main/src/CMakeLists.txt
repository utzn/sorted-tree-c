# Do not change this file
include_directories(${SortedTree_SOURCE_DIR}/main/inc)
include_directories(${SortedTree_SOURCE_DIR}/tree/inc)
link_directories(${SortedTree_SOURCE_DIR}/tree/src)
add_executable(main main.c)
target_link_libraries(main tree)