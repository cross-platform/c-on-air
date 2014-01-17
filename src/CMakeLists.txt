add_library(
    ${PROJECT_NAME}

    CoaJson.h
    CoaJson.cpp
    CoaServer.h
    CoaServer.cpp
)

target_link_libraries(
    ${PROJECT_NAME}

    ${PROJECT_NAME}Dependencies
)
