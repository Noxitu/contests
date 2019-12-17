cmake_policy(VERSION 3.10)

set(TIME_LIMIT 0)
set(MEMORY_LIMIT 0)
set(VALID_KEYS "TIME_LIMIT;MEMORY_LIMIT")

set(REGEX "^ *#define +([A-Z_]+) +([0-9.]+) *$")
file(STRINGS ${FILE_PATH} PARSED_INPUT REGEX "${REGEX}")
foreach(line ${PARSED_INPUT})
    string(REGEX MATCH "${REGEX}" _ "${line}")

    if (CMAKE_MATCH_1 IN_LIST VALID_KEYS)
        set("${CMAKE_MATCH_1}" "${CMAKE_MATCH_2}")
    endif()
endforeach()

configure_file("${CMAKE_CURRENT_LIST_DIR}/GGrzContestWorkspace_resources.cpp.in" "${OUTPUT_NAME}")
file(TOUCH "${OUTPUT_NAME}.touch")

