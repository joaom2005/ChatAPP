add_library(window STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendors/GLAD/src/glad.c
)

target_include_directories(window
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/vendors/GLAD/include
)

if(WIN32)
    target_sources(window PRIVATE Windows/windows_window.cpp Windows/windows_context.cpp)
    target_link_libraries(
        window
        PRIVATE
            dwmapi.lib
            opengl32.lib
    )
elseif(UNIX AND NOT APPLE)
    target_sources(window PRIVATE Linux/linux_window.cpp Linux/linux_context.cpp)
    target_link_libraries(
        window
        PRIVATE
            GL
            X11
    )
endif()