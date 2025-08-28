#pragma once
#include <memory>
#include <string>
#include <vector>

/**
 * @namespace lib::storage
 * @brief Contains functions and types for serializing and deserializing project tree data.
 *
 * The project tree data consists of a project which contains zero to many chats.
 *
 * ```bash
 * [uint32] Number of Projects
 *      [Project 1]
 *          [uint32] Length of project name
 *          [bytes] Project name
 *
 *          [uint32] Length of project instructions
 *          [bytes] Project instructions
 *
 *          [uint32] Number of Chats
 *              [Chat 1]
 *                  [uint32] Length of chat id
 *                  [bytes] Chat id
 *
 *                  [uint32] Length of chat name
 *                  [bytes] Chat name
 *
 *                  [uint32] Number of input tokens
 *                  [uint32] Number of output tokens
 *
 *                  [uint32] Number of Messages
 *                      [Message 1]
 *                          [uint32] Length of message id
 *                          [bytes] Message id
 *                          [uint32] Length of message content
 *                          [bytes] Chat content
 * ```
 */
namespace lib::storage {
    /** @brief The default storage filename. */
    const std::string FILENAME = "data.bin";

    /** @brief Project struct forward declaration. */
    struct Project;

    /** @brief A chat can have many messages. */
    struct Message {
        std::string id;
        std::string content;
    };

    /** @brief A project can have many chats. */
    struct Chat {
        std::string id;
        std::string name;
        uint32_t inputTokens;
        uint32_t outputTokens;
        std::vector<std::unique_ptr<Message>> messages;
        Project* parent = nullptr;
    };

    /** @brief A single project. */
    struct Project {
        std::string name;
        std::string instructions;
        std::vector<std::unique_ptr<Chat>> chats;
    };

    /**
     * @brief Deletes the specified chat.
     * @param project   The project.
     * @param name      The chat name.
     * @return bool Successful deletion.
     */
    bool deleteChat(Project& project, const std::string& name);

    /**
     * @brief Deletes the specified project.
     * @param projects  Vector of Projects.
     * @param name      The project name.
     * @return bool Successful deletion.
     */
    bool deleteProject(std::vector<std::unique_ptr<Project>>& projects, const std::string& name);

    /**
     * @brief Loads a list of projects from a binary file.
     * @param filename Path to the binary file to read.
     * @return Vector of loaded Project objects.
     */
    std::vector<std::unique_ptr<Project>> load(const std::string& filename);

    /**
     * @brief Reads a string from an input stream in binary format.
     *
     * ```bash
     * [uint32] Length
     * [bytes] UTF-8 string data
     * ```
     *
     * @param in    Input stream to read from.
     * @param str   Output string to populate.
     */
    void readString(std::ifstream& in, std::string& str);

    /**
     * @brief Reads a uint32 from an input stream in binary format.
     * @param in    Input stream to read from.
     * @param value Output uint32 to populate.
     */
    void readUInt32(std::ifstream& in, uint32_t& value);

    /**
     * @brief Saves a list of projects to a binary file.
     *
     * The format written is compatible with the `load()` function.
     *
     * @param projects  Vector of Project objects to save.
     * @param filename  Path to the binary file to write.
     */
    void save(const std::vector<std::unique_ptr<Project>>& projects, const std::string& filename);

    /**
     * @brief Writes a string to an output stream in binary format.
     *
     * ```bash
     * [uint32] Length
     * [bytes] UTF-8 string data
     * ```
     *
     * @param out Output stream to write to.
     * @param str String to write.
     */
    void writeString(std::ofstream& out, const std::string& str);

    /**
     * @brief Writes a uint32 to an output stream in binary format.
     * @param out   Ouput stream to write to.
     * @param value Uint32 to write.
     */
    void writeUInt32(std::ofstream& out, uint32_t& value);
};
