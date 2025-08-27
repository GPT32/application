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
        return lib::registry::write(Key, input);
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
