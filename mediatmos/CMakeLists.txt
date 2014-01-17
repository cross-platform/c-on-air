add_executable(
    Mediatmos

    CoaController.h
    CoaController.cpp

    main.cpp
)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

target_link_libraries(
    Mediatmos

    ${PROJECT_NAME}
    ${CURL_LIBRARIES}
)

execute_process(
    COMMAND rm -R -f html
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

execute_process(
    COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/html ./
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
