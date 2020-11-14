#ifndef PREDICATE_H
#define PREDICATE_H

#include <functional>
#include <string>

/**
 * A class which wraps a std::function which always returns bool
 * Note for T, any parameter passed into this functor will be const
 * 
 * Provides a layer of abstraction
 */
template <typename T>
class Predicate {
    private:
        std::function<bool(const T &)> func;

    public:
        Predicate(const std::function<bool(const T &)> &predicate) {
            this->func = predicate;
        }

        operator std::function<bool(const T &)>() const {
            return func;
        }

        bool operator()(const T &t) const {
            return this->func(t);
        }

        Predicate<T> &operator==(const std::function<bool(const T &)> &predicate) {
            this->func = predicate;
            return *this;
        }
};

#endif