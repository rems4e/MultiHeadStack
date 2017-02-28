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
    ASSERT_EQ(0, _stack.size());
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    ASSERT_EQ(5, headFill.size());
    ASSERT_EQ(0, _stack.size());
}

TEST_F(StackTest, PushSize) {
    ASSERT_EQ(0, _stack.size());
    auto &pushFill = _stack.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(5, pushFill.size());
    ASSERT_EQ(0, _stack.size());
}

TEST_F(StackTest, FillEqPush) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &pushFill = stack2.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(0, stack2.size());

    ASSERT_EQ(pushFill, headFill);
}

TEST_F(StackTest, Equal1) {
    decltype(_stack) stack2;
    ASSERT_EQ(_stack, stack2);
}

TEST_F(StackTest, Equal2) {
    auto &headFill = _stack.fill(1, 2, 3);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3);

    ASSERT_EQ(headFill2, headFill);
}

TEST_F(StackTest, Equal3) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3, 4, 5);

    ASSERT_EQ(headFill2, headFill);
}

TEST_F(StackTest, NotEqual1) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;

    ASSERT_NE(stack2, headFill);
}

TEST_F(StackTest, NotEqual2) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(StackTest, NotEqual3) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3, 4);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(StackTest, NotEqual4) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3, 4, 6);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(StackTest, NotEqual5) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(0, 2, 3, 4, 5);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(StackTest, NotEqual6) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 7, 4, 5);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(StackTest, ParentEmpty) {
    ASSERT_EQ(nullptr, _stack.parent());
}

TEST_F(StackTest, Parent1) {
    auto &headFill = _stack.fill(1);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill.parent());
}

TEST_F(StackTest, Parent2) {
    auto &headFill1 = _stack.push(1);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill1.parent());

    auto &headFill2 = headFill1.push(2);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill1.parent());
    ASSERT_EQ(&headFill1, headFill2.parent());

    auto &headFill3 = headFill2.push(3);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill1.parent());
    ASSERT_EQ(&headFill1, headFill2.parent());
    ASSERT_EQ(&headFill2, headFill3.parent());
}

TEST_F(StackTest, ToStringEmpty) {
    ASSERT_EQ("", to_string(_stack));
}

TEST_F(StackTest, ToStringOne) {
    auto &headFill = _stack.push(1);
    ASSERT_EQ("1", to_string(headFill));
    ASSERT_EQ("", to_string(_stack));
}

TEST_F(StackTest, ToStringIntermediate) {
    auto &headFill1 = _stack.push(1);
    ASSERT_EQ("", to_string(_stack));
    ASSERT_EQ("1", to_string(headFill1));

    auto &headFill2 = headFill1.push(2);
    ASSERT_EQ("", to_string(_stack));
    ASSERT_EQ("1", to_string(headFill1));
    ASSERT_EQ("1 2", to_string(headFill2));

    auto &headFill3 = headFill2.push(3);
    ASSERT_EQ("", to_string(_stack));
    ASSERT_EQ("1", to_string(headFill1));
    ASSERT_EQ("1 2", to_string(headFill2));
    ASSERT_EQ("1 2 3", to_string(headFill3));
}

TEST_F(StackTest, ToStringFull) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    ASSERT_EQ("1 2 3 4 5", to_string(headFill));
    ASSERT_EQ("", to_string(_stack));
}

TEST_F(StackTest, ToStringFillPush) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &pushFill = stack2.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(0, stack2.size());

    ASSERT_EQ("1 2 3 4 5", to_string(headFill));
    ASSERT_EQ("1 2 3 4 5", to_string(pushFill));
}
