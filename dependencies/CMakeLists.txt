project(ConAirDependencies)

add_definitions(
    -DUSE_WEBSOCKET
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    ${PROJECT_NAME}

    cJSON.h
    cJSON.c

    mongoose.h
    mongoose.c
)

if(UNIX)
    target_link_libraries(
        ${PROJECT_NAME}

        pthread
        dl
    )
endif(UNIX)
