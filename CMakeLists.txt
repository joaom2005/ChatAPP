cmake_minimum_required(VERSION 3.25)

project(ChatAPP
    VERSION 0.0.1
    DESCRIPTION "An chat application written in CPP for learning purposes"
    LANGUAGES C CXX
)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use -std=c++23, not -std=gnu++23

# Enable warning as error
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

option(ENABLE_SANITIZERS "Enable sanitizers" ON)

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

if(TARGET sanitizers)
    link_libraries(sanitizers)
endif()

# Shared GLAD library compiled once for the whole project
add_library(glad STATIC
    ${PROJECT_SOURCE_DIR}/vendors/GLAD/src/glad.c
)

target_include_directories(glad
    PUBLIC
        ${PROJECT_SOURCE_DIR}/vendors/GLAD/include
        ${PROJECT_SOURCE_DIR}/vendors/glm
        ${PROJECT_SOURCE_DIR}/vendors/stb
)

# Add subdirectories
add_subdirectory(window)
add_subdirectory(graphics)
add_subdirectory(ui)
add_subdirectory(app)