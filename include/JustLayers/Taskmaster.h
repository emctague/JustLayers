// Taskmaster - Provides a means of running actions repeatedly.
// 
// Copyright (c) 2020 Ethan McTague
#pragma once

#include <vector>

namespace jl {

    class Taskmaster;


    /// A task provides some function that is invoked repeatedly until it is no longer needed.
    class ITask {
    public:

        /// Called repeatedly by the taskmaster.
        /// @returns True if the task should *continue*, and false if it should be removed.
        virtual bool updateTask(Taskmaster *taskmaster) = 0;

        virtual ~ITask() = default;

    };


    /// An IndirectTask allows tasks that cannot be shared_ptr to be run indirectly through it.
    class IndirectTask : public ITask {
        ITask *task;

    public:

        explicit IndirectTask(ITask *otherTask);

        bool updateTask(Taskmaster *taskmaster) override;

        static inline std::shared_ptr<IndirectTask> make(ITask *task) {
            return std::make_shared<IndirectTask>(task);
        }

    };



    /// The taskmaster runs a main loop which updates tasks.
    class Taskmaster {

        /// A list of tasks to be called every frame.
        std::vector<std::shared_ptr<ITask>> tasks;

        /// Indicates if the taskmaster should keep running.
        bool running = true;

    public:

        /// Adds a task to the taskmaster's task list.
        void addTask(std::shared_ptr<ITask> task);

        /// Starts the taskmaster's main loop. This is a blocking operation.
        void start();

        /// Requests that the taskmaster stop on the next frame.
        void stop();

    };

}