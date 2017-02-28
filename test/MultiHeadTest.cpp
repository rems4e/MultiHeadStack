#include "../MultiHeadStack.h"
#include "gtest/gtest.h"

using namespace MultiHeadStackNS;

class MultiHeadStackTest : public testing::Test {
protected:
    MultiHeadStack<int> _stack;
};

TEST_F(MultiHeadStackTest, TwoRoots1) {
    auto &h1 = _stack.push(1);
    auto &h2 = _stack.push(2);

    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(1, h1.size());
    ASSERT_EQ(1, h2.size());

    ASSERT_EQ(nullptr, h1.parent());
    ASSERT_EQ(nullptr, h2.parent());
}

TEST_F(MultiHeadStackTest, TwoRoots2) {
    auto &h1 = _stack.push(1);
    auto &h11 = h1.push(2);
    auto &h2 = _stack.push(3);
    auto &h21 = h2.push(4);

    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(1, h1.size());
    ASSERT_EQ(1, h2.size());
    ASSERT_EQ(2, h11.size());
    ASSERT_EQ(2, h21.size());

    ASSERT_EQ(nullptr, h1.parent());
    ASSERT_EQ(nullptr, h2.parent());
    ASSERT_EQ(&h1, h11.parent());
    ASSERT_EQ(&h2, h21.parent());
}

TEST_F(MultiHeadStackTest, SplitAtRoot) {
    auto &h1 = _stack.push(1);
    auto &h2 = h1.push(2);
    auto &h3 = h1.push(3);

    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(1, h1.size());
    ASSERT_EQ(2, h2.size());
    ASSERT_EQ(2, h3.size());

    ASSERT_EQ(nullptr, h1.parent());
    ASSERT_EQ(&h1, h2.parent());
    ASSERT_EQ(&h1, h3.parent());
}
