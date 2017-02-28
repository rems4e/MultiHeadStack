#ifndef _MULTIHEADSTACK_H_
#define _MULTIHEADSTACK_H_

#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>

namespace MultiHeadStackNS {

    template <typename T>
    class StackNode;

    template <typename T>
    class MultiHeadStack : public StackNode<T> {
        friend class StackNode<T>;
        static_assert(std::is_default_constructible<T>::value, "T must be default-constructible.");
        static_assert(std::is_copy_constructible<T>::value || std::is_move_constructible<T>::value,
                      "T must be copy- or move-constructible.");

    public:
        MultiHeadStack()
                : StackNode<T>(*this, nullptr, T{})
                , _mutex(std::make_unique<std::mutex>()) {}

        MultiHeadStack(MultiHeadStack &&stack)
                : StackNode<T>(std::move(stack))
                , _mutex(std::move(stack._mutex))
                , _nodes(std::move(stack._nodes)) {}

    private:
        auto const &addNode(std::shared_ptr<StackNode<T>> &&node) const {
            std::lock_guard<std::mutex> lock(*_mutex);
            auto it = _nodes.insert(_nodes.end(), std::move(node));
            (**it)._it = it;

            return **it;
        }

        void removeNode(typename std::list<std::shared_ptr<StackNode<T>>>::iterator &it) const {
            std::lock_guard<std::mutex> lock(*_mutex);
            _nodes.erase(it);
        }

        mutable std::unique_ptr<std::mutex> _mutex;
        mutable std::list<std::shared_ptr<StackNode<T>>> _nodes;
    };

    template <typename T>
    class StackNode : public std::enable_shared_from_this<StackNode<T>> {
        friend class MultiHeadStack<T>;

    public:
        virtual ~StackNode() {}

        StackNode(StackNode<T> const &) = delete;

        std::size_t size() const {
            return this != &_stack ? (_parent ? 1 + _parent->size() : 1) : 0;
        }

        friend bool operator==(StackNode<T> const &s1, StackNode<T> const &s2) {
            return s1.size() == s2.size() && s1.value() == s2.value() &&
                   (!s1.parent() || *s1.parent() == *s2.parent());
        }

        friend bool operator!=(StackNode<T> const &s1, StackNode<T> const &s2) {
            return !(s1 == s2);
        }

        StackNode<T> const &push(T value) const {
            return _stack.addNode(
            make_shared(_stack, const_cast<StackNode<T> *>(this == &_stack ? nullptr : this), std::move(value)));
        }

        StackNode<T> const *pop() const {
            auto parent = this->parent();
            if(_it->use_count() == 1) {
                // This will delete the current instance, be careful as references will be
                // invalidated
                _stack.removeNode(_it);
            }

            return parent;
        }

        StackNode<T> const &fill() const {
            return *this;
        }

        template <typename... Args>
        StackNode<T> const &fill(T val, Args... args) const {
            auto const &n = this->push(std::move(val));
            return n.fill(args...);
        }

        friend std::ostream &operator<<(std::ostream &out, StackNode<T> const &node) {
            if(&node != &node._stack) {
                node.put(out);
            }
            return out;
        }

        friend std::string to_string(StackNode<T> const &node) {
            std::ostringstream oss;
            oss << node;
            return oss.str();
        }

        StackNode<T> const *parent() const {
            auto ptr = _parent.get();
            return ptr ? ptr : (this == &_stack) ? nullptr : &_stack;
        }

        T value() const {
            return _value;
        }

    protected:
        StackNode(StackNode<T> &&node)
                : _value(std::move(node._value))
                , _stack(node._stack)
                , _it(std::move(node._it))
                , _parent(std::move(node._parent)) {}

    private:
        template <typename... Args>
        static std::shared_ptr<StackNode<T>> make_shared(Args &&... args) {
            struct make_shared_enabler : StackNode<T> {
            public:
                make_shared_enabler(Args &&... args)
                        : StackNode(std::forward<Args>(args)...) {}
            };
            return std::make_shared<make_shared_enabler>(std::forward<Args>(args)...);
        }

        StackNode(MultiHeadStack<T> const &stack, StackNode<T> *parent, T value)
                : _value(std::move(value))
                , _stack(stack)
                , _parent(parent ? parent->shared_from_this() : nullptr) {}

        void put(std::ostream &out, bool separator = false) const {
            if(_parent) {
                _parent->put(out, true);
            }

            out << value();
            if(separator) {
                out << ' ';
            }
        }

        T _value;
        MultiHeadStack<T> const &_stack;
        mutable typename std::list<std::shared_ptr<StackNode<T>>>::iterator _it;
        std::shared_ptr<StackNode> _parent;
    };
}

#endif /* end of include guard: _MULTIHEADSTACK_H_ */
