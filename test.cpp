#include "MultiHeadStack.h"
#include "gtest/gtest.h"

using namespace MultiHeadStackNS;

class StackTest : public testing::Test {
protected:
    MultiHeadStack<int> _stack;
};

TEST_F(StackTest, DefaultConstructor) {
    ASSERT_EQ(0, _stack.size());
}

TEST_F(StackTest, FillSize) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    ASSERT_EQ(5, headFill.size());
}

TEST_F(StackTest, PushSize) {
    auto &pushFill = _stack.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(5, pushFill.size());
}
