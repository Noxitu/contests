
if (DEFINED PROJECT_NAME)
    message(WARNING "PROJECT_NAME defined; include GGrzContestWorkspace before project call for proper defaults.")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_PROFILE "-O2 -DNDEBUG -pg" CACHE STRING "")
set(CMAKE_CXX_FLAGS_SANITIZED "-O0 -fsanitize=address -fsanitize=undefined" CACHE STRING "")

set(CURRENT_MODULE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Main" "Main")

function (ggrz_setup)
    file(GLOB files "*.cpp")
    foreach(file_path ${files})
        # Extract actual task name
        get_filename_component(task_name ${file_path} NAME_WE)
        string(REPLACE " " "_" task_name ${task_name})
        string(REPLACE "--" ";" task_name_components ${task_name})
        list(GET task_name_components 0 task_name_slug)
        get_filename_component(task_test_path "${CMAKE_CURRENT_SOURCE_DIR}/tests/${task_name_slug}" ABSOLUTE)

        # Generate resources
        set(task_resources "${task_name}-resources.cpp")
        add_custom_command(
            OUTPUT
                "${task_resources}.touch"
            BYPRODUCTS 
                "${task_resources}"
            COMMAND
                "${CMAKE_COMMAND}" 
                "-DFILE_PATH=${file_path}"
                "-DTEST_PATH=${task_test_path}"
                "-DOUTPUT_NAME=${task_resources}"
                "-P" "${CURRENT_MODULE_DIRECTORY}/GGrzContestWorkspace_GenerateResources.cmake"
            DEPENDS
                "${file_path}"
        )

        # Describe executable
        add_executable(${task_name} 
            ${file_path}
            "${task_resources}"
            "${task_resources}.touch"
        )

        target_compile_definitions(${task_name} 
            PRIVATE 
                CONTEST_WORKSPACE
        )

        set_target_properties(${task_name} 
            PROPERTIES 
                RUNTIME_OUTPUT_DIRECTORY "bin"
                CXX_STANDARD 14
        )

        target_link_libraries(${task_name} PRIVATE Main)
        set_source_files_properties(${file_path} COMPILE_FLAGS -Dmain=contestworkspace_test_main)

    endforeach()
endfunction()