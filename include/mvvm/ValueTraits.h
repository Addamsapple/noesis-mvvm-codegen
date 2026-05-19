#pragma once

#include <string>

namespace mvvm {

template<typename T>
class SharedPtr;

template<typename T>
struct ValueHandle {
    using Type = T;
};

template<>
struct ValueHandle<std::string> {
    using Type = const std::string &;
};

template<typename T>
struct ValueHandle<SharedPtr<T>> {
    using Type = const SharedPtr<T> &;
};

template<typename T>
using ValueHandleT = typename ValueHandle<T>::Type;

}
