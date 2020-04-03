// Engine - 
// 
// Copyright (c) 2020 Ethan McTague
#include "JustLayers/Taskmaster.h"

namespace jl {

    void Taskmaster::addTask(std::shared_ptr <ITask> task) {
        tasks.push_back(task);
    }

    void Taskmaster::start() {
        while (running && !tasks.empty()) {
            for (auto it = tasks.begin(); it != tasks.end(); it++) {
                if (!it->get()->updateTask(this))
                    tasks.erase(it--);
            }
        }
    }

    void Taskmaster::stop() {
        running = false;
    }

    IndirectTask::IndirectTask(ITask *otherTask) : task(otherTask) {

    }

    bool IndirectTask::updateTask(Taskmaster *taskmaster) {
        return task->updateTask(taskmaster);
    }
}