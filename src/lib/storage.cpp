#include "storage.hpp"

#include <fstream>

namespace lib::storage {
    bool deleteChat(Project& project, const std::string& name) {
        auto chat =
            std::find_if(project.chats.begin(), project.chats.end(), [&](const auto& c) { return c->name == name; });

        if (chat != project.chats.end()) {
            project.chats.erase(chat);
            return true;
        }

        return false;
    }

    bool deleteProject(std::vector<std::unique_ptr<Project>>& projects, const std::string& name) {
        auto project = std::find_if(projects.begin(), projects.end(), [&](const auto& p) { return p->name == name; });

        if (project != projects.end()) {
            projects.erase(project);
            return true;
        }

        return false;
    }

    std::vector<std::unique_ptr<Project>> load(const std::string& filename) {
        std::vector<std::unique_ptr<Project>> projects;
        std::ifstream in(filename, std::ios::binary);

        if (!in) {
            return projects;
        }

        uint32_t projectCount;
        in.read(reinterpret_cast<char*>(&projectCount), sizeof(projectCount));

        for (uint32_t i = 0; i < projectCount; i++) {
            auto project = std::make_unique<Project>();
            readString(in, project->name);
            readString(in, project->instructions);

            uint32_t chatCount;
            in.read(reinterpret_cast<char*>(&chatCount), sizeof(chatCount));

            for (uint32_t j = 0; j < chatCount; j++) {
                auto chat = std::make_unique<Chat>();
                chat->parent = project.get();
                readString(in, chat->id);
                readString(in, chat->name);

                uint32_t messageCount;
                in.read(reinterpret_cast<char*>(&messageCount), sizeof(messageCount));

                for (uint32_t k = 0; k < messageCount; k++) {
                    auto message = std::make_unique<Message>();
                    readString(in, message->id);
                    readString(in, message->content);
                    chat->messages.push_back(std::move(message));
                }

                project->chats.push_back(std::move(chat));
            }

            projects.push_back(std::move(project));
        }

        return projects;
    }

    void readString(std::ifstream& in, std::string& str) {
        // grab string length header
        uint32_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));

        // read the content based on string length
        str.resize(len);
        in.read(&str[0], len);
    }

    void save(const std::vector<std::unique_ptr<Project>>& projects, const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        uint32_t projectCount = static_cast<uint32_t>(projects.size());
        out.write(reinterpret_cast<const char*>(&projectCount), sizeof(projectCount));

        for (const auto& project : projects) {
            writeString(out, project->name);
            writeString(out, project->instructions);

            uint32_t chatCount = static_cast<uint32_t>(project->chats.size());
            out.write(reinterpret_cast<const char*>(&chatCount), sizeof(chatCount));

            for (const auto& chat : project->chats) {
                writeString(out, chat->id);
                writeString(out, chat->name);

                uint32_t messageCount = static_cast<uint32_t>(chat->messages.size());
                out.write(reinterpret_cast<const char*>(&messageCount), sizeof(messageCount));

                for (const auto& message : chat->messages) {
                    writeString(out, message->id);
                    writeString(out, message->content);
                }
            }
        }
    }

    void writeString(std::ofstream& out, const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(str.data(), len);
    }
}
