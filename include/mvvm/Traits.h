#pragma once

namespace mvvm {

template<typename T>
struct AlwaysFalse {
    static constexpr bool Value = false;
};

template<typename T>
inline constexpr bool AlwaysFalseV = AlwaysFalse<T>::Value;

}

