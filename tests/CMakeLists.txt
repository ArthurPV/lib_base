add_executable(copy_test ./copy.c)
target_link_libraries(copy_test lib_base)
target_include_directories(copy_test PRIVATE ../src)

add_executable(result_test ./result.c)
target_link_libraries(result_test lib_base)
target_include_directories(result_test PRIVATE ../src)

add_executable(string_test ./string.c)
target_link_libraries(string_test lib_base)
target_include_directories(string_test PRIVATE ../src)

add_executable(vec_test ./vec.c)
target_link_libraries(vec_test lib_base)
target_include_directories(vec_test PRIVATE ../src)
