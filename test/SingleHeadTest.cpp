#include "../MultiHeadStack.h"
#include "gtest/gtest.h"

using namespace MultiHeadStackNS;

class SingleHeadStackTest : public testing::Test {
protected:
    MultiHeadStack<int> _stack;
};

TEST_F(SingleHeadStackTest, DefaultConstructor) {
    ASSERT_EQ(0, _stack.size());
}

TEST_F(SingleHeadStackTest, FillSize) {
    ASSERT_EQ(0, _stack.size());
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    ASSERT_EQ(5, headFill.size());
    ASSERT_EQ(0, _stack.size());
}

TEST_F(SingleHeadStackTest, PushSize) {
    ASSERT_EQ(0, _stack.size());
    auto &pushFill = _stack.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(5, pushFill.size());
    ASSERT_EQ(0, _stack.size());
}

TEST_F(SingleHeadStackTest, FillEqPush) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &pushFill = stack2.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(0, stack2.size());

    ASSERT_EQ(pushFill, headFill);
}

TEST_F(SingleHeadStackTest, Equal1) {
    decltype(_stack) stack2;
    ASSERT_EQ(_stack, stack2);
}

TEST_F(SingleHeadStackTest, Equal2) {
    auto &headFill = _stack.fill(1, 2, 3);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3);

    ASSERT_EQ(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, Equal3) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3, 4, 5);

    ASSERT_EQ(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, NotEqual1) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;

    ASSERT_NE(stack2, headFill);
}

TEST_F(SingleHeadStackTest, NotEqual2) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, NotEqual3) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3, 4);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, NotEqual4) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 3, 4, 6);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, NotEqual5) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(0, 2, 3, 4, 5);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, NotEqual6) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &headFill2 = _stack.fill(1, 2, 7, 4, 5);

    ASSERT_NE(headFill2, headFill);
}

TEST_F(SingleHeadStackTest, PushIntermediate) {
    auto &headFill1 = _stack.push(1);
    ASSERT_EQ(1, headFill1.value());

    auto &headFill2 = headFill1.push(2);
    ASSERT_EQ(1, headFill1.value());
    ASSERT_EQ(2, headFill2.value());

    auto &headFill3 = headFill2.push(3);
    ASSERT_EQ(1, headFill1.value());
    ASSERT_EQ(2, headFill2.value());
    ASSERT_EQ(3, headFill3.value());

    auto &headFill4 = headFill3.push(4);
    ASSERT_EQ(1, headFill1.value());
    ASSERT_EQ(2, headFill2.value());
    ASSERT_EQ(3, headFill3.value());
    ASSERT_EQ(4, headFill4.value());
}

TEST_F(SingleHeadStackTest, ParentEmpty) {
    ASSERT_EQ(nullptr, _stack.parent());
}

TEST_F(SingleHeadStackTest, Parent1) {
    auto &headFill = _stack.fill(1);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill.parent());
}

TEST_F(SingleHeadStackTest, Parent2) {
    auto &headFill1 = _stack.push(1);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill1.parent());

    auto &headFill2 = headFill1.push(2);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill1.parent());
    ASSERT_EQ(&headFill1, headFill2.parent());
    ASSERT_EQ(1, headFill1.value());

    auto &headFill3 = headFill2.push(3);
    ASSERT_EQ(nullptr, _stack.parent());
    ASSERT_EQ(nullptr, headFill1.parent());
    ASSERT_EQ(&headFill1, headFill2.parent());
    ASSERT_EQ(&headFill2, headFill3.parent());
    ASSERT_EQ(2, headFill2.value());
}

TEST_F(SingleHeadStackTest, ToStringEmpty) {
    ASSERT_EQ("", to_string(_stack));
}

TEST_F(SingleHeadStackTest, ToStringOne) {
    auto &headFill = _stack.push(1);
    ASSERT_EQ("1", to_string(headFill));
    ASSERT_EQ("", to_string(_stack));
}

TEST_F(SingleHeadStackTest, ToStringIntermediate) {
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

TEST_F(SingleHeadStackTest, ToStringFull) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    ASSERT_EQ("1 2 3 4 5", to_string(headFill));
    ASSERT_EQ("", to_string(_stack));
}

TEST_F(SingleHeadStackTest, ToStringFillPush) {
    auto &headFill = _stack.fill(1, 2, 3, 4, 5);
    decltype(_stack) stack2;
    auto &pushFill = stack2.push(1).push(2).push(3).push(4).push(5);
    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(0, stack2.size());

    ASSERT_EQ("1 2 3 4 5", to_string(headFill));
    ASSERT_EQ("1 2 3 4 5", to_string(pushFill));
}
