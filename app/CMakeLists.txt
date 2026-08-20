add_executable(app main.cpp)

target_link_libraries(app
    PRIVATE
        window
        graphics
        ui
)

# Move the assets directory to build's directory
set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSETS_DIR}"
            "$<TARGET_FILE_DIR:app>/assets"
)