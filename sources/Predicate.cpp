#include "headers/Predicate.h"
#include <string>

template <typename T>
Predicate<T>::Predicate(const std::function<bool(const T &)> &predicate) {
    this->func = predicate;
}

template <typename T>
Predicate<T>::operator std::function<bool(const T &)>() const {
    return func;
}

template <typename T>
bool Predicate<T>::operator()(const T &t) const {
    return this->func(t);
}

template <typename T>
Predicate<T> &Predicate<T>::operator==(const std::function<bool(const T &)> &predicate) {
    this->func = predicate;
    return *this;
}

template class Predicate<int>;
template class Predicate<double>;
template class Predicate<float>;
template class Predicate<bool>;
template class Predicate<std::string>;