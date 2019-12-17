project(Main LANGUAGES CXX)

find_package(Threads REQUIRED)

add_library(Main STATIC
    commands.cpp
    compare.cpp
    limits.cpp
    main.cpp
    test_discovery.cpp
    test_execution.cpp
)

set_property(TARGET Main PROPERTY CXX_STANDARD 14)
target_link_libraries(Main PRIVATE -lstdc++fs Threads::Threads)