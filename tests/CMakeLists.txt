# storage tests
add_executable(storage storage.cpp)
target_link_libraries(storage PRIVATE app.lib)
add_test(storage storage)

# markdown tests
add_executable(markdown markdown.cpp)
target_link_libraries(markdown PRIVATE app.lib)
add_test(markdown markdown)

# registry tests
add_executable(registry registry.cpp)
target_link_libraries(registry PRIVATE app.lib)
add_test(registry registry)

# requests tests
add_executable(requests requests.cpp)
target_link_libraries(requests PRIVATE app.lib)
add_test(requests requests)

# api tests
add_executable(api api.cpp)
target_link_libraries(api PRIVATE app.lib)
add_test(api api)

# uuid tests
add_executable(uuid uuid.cpp)
target_link_libraries(uuid PRIVATE app.lib)
add_test(uuid uuid)
