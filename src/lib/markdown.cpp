#include "markdown.hpp"

namespace lib::markdown {
    std::string escapeRTF(const std::string& text) {
        std::string out;
        for (char ch : text) {
            switch (ch) {
                case '\\':
                    out += "\\\\";
                    break;
                case '{':
                    out += "\\{";
                    break;
                case '}':
                    out += "\\}";
                    break;
                default:
                    out += ch;
                    break;
            }
        }

        return out;
    }

    std::string toRTF(const std::string& text) {
        std::ostringstream out;
        std::istringstream in(text);

        std::string line;
        std::string rtf;

        bool inCodeBlock = false;

        while (std::getline(in, line)) {
            std::string escaped = escapeRTF(line);

            if (std::regex_match(escaped, std::regex(R"(^```([\w\-\+]*)\s*$)"))) {
                inCodeBlock = !inCodeBlock;
                rtf += inCodeBlock ? R"({\f1)" : R"(}\par )";
                continue;
            }

            if (inCodeBlock) {
                rtf += escaped + R"(\line )";
                continue;
            }

            // headers
            for (int level = 6; level >= 1; --level) {
                std::string prefix(level, '#');
                if (escaped.starts_with(prefix + " ")) {
                    int fontSize = 48 - (level - 1) * 6;
                    std::string content = escaped.substr(level + 1);
                    escaped = R"({\fs)" + std::to_string(fontSize) + R"( \b )" + content + R"(\b0})";
                    break;
                }
            }

            // bulleted list
            for (int level = 6; level >= 0; --level) {
                std::string indent(level * 2, ' ');  // 4 spaces per level
                std::string prefix1 = indent + "- ";
                std::string prefix2 = indent + "* ";

                if (escaped.starts_with(prefix1) || escaped.starts_with(prefix2)) {
                    int rtfIndent = 720 * (level + 1);
                    std::string content = escaped.substr(prefix1.length());  // same length for `- ` and `* `
                    escaped = R"({\pard\li)" + std::to_string(rtfIndent) + R"(\tx)" + std::to_string(rtfIndent) +
                              R"(\fi-360\bullet\tab )" + content + R"(\par})";
                    break;
                }
            }

            // inline code blocks
            escaped = std::regex_replace(escaped, std::regex(R"(`([^`]+?)`)"), R"({\f1 $1})");

            // bold
            escaped = std::regex_replace(escaped, std::regex(R"(\*\*(.+?)\*\*)"), R"({\b $1\b0})");

            // italic
            escaped = std::regex_replace(escaped, std::regex(R"(\*(.+?)\*)"), R"({\i $1\i0})");
            escaped = std::regex_replace(escaped, std::regex(R"(_(.+?)_)"), R"({\i $1\i0})");

            // hyperlinks
            escaped = std::regex_replace(escaped,
                std::regex(R"(\[([^\]]+)\]\(([^)]+)\))"),
                R"({\field{\*\fldinst{HYPERLINK "$2"}}{\fldrslt $1}})");

            // close the paragraph if not already
            if (!std::regex_search(escaped, std::regex(R"(\\par\}?$)"))) {
                rtf += escaped + R"(\par )";
            } else {
                rtf += escaped;
            }
        }

        out << rtf;
        return out.str();
    }
}
