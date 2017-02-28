#ifndef _MULTIHEADSTACK_H_
#define _MULTIHEADSTACK_H_

#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <set>

namespace MultiHeadStackNS {

    template <typename T>
    struct StackNode;

    template <typename T>
    class MultiHeadStack : public StackNode<T> {
    public:
        MultiHeadStack()
                : StackNode<T>(*this, nullptr, T{}) {}

        auto const &addNode(std::shared_ptr<StackNode<T>> &&node) const {
            std::lock_guard<std::mutex> lock(_mutex);
            auto it = _nodes.insert(_nodes.end(), std::move(node));
            (**it)._it = it;

            return **it;
        }

        void removeNode(typename std::list<std::shared_ptr<StackNode<T>>>::iterator &it) const {
            std::lock_guard<std::mutex> lock(_mutex);
            _nodes.erase(it);
        }

        mutable std::mutex _mutex;
        mutable std::list<std::shared_ptr<StackNode<T>>> _nodes;
    };

    template <typename T>
    struct StackNode : std::enable_shared_from_this<StackNode<T>> {
        StackNode(MultiHeadStack<T> const &stack, StackNode<T> *parent, T value)
                : _value(value)
                , _stack(stack)
                , _parent(parent ? parent->shared_from_this() : nullptr) {}

        StackNode(StackNode<T> const &) = delete;

        // No need for virtual here
        ~StackNode() {}

        std::size_t size() const {
            return this != &_stack ? (_parent ? 1 + _parent->size() : 1) : 0;
        }

        friend bool operator==(StackNode<T> const &s1, StackNode<T> const &s2) {
            return s1.size() == s2.size() && s1.value() == s2.value() &&
                   (!s1.parent() || *s1.parent() == *s2.parent());
        }

        StackNode<T> const &push(T value) const {
            return _stack.addNode(std::make_shared<StackNode<T>>(
            _stack, const_cast<StackNode<T> *>(this == &_stack ? nullptr : this), value));
        }

        StackNode<T> const *pop() const {
            auto parent = this->parent();
            if(_it->use_count() == 1) {
                // This will delete the current instance, be careful
                _stack.removeNode(_it);
            }

            return parent;
        }

        StackNode<T> const &fill() const {
            return *this;
        }

        template <typename... Args>
        StackNode<T> const &fill(T val, Args... args) const {
            auto const &n = this->push(val);
            return n.fill(args...);
        }

        friend std::ostream &operator<<(std::ostream &out, StackNode<T> const &node) {
            if(node.parent()) {
                out << *node.parent();
            }
            out << node.value() << ' ';

            return out;
        }

        StackNode<T> const *parent() const {
            return _parent.get();
        }

        T value() const {
            return _value;
        }

        T _value;
        MultiHeadStack<T> const &_stack;
        mutable typename std::list<std::shared_ptr<StackNode<T>>>::iterator _it;
        std::shared_ptr<StackNode> _parent;
    };
}

#endif /* end of include guard: _MULTIHEADSTACK_H_ */
