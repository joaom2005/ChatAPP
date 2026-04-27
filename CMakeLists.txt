cmake_minimum_required(VERSION 4.1.1)

project(ChatAPP
    VERSION 0.0.1
    DESCRIPTION "An chat application written in CPP for learning purposes"
    LANGUAGES CXX
)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use -std=c++23, not -std=gnu++23

# Enable warning as error
set(CMAKE_COMPILE_WARNING_AS_ERROR on)

option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(ENABLE_SANITIZERS "Enable sanitizers" ON)
endif()

# Enable sanitizers
if(ENABLE_SANITIZERS AND NOT MSVC)
    add_library(sanitizers INTERFACE)

    target_compile_options(sanitizers INTERFACE
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )

    target_link_options(sanitizers INTERFACE
        -fsanitize=address
        -fsanitize=undefined
    )
endif()

# Add subdirectories
add_subdirectory(window)
add_subdirectory(app)