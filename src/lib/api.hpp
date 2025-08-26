#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "requests.hpp"

/**
 * @namespace lib::api
 * @brief A lightweight SDK for OpenAI.
 */
namespace lib::api {
    /** @brief OpenAI REST API base url. */
    inline const std::wstring BASE_URL = L"https://api.openai.com/v1";

    /**
     * @brief Builds authentication header.
     * @param apiKey    The OpenAI API key.
     * @returns Headers Request headers.
     */
    lib::requests::Headers authenticate(const std::string& apiKey);

    /**
     * @brief Gets cost details.
     * @param adminApiKey   The OpenAI Admin API key.
     * @param startTime     Start time in unix seconds.
     * @return Response The response object.
     */
    lib::requests::Response cost(const std::string& adminApiKey, const time_t startTime);

    /**
     * @brief Creates a model response.
     * @param apiKey        The OpenAI API key.
     * @param input         The model input.
     * @param prevId        Previous response id.
     * @param model         The model to use.
     * @param instructions  Instructions, role or tone.
     * @return Response The response object.
     */
    lib::requests::Response createResponse(const std::string& apiKey,
        const std::string& input,
        const std::string& model,
        const std::string prevId,
        const std::string instructions);

    /**
     * @brief Lists the currently available models.
     * @param apiKey The OpenAI API key.
     * @return Response The response object.
     */
    lib::requests::Response listModels(const std::string& apiKey);

    /**
     * @brief Aggregates paginated endpoint data into a single vector.
     * @param adminApiKey   The OpenAI Admin API key.
     * @param baseUrl       The base URL.
     * @return Response The response object.
     */
    lib::requests::Response paginate(const std::string& adminApiKey, const std::wstring& baseUrl);

    /**
     * @brief Converts a string to wide string.
     * @param str The string to convert.
     * @return std::wstring The converted string.
     */
    std::wstring toWideString(const std::string& str);

    /**
     * @brief Get completions usage details.
     * @param adminApiKey   The OpenAI Admin API key.
     * @param startTime     Start time in unix seconds.
     * @return Response The response object.
     */
    lib::requests::Response usage(const std::string& adminApiKey, const time_t startTime);
}
