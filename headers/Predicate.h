#ifndef PREDICATE_H
#define PREDICATE_H

#include <functional>

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
        Predicate(const std::function<bool(const T &)> &predicate);
        operator std::function<bool(const T &)>() const;
        bool operator()(const T &t) const;
        Predicate<T> &operator==(const std::function<bool(const T &)> &predicate);
};

#endif