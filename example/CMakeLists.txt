project(ConAirExample)

add_definitions(-DHTML_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/html")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(
    ${PROJECT_NAME}

    CoaController.h
    CoaController.cpp

    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}

    ConAir
)
