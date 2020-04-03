// Tests - 
// 
// Copyright (c) 2020 Ethan McTague

#include <gtest/gtest.h>
#include <JustLayers/Registry.h>


/// A base registered type to test on.
class RegistryBase {
public:
    virtual std::string getResult() = 0;
    virtual ~RegistryBase() = default;
};

/// Create RegistryBase subclasses that append '_' and a letter to the text.
#define SUBTYPE(LETTER) class RegistryType##LETTER : public RegistryBase { \
    std::string text; \
public: \
    RegistryType##LETTER(std::string text) : text(text) {} \
    std::string getResult() override { \
        return text + "_" + #LETTER; \
    } \
};

SUBTYPE(A)
SUBTYPE(B)

/// A registry of RegistryBase subclasses
/// A registry of RegistryBase subclasses
class DemoRegistry : public jl::Registry<RegistryBase, std::string> {
};



class RegistryTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry.add("RegistryTypeA", [](std::string text) { return new RegistryTypeA(text); });
        registry.add("RegistryTypeB", [](std::string text) { return new RegistryTypeB(text); });

        no_type = registry.create("RegistryTypeBad", "Test");
        a_type = registry.create("RegistryTypeA", "Test");
        b_type = registry.create("RegistryTypeB", "Test");
    }

    void TearDown() override {
        delete a_type;
        delete b_type;
    }

    DemoRegistry registry;
    RegistryBase *no_type;
    RegistryBase *a_type;
    RegistryBase *b_type;
};


TEST_F(RegistryTest, ProducesNullOnInvalid) {
    EXPECT_EQ(no_type, nullptr);
}

TEST_F(RegistryTest, ProducesValidPointersForRegisteredTypes) {
    EXPECT_NE(a_type, nullptr);
    EXPECT_NE(b_type, nullptr);
}

TEST_F(RegistryTest, ProducesCorrectType) {
    EXPECT_EQ(a_type->getResult(), "Test_A");
    EXPECT_EQ(b_type->getResult(), "Test_B");
}
