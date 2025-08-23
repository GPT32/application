# config
cmake_minimum_required(VERSION 3.30.5)
project(${APP_ID} LANGUAGES CXX)

set(APP_AUTHOR CACHE STRING "Application author")
set(APP_DESCRIPTION CACHE STRING "Application description")
set(APP_ID CACHE STRING "Application name")
set(APP_NAME CACHE STRING "Application name")
set(APP_REPO CACHE STRING "Application GitHub URL")
set(APP_VERSION CACHE STRING "Application version")
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WIN32_APP_VERSION "${APP_VERSION}.0")

# external dependencies
include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         v3.11.3
)
FetchContent_MakeAvailable(json)

# generate resource file
string(REPLACE "." "," WIN32_APP_VERSION "${WIN32_APP_VERSION}")
configure_file(
    "${PROJECT_SOURCE_DIR}/resources/resource.in.rc"
    "${PROJECT_SOURCE_DIR}/resources/resource.rc"
    @ONLY
)

# shared libraries
add_subdirectory(src/lib)

# main application executable
add_executable(${APP_ID}
    src/main.cpp
    src/controller.cpp
    src/model.cpp
    src/view.cpp
    src/views/api-key.cpp
    src/views/chat.cpp
    src/views/model-selector.cpp
    src/views/instructions.cpp
    src/views/about.cpp
    src/views/status-bar.cpp
    "${PROJECT_SOURCE_DIR}/resources/resource.rc"
    "${PROJECT_SOURCE_DIR}/resources/app.manifest"
)
set_target_properties(${APP_ID} PROPERTIES WIN32_EXECUTABLE ON)
target_link_libraries(${APP_ID} PRIVATE
    comctl32
    shell32
    app.lib
)
target_include_directories(${APP_ID} PRIVATE
    "${PROJECT_SOURCE_DIR}/resources"
    "${PROJECT_SOURCE_DIR}/src/views"
)
target_compile_options(${APP_ID} PRIVATE /W4)
target_compile_definitions(${APP_ID} PRIVATE
    APP_ID="${APP_ID}"
    APP_NAME="${APP_NAME}"
    APP_REPO="${APP_REPO}"
)

# unit testing
enable_testing()
add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
