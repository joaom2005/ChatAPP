add_library(graphics STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/font.cpp
)

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/vendors/glm
        ${PROJECT_SOURCE_DIR}/vendors/stb
)

target_link_libraries(graphics
    PUBLIC
        glad
)