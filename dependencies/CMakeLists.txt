add_definitions(
    -DUSE_WEBSOCKET
)

add_library(
    ${PROJECT_NAME}Dependencies

    cJSON.h
    cJSON.c
    interpp.h
    interpp.cpp
    mongoose.h
    mongoose.c
)

target_link_libraries(
    ${PROJECT_NAME}Dependencies

    pthread
    dl
)
