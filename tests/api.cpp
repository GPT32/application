#include "lib/api.hpp"

#include <cassert>
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
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    // bail early if no api key is found
    const char* apiKey = std::getenv("API_KEY");

    if (!apiKey) {
        return 1;
    }

    // create prompt
    auto input = "Hello from GPT32, a desktop application built with the Win32 API.";
    auto r1 = lib::api::createResponse(apiKey, input, "gpt-4.1", "", "");
    log(r1);

    // list models
    auto r2 = lib::api::listModels(apiKey);
    log(r2);

    for (auto& [key, val] : r2.body["data"].items()) {
        if (val["owned_by"] != "openai") {
            continue;
        }

        std::cout << val["id"] << std::endl;
    }

    return 1;
}
