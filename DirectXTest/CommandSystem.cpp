#include "CommandSystem.h"

CommandSystem* CommandSystem::getInstance() {
    static CommandSystem instance;
    return &instance;
}

void CommandSystem::queueCommand(std::shared_ptr<Command> command) {
    m_commandQueue.push(command);
}

void CommandSystem::processQueue() {
    while (!m_commandQueue.empty()) {
        auto cmd = m_commandQueue.front();
        m_commandQueue.pop();
        executeCommand(cmd);
    }
}

void CommandSystem::executeCommand(std::shared_ptr<Command> command) {
    command->execute();

    m_undoQueue.push_front(command);

    // When new action is done clear the redo
    m_redoQueue.clear();

    // Manage queue size
    if (m_undoQueue.size() > m_maxQueueSize) {
        m_undoQueue.pop_back(); // Remove oldest command
    }
}

void CommandSystem::undo() {
    if (m_undoQueue.empty()) return;

	// Get the recent command from the undo queue
    auto cmd = m_undoQueue.front();
    m_undoQueue.pop_front();

	cmd->undo(); // call the undo method of the command

	m_redoQueue.push_front(cmd); // Add the undone command to the redo queue
}

void CommandSystem::redo() {
    if (m_redoQueue.empty()) return;

	// Get the recent command from the redo queue
    auto cmd = m_redoQueue.front();
    m_redoQueue.pop_front();

	cmd->execute(); // reexecute the command

	m_undoQueue.push_front(cmd); // Add the redone command back to the undo queue
}

// Set the maximum queue size for the undo queue
void CommandSystem::setMaxQueueSize(size_t limit) {
    m_maxQueueSize = limit;
    while (m_undoQueue.size() > m_maxQueueSize) {
        m_undoQueue.pop_back();
    }
}

// Clear all done commands from the queue
void CommandSystem::clearQueue() {
    m_undoQueue.clear();
    m_redoQueue.clear();
    std::queue<std::shared_ptr<Command>> empty;
    std::swap(m_commandQueue, empty);
}