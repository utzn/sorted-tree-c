# Do not change this file
add_executable(test_tree test_tree.c)
include_directories(${SortedTree_SOURCE_DIR}/tree/inc)
add_test(NAME test_tree COMMAND test_tree)
target_link_libraries(test_tree tree)


add_executable(test_sorted_tree_snow test_sorted_tree_snow.c)
include_directories(${SortedTree_SOURCE_DIR}/tree/inc)
add_test(NAME test_sorted_tree_snow COMMAND test_sorted_tree_snow)
target_link_libraries(test_sorted_tree_snow tree)

add_executable(test_balanced_tree_snow test_balanced_tree_snow.c)
include_directories(${SortedTree_SOURCE_DIR}/tree/inc)
add_test(NAME test_balanced_tree_snow COMMAND test_balanced_tree_snow)
target_link_libraries(test_balanced_tree_snow tree)