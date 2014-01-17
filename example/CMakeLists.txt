add_executable(
    ${PROJECT_NAME}Example

    CoaController.h
    CoaController.cpp

    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}Example

    ${PROJECT_NAME}
)

file(COPY html DESTINATION ./)
