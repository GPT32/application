#pragma once
#include <regex>
#include <sstream>
#include <string>

/**
 * @namespace lib::markdown
 * @brief Functions for handling markdown.
 */
namespace lib::markdown {
    std::string escapeRTF(const std::string& text);
    std::string toRTF(const std::string& text);
}
