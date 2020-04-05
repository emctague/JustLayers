// Engine - 
// 
// Copyright (c) 2020 Ethan McTague
#include "JustLayers/Taskmaster.h"
#include <chrono>

namespace jl {

    void Taskmaster::addTask(std::shared_ptr <ITask> task) {
        tasks.push_back(task);
    }

    void Taskmaster::start() {
        auto prevFrameTime = std::chrono::high_resolution_clock::now();



        while (running && !tasks.empty()) {

            // Calculate delta time in seconds
            auto newFrameTime = std::chrono::high_resolution_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(newFrameTime - prevFrameTime).count() / 1000.0f;
            prevFrameTime = newFrameTime;

            // Update all tasks
            for (auto it = tasks.begin(); it != tasks.end();) {
                if (!it->get()->updateTask(this, diff))
                    it = tasks.erase(it);
                else it++;
            }
        }
    }

    void Taskmaster::stop() {
        running = false;
    }

    IndirectTask::IndirectTask(ITask *otherTask) : task(otherTask) {

    }

    bool IndirectTask::updateTask(Taskmaster *taskmaster, float delta) {
        return task->updateTask(taskmaster, delta);
    }
}