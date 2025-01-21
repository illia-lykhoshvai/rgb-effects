project(EffectsLibrary)

set(LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/colors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fire.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hue_scroll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/meteors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/explosions.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/garland.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects.c
)

# Specify the library target
add_library(EffectsLibrary STATIC ${LIB_SOURCES})

# Include the library's headers
target_include_directories(EffectsLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(EffectsLibrary
    PRIVATE -Wall -Os
)
