#include "lib/requests.hpp"

#include <cassert>
#include <string>

/** @brief Testing url. */
constexpr wchar_t TEST_URL[] = L"https://httpbin.org";

/**
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    // send an unauthorized request
    auto r1 = lib::requests::get((std::wstring(TEST_URL) + L"/bearer").c_str());
    assert(r1.code == HTTP_STATUS_DENIED);

    // now send a authorized request
    lib::requests::Headers headers1;
    headers1[L"Authorization"] = L"Bearer YOUR_TOKEN_HERE";
    auto r2 = lib::requests::get((std::wstring(TEST_URL) + L"/bearer").c_str(), headers1);
    assert(r2.body.contains("authenticated"));
    assert(r2.body["authenticated"] == true);

    // send a post with data
    lib::requests::Headers headers2;
    headers2[L"Authorization"] = L"Bearer YOUR_TOKEN_HERE";
    nlohmann::json data1 = { { "name", "ChatGPT" }, { "message", "Hola, mundo" } };
    auto r3 = lib::requests::post((std::wstring(TEST_URL) + L"/anything").c_str(), headers2, data1);

    return 0;
}
