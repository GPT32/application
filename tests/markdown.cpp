#include "lib/markdown.hpp"

#include <cassert>
#include <string>

/**
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    // generic formatting tests
    assert(lib::markdown::escapeRTF("This {is} a test\\") == "This \\{is\\} a test\\\\");
    assert(lib::markdown::toRTF("Plain text") == "Plain text\\line ");
    assert(lib::markdown::toRTF("line1\nline2") == "line1\\line line2\\line ");
    assert(lib::markdown::toRTF("end\n") == "end\\line ");
    assert(lib::markdown::toRTF("**This {needs} escaping**") == "{\\b This \\{needs\\} escaping}\\line ");

    // bold
    assert(lib::markdown::toRTF("This is **bold** text") == "This is {\\b bold} text\\line ");
}
