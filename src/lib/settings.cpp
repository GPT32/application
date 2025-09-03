#include "settings.hpp"

#include "registry.hpp"

namespace lib::settings::apiKey {
    bool load(std::string& output) {
        return lib::registry::read(Key, output);
    }
    bool save(std::string& input) {
        return lib::registry::write(Key, input);
    }
}

namespace lib::settings::adminApiKey {
    bool load(std::string& output) {
        return lib::registry::read(Key, output);
    }
    bool save(std::string& input) {
        return lib::registry::write(Key, input);
    }
}

namespace lib::settings::alwaysOnTop {
    bool load(bool& output) {
        DWORD value = 0;

        if (!lib::registry::read(Key, value)) {
            output = false;
            return false;
        }

        output = (value != 0);
        return true;
    }

    bool save(bool input) {
        return lib::registry::write(Key, static_cast<DWORD>(input));
    }
}

namespace lib::settings::model {
    bool load(std::string& output) {
        return lib::registry::read(Key, output);
    }
    bool save(std::string& input) {
        return lib::registry::write(Key, input);
    }
}

namespace lib::settings::statLastChecked {
    bool load(std::chrono::system_clock::time_point& output) {
        DWORD64 value = 0;

        if (!lib::registry::read(Key, value)) {
            return false;
        }

        output = std::chrono::system_clock::time_point(std::chrono::seconds(value));
        return true;
    }
    bool save(const std::chrono::system_clock::time_point& input) {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(input.time_since_epoch());
        return lib::registry::write(Key, static_cast<DWORD64>(seconds.count()));
    }
}

namespace lib::settings::statCost {
    bool load(uint32_t& output) {
        DWORD value = 0;

        if (!lib::registry::read(Key, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(uint32_t input) {
        return lib::registry::write(Key, static_cast<DWORD>(input));
    }
}

namespace lib::settings::statInputTokens {
    bool load(uint32_t& output) {
        DWORD value = 0;

        if (!lib::registry::read(Key, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(uint32_t input) {
        return lib::registry::write(Key, static_cast<DWORD>(input));
    }
}

namespace lib::settings::statOutputTokens {
    bool load(uint32_t& output) {
        DWORD value = 0;

        if (!lib::registry::read(Key, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(uint32_t input) {
        return lib::registry::write(Key, static_cast<DWORD>(input));
    }
}

namespace lib::settings::leftPaneWidth {
    bool load(int& output) {
        DWORD value = 0;

        if (!lib::registry::read(Key, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(int input) {
        return lib::registry::write(Key, static_cast<DWORD>(input));
    }
}
