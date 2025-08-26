#include "lib/api.hpp"

#include <cassert>
#include <chrono>
#include <iostream>

/**
 * @brief Prints the response structure to console.
 * @param response The response structure.
 */
void log(lib::requests::Response response) {
    std::cout << "Status: " << response.code << std::endl;
    std::cout << "Headers:\n";
    for (const auto& [key, value] : response.headers) {
        std::wcout << "  " << key << ": " << value << std::endl;
    }
    std::cout << "Body:\n" << response.body.dump(2) << std::endl;
    std::cout << "Error:\n" << response.error.value_or("none") << std::endl;
}

/**
 * @brief Tests cost endpoint.
 * @return int Exit code (0 if assertion passes)
 */
int testCost() {
    // bail early if no admin api key is found
    const char* adminApiKey = std::getenv("ADMIN_API_KEY");

    if (!adminApiKey) {
        return 0;
    }

    auto now = std::chrono::system_clock::now();
    auto lastWeek = now - std::chrono::weeks(1);
    auto lastWeekTimestamp = std::chrono::duration_cast<std::chrono::seconds>(lastWeek.time_since_epoch()).count();
    auto r = lib::api::cost(adminApiKey, lastWeekTimestamp);
    log(r);
    return 0;
}

/**
 * @brief Create prompt test.
 * @return int Exit code (0 if assertion passes)
 */
int testCreatePrompt() {
    // bail early if no api key is found
    const char* apiKey = std::getenv("API_KEY");

    if (!apiKey) {
        return 0;
    }

    auto input = "Hello from GPT32, a desktop application built with the Win32 API.";
    auto r = lib::api::createResponse(apiKey, input, "gpt-4.1", "", "");
    log(r);
    return 0;
}

/**
 * @brief Tests list models.
 * @return int Exit code (0 if assertion passes)
 */
int testListModels() {
    // bail early if no api key is found
    const char* apiKey = std::getenv("API_KEY");

    if (!apiKey) {
        return 0;
    }

    auto r = lib::api::listModels(apiKey);
    log(r);

    for (auto& [key, val] : r.body["data"].items()) {
        if (val["owned_by"] != "openai") {
            continue;
        }

        std::cout << val["id"] << std::endl;
    }

    return 0;
}

/**
 * @brief Tests usage endpoint.
 * @return int Exit code (0 if assertion passes)
 */
int testUsage() {
    // bail early if no admin api key is found
    const char* adminApiKey = std::getenv("ADMIN_API_KEY");

    if (!adminApiKey) {
        return 0;
    }

    auto now = std::chrono::system_clock::now();
    auto lastWeek = now - std::chrono::weeks(1);
    auto lastWeekTimestamp = std::chrono::duration_cast<std::chrono::seconds>(lastWeek.time_since_epoch()).count();
    auto r = lib::api::usage(adminApiKey, lastWeekTimestamp);
    log(r);
    return 0;
}

/**
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    assert(testCreatePrompt() == 0);
    assert(testListModels() == 0);
    assert(testCost() == 0);
    assert(testUsage() == 0);
    return 1;
}
