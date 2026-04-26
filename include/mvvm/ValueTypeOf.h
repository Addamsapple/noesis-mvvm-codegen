#pragma once

#include <cstdint>
#include <string>

#include <mvvm/ValueType.h>

namespace mvvm {

class Model;

template<typename T>
class ModelCollection;

template<typename T>
class SharedPtr;

// default

template<typename T, typename = void> struct ValueTypeOf {
    static constexpr auto Value = ValueType::Unknown;
};

// signed

template<> struct ValueTypeOf<int8_t> {
    static constexpr auto Value = ValueType::Int8;
};
template<> struct ValueTypeOf<int16_t> {
    static constexpr auto Value = ValueType::Int16;
};
template<> struct ValueTypeOf<int32_t> {
    static constexpr auto Value = ValueType::Int32;
};
template<> struct ValueTypeOf<int64_t> {
    static constexpr auto Value = ValueType::Int64;
};

// unsigned

template<> struct ValueTypeOf<uint8_t> {
    static constexpr auto Value = ValueType::UInt8;
};
template<> struct ValueTypeOf<uint16_t> {
    static constexpr auto Value = ValueType::UInt16;
};
template<> struct ValueTypeOf<uint32_t> {
    static constexpr auto Value = ValueType::UInt32;
};
template<> struct ValueTypeOf<uint64_t> {
    static constexpr auto Value = ValueType::UInt64;
};

// floating-point

template<> struct ValueTypeOf<float> {
    static constexpr auto Value = ValueType::Float;
};
template<> struct ValueTypeOf<double> {
    static constexpr auto Value = ValueType::Double;
};

// class

template<> struct ValueTypeOf<std::string> {
    static constexpr auto Value = ValueType::String;
};
template<typename T>
struct ValueTypeOf<SharedPtr<T>, std::enable_if_t<std::is_base_of_v<Model, T>>> {
    static constexpr auto Value = ValueType::Model;
};
template<typename T>
struct ValueTypeOf<SharedPtr<ModelCollection<T>>> {
    static constexpr auto Value = ValueType::Collection;
};

template<typename T>
inline constexpr auto ValueTypeOfV = ValueTypeOf<T>::Value;

}
