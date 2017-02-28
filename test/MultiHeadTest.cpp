#include "../MultiHeadStack.h"
#include "gtest/gtest.h"
#include <set>

using namespace MultiHeadStackNS;

struct MockInt {
    MockInt()
            : MockInt(-999999) {}
    MockInt(int value)
            : _value(value) {}
    MockInt(MockInt &&other)
            : _value(other._value) {
        other._value = -999999;
    }
    ~MockInt() {
        _destroyed.insert(_value);
    }

    int _value;

    static std::set<int> _destroyed;
};

std::set<int> MockInt::_destroyed;

class MultiHeadStackTest : public testing::Test {
protected:
    virtual void SetUp() override {
        MockInt::_destroyed.clear();
    }

    MultiHeadStack<int> _stack;
    MultiHeadStack<MockInt> _stackMock;
};

TEST_F(MultiHeadStackTest, TwoRoots1) {
    auto &h1 = _stack.push(1);
    auto &h2 = _stack.push(2);

    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(1, h1.size());
    ASSERT_EQ(1, h2.size());

    ASSERT_EQ(&_stack, h1.parent());
    ASSERT_EQ(&_stack, h2.parent());
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

    ASSERT_EQ(&_stack, h1.parent());
    ASSERT_EQ(&_stack, h2.parent());
    ASSERT_EQ(&h1, h11.parent());
    ASSERT_EQ(&h2, h21.parent());
}

TEST_F(MultiHeadStackTest, SplitAtFirst) {
    auto &h1 = _stack.push(1);
    auto &h2 = h1.push(2);
    auto &h3 = h1.push(3);

    ASSERT_EQ(0, _stack.size());
    ASSERT_EQ(1, h1.size());
    ASSERT_EQ(2, h2.size());
    ASSERT_EQ(2, h3.size());

    ASSERT_EQ(&_stack, h1.parent());
    ASSERT_EQ(&h1, h2.parent());
    ASSERT_EQ(&h1, h3.parent());
}

TEST_F(MultiHeadStackTest, SplitAtFirstThenPopOneBranchThenTheOther) {
    auto &h1 = _stackMock.push(1);
    auto &h2 = h1.push(2);
    (void)h2;
    auto &h3 = h1.push(3);

    auto h4 = h3.pop();

    ASSERT_EQ(h4, &h1);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(0, MockInt::_destroyed.count(2));
    ASSERT_EQ(1, MockInt::_destroyed.count(3));

    auto h5 = h4->pop();

    ASSERT_EQ(h5, &_stackMock);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(0, MockInt::_destroyed.count(2));
    ASSERT_EQ(1, MockInt::_destroyed.count(3));

    auto h6 = h2.pop();

    ASSERT_EQ(h6, &h1);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(1, MockInt::_destroyed.count(3));

    auto h7 = h6->pop();

    ASSERT_EQ(h7, &_stackMock);

    ASSERT_EQ(1, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(1, MockInt::_destroyed.count(3));
}

TEST_F(MultiHeadStackTest, SplitAtFirstThenPopOneBranchFromMiddleThenTheOther) {
    auto &h1 = _stackMock.push(1);
    auto &h2 = h1.push(2);
    (void)h2;
    auto &h3 = h1.push(3);
    auto &h31 = h3.push(4);

    auto h4 = h3.pop();

    ASSERT_EQ(h4, &h1);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(0, MockInt::_destroyed.count(2));
    ASSERT_EQ(0, MockInt::_destroyed.count(3));
    ASSERT_EQ(0, MockInt::_destroyed.count(4));

    auto h5 = h4->pop();

    ASSERT_EQ(h5, &_stackMock);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(0, MockInt::_destroyed.count(2));
    ASSERT_EQ(0, MockInt::_destroyed.count(3));
    ASSERT_EQ(0, MockInt::_destroyed.count(4));

    auto h6 = h2.pop();

    ASSERT_EQ(h6, &h1);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(0, MockInt::_destroyed.count(3));
    ASSERT_EQ(0, MockInt::_destroyed.count(4));

    auto h7 = h6->pop();

    ASSERT_EQ(h7, &_stackMock);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(0, MockInt::_destroyed.count(3));
    ASSERT_EQ(0, MockInt::_destroyed.count(4));

    auto h8 = h31.pop();

    ASSERT_EQ(h8, &h3);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(0, MockInt::_destroyed.count(3));
    ASSERT_EQ(1, MockInt::_destroyed.count(4));

    auto h9 = h8->pop();

    ASSERT_EQ(h9, &h1);

    ASSERT_EQ(0, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(1, MockInt::_destroyed.count(3));
    ASSERT_EQ(1, MockInt::_destroyed.count(4));

    auto h10 = h9->pop();

    ASSERT_EQ(h10, &_stackMock);

    ASSERT_EQ(1, MockInt::_destroyed.count(1));
    ASSERT_EQ(1, MockInt::_destroyed.count(2));
    ASSERT_EQ(1, MockInt::_destroyed.count(3));
    ASSERT_EQ(1, MockInt::_destroyed.count(4));
}
