#pragma once

namespace mvvm {

enum class ValueType {
    Unknown,
    Int8, Int16, Int32, Int64,
    UInt8, UInt16, UInt32, UInt64,
    Float, Double,
    String,
    Model, Collection,
};

}
