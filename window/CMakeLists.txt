add_library(window
    window.cpp
    window.hpp
)

# Makes include paths available to anyone who links this library
target_include_directories(window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})