add_library(app.lib STATIC
    api.cpp
    markdown.cpp
    requests.cpp
    settings.cpp
    storage.cpp
)

target_include_directories(app.lib PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/resources"
)

target_compile_definitions(app.lib PUBLIC
    APP_NAME="${APP_NAME}"
    APP_ID="${APP_ID}"
)

target_link_libraries(app.lib PUBLIC
    winhttp
    rpcrt4.lib
    nlohmann_json::nlohmann_json
)
