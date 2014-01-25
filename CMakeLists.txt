cmake_minimum_required(VERSION 2.8)

project(ConAir)

add_subdirectory(dependencies)
add_subdirectory(example)

include_directories(
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)
file(GLOB in_hdrs include/conair/*.h)

add_library(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
    ${in_hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}

    conair_dependencies
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

install(
    FILES ${hdrs}
    DESTINATION include
)

install(
    FILES ${in_hdrs}
    DESTINATION include/conair
)
