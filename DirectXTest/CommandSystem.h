#pragma once
#include "Command.h"
#include <deque>
#include <queue>
#include <memory>

// Manager class to handle commands
class CommandSystem {
public:
    static CommandSystem* getInstance();

    void queueCommand(std::shared_ptr<Command> command);

    void processQueue();

    void executeCommand(std::shared_ptr<Command> command);

    void undo();
    void redo();


	void setMaxQueueSize(size_t limit); // Queue size management
    void clearQueue();

private:
    CommandSystem() = default;
    ~CommandSystem() = default;

    std::queue<std::shared_ptr<Command>> m_commandQueue; // Command queue
    std::deque<std::shared_ptr<Command>> m_undoQueue; // undo queue
    std::deque<std::shared_ptr<Command>> m_redoQueue; // redo queue

    size_t m_maxQueueSize = 50;
};