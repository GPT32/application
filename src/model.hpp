#pragma once

#include <unordered_set>

#include "lib/storage.hpp"

/**
 * @class Model
 * @brief Application data.
 */
class Model {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static Model& Instance();

    /** @brief Tree view data. */
    std::vector<std::unique_ptr<lib::storage::Project>> projects;

    /** @brief Tracks which chats are awaiting api responses. */
    std::unordered_set<std::string> tasks;

   private:
    /** @brief Prevents external instantiation of the class. */
    Model() = default;
};
