add_library(graphics STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${PROJECT_SOURCE_DIR}/vendors/GLAD/src/glad.c
)

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/vendors/GLAD/include
)