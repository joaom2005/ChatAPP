add_library(ui STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIManager.hpp
    
)

target_include_directories(ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ui
    PUBLIC
        graphics
        window
)