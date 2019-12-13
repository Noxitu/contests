cmake_minimum_required(VERSION 3.10)

set(CONTESTWORKSPACE_CODE_PATH "code" CACHE PATH "Path to directory with codes.")
set(CONTESTWORKSPACE_TEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/test" CACHE PATH "Path to directory with tests.")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_PROFILE "-O2 -DNDEBUG -pg" CACHE STRING "")
set(CMAKE_CXX_FLAGS_SANITIZED "-O0 -fsanitize=address" CACHE STRING "")

project(ContestWorkspace LANGUAGES CXX)

add_library(TestMain STATIC "util/test_main.cpp" 
                            "util/test_discovery.cpp"
                            "util/compare.cpp"
                            "util/limits.cpp")

set_property(TARGET TestMain PROPERTY CXX_STANDARD 14)
target_link_libraries(TestMain PRIVATE -lstdc++fs)

file(GLOB files "${CONTESTWORKSPACE_CODE_PATH}/*.cpp")
foreach(file_path ${files})
    get_filename_component(task_name ${file_path} NAME_WE)
    get_filename_component(task_test_path "${CONTESTWORKSPACE_TEST_PATH}/${task_name}" ABSOLUTE)

    add_executable(${task_name} ${file_path} "util/resources.cpp")
    target_compile_definitions(${task_name} PRIVATE CONTEST_WORKSPACE 
                                            TESTS_PATH=\"${task_test_path}\")
    target_link_libraries(${task_name} PRIVATE TestMain)

    set_source_files_properties(${file_path} COMPILE_FLAGS -Dmain=contestworkspace_test_main)

    set_property(TARGET ${task_name} PROPERTY CXX_STANDARD 14)
    set_target_properties(${task_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")
endforeach()