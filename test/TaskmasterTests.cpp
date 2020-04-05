// TaskmasterTests - 
// 
// Copyright (c) 2020 Ethan McTague

#include <gtest/gtest.h>
#include <JustLayers/Taskmaster.h>

class CountAndCancelTask : public jl::ITask {
public:
    int count = 0;
    bool updateTask(jl::Taskmaster *taskmaster, float delta) override {
        count++;

        return (count < 10);
    }
};

class CountAndStopTask : public jl::ITask {
public:
    int count = 0;
    bool updateTask(jl::Taskmaster *taskmaster, float delta) override {
        count++;

        if (count >= 20) taskmaster->stop();

        return true;
    }
};

TEST(Taskmaster, HandlesTasksCorrectly) {
    CountAndCancelTask cct;
    CountAndStopTask cst;

    jl::Taskmaster taskmaster;
    taskmaster.addTask(jl::IndirectTask::make(&cct));
    taskmaster.addTask(jl::IndirectTask::make(&cst));

    taskmaster.start();

    ASSERT_EQ(cct.count, 10);
    ASSERT_EQ(cst.count, 20);
}
