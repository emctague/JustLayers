// ECSTests - 
// 
// Copyright (c) 2020 Ethan McTague

#include <gtest/gtest.h>
#include <JustLayers/ECS.h>

#define COMPO(LETTER) class Component##LETTER : public jl::Component { \
    int value; \
public: \
    explicit Component##LETTER(int v) : value(v){} \
    virtual int getValue() { return value; } \
};

COMPO(A);
COMPO(B);
COMPO(C);

TEST(ECS, CanAddAndRetrieveComponents) {
    jl::Entity entity;

    auto ca = new ComponentA(10);
    auto cb = new ComponentB(12);

    entity.addComponent(ca)
          ->addComponent(cb);

    EXPECT_EQ(entity.getComponent<ComponentA>().get(), ca);
    EXPECT_EQ(entity.getComponent<ComponentB>().get(), cb);
    EXPECT_EQ(entity.getComponent<ComponentC>().get(), nullptr);
}

class DemoListener : public jl::IECSListener {
public:
    int addCount = 0;
    int removeCount = 0;
    jl::Entity *latestAdded = nullptr;
    jl::Entity *latestRemoved = nullptr;

    void entityAdded(jl::Entity *entity) override {
        addCount++;
        latestAdded = entity;
    }

    void entityRemoving(jl::Entity *entity) override {
        removeCount++;
        latestRemoved = entity;
    }

};

TEST(ECS, CanAddAndRemoveListeners) {
    jl::ECS ecs;

    DemoListener dl;
    ecs.addECSListener(&dl);

    EXPECT_EQ(dl.addCount, 0);
    EXPECT_EQ(dl.removeCount, 0);

    auto *newEntity = new jl::Entity();
    ecs.addEntity(newEntity);

    EXPECT_EQ(dl.addCount, 1);
    EXPECT_EQ(dl.removeCount, 0);
    EXPECT_EQ(dl.latestAdded, newEntity);

    ecs.removeEntity(newEntity);

    EXPECT_EQ(dl.addCount, 1);
    EXPECT_EQ(dl.removeCount, 1);
    EXPECT_EQ(dl.latestRemoved, newEntity);

    ecs.removeECSListener(&dl);

    ecs.addEntity(new jl::Entity());
    EXPECT_EQ(dl.addCount, 1);
    EXPECT_EQ(dl.removeCount, 1);
}
