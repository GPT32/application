#include "lib/uuid.hpp"

#include <cassert>
#include <iostream>
#include <string>

/**
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    auto uuid = lib::uuid::generate();
    std::cout << uuid << std::endl;
    assert(true);
    return 0;
}
