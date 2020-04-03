// ResourceManagerTests - 
// 
// Copyright (c) 2020 Ethan McTague

#include <gtest/gtest.h>
#include <string>
#include <JustLayers/ResourceManager.h>

class ResourceType {
public:
    static int totalResourceTypes;
    std::string id;
    int counter;
    ResourceType(std::string resourceIdentifier) : id(resourceIdentifier), counter(totalResourceTypes++) {}
};

int ResourceType::totalResourceTypes = 0;


class ResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = new jl::ResourceManager<ResourceType, std::string>([](std::string key) { return std::make_shared<ResourceType>(key); });
    }

    void TearDown() override {
        delete manager;
    }

    jl::ResourceManager<ResourceType, std::string> *manager;
};

TEST_F(ResourceTest, NewResourcesAreConstructed) {
    auto newRes = manager->get("Hello");
    EXPECT_NE(newRes.get(), nullptr);
    EXPECT_EQ(newRes->id, "Hello");
}

TEST_F(ResourceTest, UniqueResourcesAreSeparate) {
    auto resA = manager->get("HelloA");
    auto resB = manager->get("HelloB");

    EXPECT_NE(resA.get(), resB.get());
    EXPECT_NE(resA.get(), nullptr);
    EXPECT_NE(resB.get(), nullptr);
    EXPECT_EQ(resA->id, "HelloA");
    EXPECT_EQ(resB->id, "HelloB");
    EXPECT_NE(resA->counter, resB->counter);
}

TEST_F(ResourceTest, NonUniqueResourcesAreIdentical) {
    auto resA = manager->get("FooA");
    auto resB = manager->get("FooA");

    EXPECT_EQ(resA.get(), resB.get());
    EXPECT_EQ(resA->id, resB->id);
    EXPECT_EQ(resA->counter, resB->counter);
}


TEST_F(ResourceTest, UnusedResourcesArePurged) {
    auto resAPtr = manager->get("BarA").get();
    auto resACount = resAPtr->counter;
    auto resAId = resAPtr->id;
    EXPECT_NE(resAPtr, nullptr);

    manager->cleanup();

    auto resB = manager->get("BarA");
    EXPECT_NE(resACount, resB->counter);
    EXPECT_EQ(resAId, resB->id);
}
