#include "settings.hpp"

#include "registry.hpp"

namespace lib::settings::apiKey {
    bool load(std::string& output) {
        return lib::Registry::Instance().read(SubKey, output);
    }
    bool save(std::string& input) {
        return lib::Registry::Instance().write(SubKey, input);
    }
}

namespace lib::settings::adminApiKey {
    bool load(std::string& output) {
        return lib::Registry::Instance().read(SubKey, output);
    }
    bool save(std::string& input) {
        return lib::Registry::Instance().write(SubKey, input);
    }
}

namespace lib::settings::alwaysOnTop {
    bool load(bool& output) {
        DWORD value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            output = false;
            return false;
        }

        output = (value != 0);
        return true;
    }

    bool save(bool input) {
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD>(input));
    }
}

namespace lib::settings::model {
    bool load(std::string& output) {
        return lib::Registry::Instance().read(SubKey, output);
    }
    bool save(std::string& input) {
        return lib::Registry::Instance().write(SubKey, input);
    }
}

namespace lib::settings::statLastChecked {
    bool load(std::chrono::system_clock::time_point& output) {
        DWORD64 value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            return false;
        }

        output = std::chrono::system_clock::time_point(std::chrono::seconds(value));
        return true;
    }
    bool save(const std::chrono::system_clock::time_point& input) {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(input.time_since_epoch());
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD64>(seconds.count()));
    }
}

namespace lib::settings::statCost {
    bool load(uint32_t& output) {
        DWORD value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(uint32_t input) {
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD>(input));
    }
}

namespace lib::settings::statInputTokens {
    bool load(uint32_t& output) {
        DWORD value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(uint32_t input) {
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD>(input));
    }
}

namespace lib::settings::statOutputTokens {
    bool load(uint32_t& output) {
        DWORD value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(uint32_t input) {
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD>(input));
    }
}

namespace lib::settings::leftPaneWidth {
    bool load(int& output) {
        DWORD value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            return false;
        }

        output = value;
        return true;
    }

    bool save(int input) {
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD>(input));
    }
}

namespace lib::settings::windowPosition {
    bool load(RECT& output) {
        bool ok = true;

        DWORD top, bottom;
        DWORD left, right;

        ok &= lib::Registry::Instance().read(std::string(SubKey) + "Top", output.top);
        ok &= lib::Registry::Instance().read(std::string(SubKey) + "Bottom", output.bottom);
        ok &= lib::Registry::Instance().read(std::string(SubKey) + "Left", output.left);
        ok &= lib::Registry::Instance().read(std::string(SubKey) + "Right", output.right);

        lib::Registry::Instance().close();

        return ok;
    }

    bool save(RECT input) {
        bool ok = true;

        ok &= lib::Registry::Instance().write(std::string(SubKey) + "Top", input.top);
        ok &= lib::Registry::Instance().write(std::string(SubKey) + "Bottom", input.bottom);
        ok &= lib::Registry::Instance().write(std::string(SubKey) + "Left", input.left);
        ok &= lib::Registry::Instance().write(std::string(SubKey) + "Right", input.right);

        lib::Registry::Instance().close();

        return ok;
    }
}

namespace lib::settings::viewLeftPanel {
    bool load(bool& output) {
        DWORD value = 0;

        if (!lib::Registry::Instance().read(SubKey, value)) {
            output = false;
            return false;
        }

        output = (value != 0);
        return true;
    }

    bool save(bool input) {
        return lib::Registry::Instance().write(SubKey, static_cast<DWORD>(input));
    }
}
