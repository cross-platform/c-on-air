add_definitions(
    -DUSE_WEBSOCKET
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    ${PROJECT_NAME}_dependencies

    cJSON.h
    cJSON.c

    mongoose.h
    mongoose.c
)

target_link_libraries(
    ${PROJECT_NAME}_dependencies

    pthread
    dl
)
