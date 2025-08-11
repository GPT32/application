#include "lib/storage.hpp"

#include <cassert>
#include <memory>
#include <ranges>
#include <vector>

/**
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    std::vector<std::unique_ptr<lib::storage::Project>> fixtures;

    // project "foo"
    auto projectFoo = std::make_unique<lib::storage::Project>();
    projectFoo->name = "foo";

    auto chatOne = std::make_unique<lib::storage::Chat>();
    chatOne->id = "7a98c250-6808-11cf-b73b-00aa00b677a7";
    chatOne->name = "one";
    chatOne->messages.emplace_back(std::make_unique<lib::storage::Message>("abc123", "Hello from chat one"));
    chatOne->messages.emplace_back(std::make_unique<lib::storage::Message>("", "Another message"));

    auto chatTwo = std::make_unique<lib::storage::Chat>();
    chatTwo->name = "two";

    projectFoo->chats.emplace_back(std::move(chatOne));
    projectFoo->chats.emplace_back(std::move(chatTwo));
    fixtures.emplace_back(std::move(projectFoo));

    // project "bar"
    auto projectBar = std::make_unique<lib::storage::Project>();
    projectBar->name = "bar";
    projectBar->instructions = "bar's instructions";
    projectBar->chats.emplace_back(std::make_unique<lib::storage::Chat>("", "three"));
    projectBar->chats.emplace_back(std::make_unique<lib::storage::Chat>("", "four"));
    fixtures.emplace_back(std::move(projectBar));

    // project "baz"
    auto projectBaz = std::make_unique<lib::storage::Project>();
    projectBaz->name = "baz";
    projectBaz->chats.emplace_back(std::make_unique<lib::storage::Chat>("", "fix"));
    projectBaz->chats.emplace_back(std::make_unique<lib::storage::Chat>("", "six"));
    fixtures.emplace_back(std::move(projectBaz));

    // save and load
    lib::storage::save(fixtures, lib::storage::FILENAME);
    auto projects = lib::storage::load(lib::storage::FILENAME);

    for (auto const& [i, project] : fixtures | std::views::enumerate) {
        assert(project->name == projects[i]->name);
        assert(project->chats.size() == projects[i]->chats.size());
    }

    assert(projects[0]->chats[0]->id == "7a98c250-6808-11cf-b73b-00aa00b677a7");
    assert(projects[2]->chats[1]->name == "six");
    assert(projects[0]->chats[0]->messages.size() == 2);
    assert(projects[0]->chats[0]->messages[0]->id == "abc123");
    assert(projects[0]->chats[0]->messages[0]->content == "Hello from chat one");
    assert(projects[1]->instructions == "bar's instructions");

    // delete project
    bool deleted = lib::storage::deleteProject(projects, "baz");
    assert(deleted);
    assert(projects.size() == 2);
    assert(projects[0]->name == "foo");
    assert(projects[1]->name == "bar");

    // delete chat
    deleted = lib::storage::deleteChat(*projects[1], "three");
    assert(deleted);
    assert(projects[1]->chats.size() == 1);
    assert(projects[1]->chats[0]->name == "four");

    // re-save and reload to validate persistence
    lib::storage::save(projects, lib::storage::FILENAME);
    auto persisted = lib::storage::load(lib::storage::FILENAME);
    assert(persisted.size() == 2);
    assert(persisted[0]->name == "foo");
    assert(persisted[1]->name == "bar");
    assert(persisted[1]->chats.size() == 1);
    assert(persisted[1]->chats[0]->name == "four");

    return 0;
}
