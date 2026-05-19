#include <mvvm/Value.h>

#include <cassert>

namespace mvvm {

// C++20 needed for designated intializers

Value::Value(int8_t value) : _type(ValueType::Int8) {
    _primitive.i8 = value;
}
Value::Value(int16_t value) : _type(ValueType::Int16) {
    _primitive.i16 = value;
}
Value::Value(int32_t value) : _type(ValueType::Int32) {
    _primitive.i32 = value;
}
Value::Value(int64_t value) : _type(ValueType::Int64) {
    _primitive.i64 = value;
}

Value::Value(uint8_t value) : _type(ValueType::UInt8) {
    _primitive.u8 = value;
}
Value::Value(uint16_t value) : _type(ValueType::UInt16) {
    _primitive.u16 = value;
}
Value::Value(uint32_t value) : _type(ValueType::UInt32) {
    _primitive.u32 = value;
}
Value::Value(uint64_t value) : _type(ValueType::UInt64) {
    _primitive.u64 = value;
}

Value::Value(float value) : _type(ValueType::Float) {
    _primitive.f32 = value;
}
Value::Value(double value) : _type(ValueType::Double) {
    _primitive.f64 = value;
}

Value::Value(std::string value) :
    _type(ValueType::String),
    _string(std::move(value)) {}
Value::Value(SharedPtr<Model> value) :
    _type(ValueType::Model),
    _pModel(std::move(value)) {}
Value::Value(SharedPtr<BaseModelCollection> value) :
    _type(ValueType::Collection),
    _pCollection(std::move(value)) {}

Value::Value(const Value & value) : _type(value.Type()) { _Copy(value); }
Value::Value(Value && value) : _type(value.Type()) { _Move(std::move(value)); }

Value & Value::operator=(const Value & value) {
    if(&value != this) {
        Value temp(value);

        _Clear();
        _type = value.Type();
        _Move(std::move(temp));
    }
    return *this;
}

Value & Value::operator=(Value && value) {
    if(&value != this) {
        _Clear();
        _type = value.Type();
        _Move(std::move(value));
    }
    return *this;
}

ValueType Value::Type() const { return _type; }

Value::~Value() { _Clear(); }

template<> int8_t Value::As<int8_t>() const {
    assert(Type() == ValueType::Int8);
    return _primitive.i8;
}
template<> int16_t Value::As<int16_t>() const {
    assert(Type() == ValueType::Int16);
    return _primitive.i16;
}
template<> int32_t Value::As<int32_t>() const {
    assert(Type() == ValueType::Int32);
    return _primitive.i32;
}
template<> int64_t Value::As<int64_t>() const {
    assert(Type() == ValueType::Int64);
    return _primitive.i64;
}

template<> uint8_t Value::As<uint8_t>() const {
    assert(Type() == ValueType::UInt8);
    return _primitive.u8;
}
template<> uint16_t Value::As<uint16_t>() const {
    assert(Type() == ValueType::UInt16);
    return _primitive.u16;
}
template<> uint32_t Value::As<uint32_t>() const {
    assert(Type() == ValueType::UInt32);
    return _primitive.u32;
}
template<> uint64_t Value::As<uint64_t>() const {
    assert(Type() == ValueType::UInt64);
    return _primitive.u64;
}

template<> float Value::As<float>() const {
    assert(Type() == ValueType::Float);
    return _primitive.f32;
}
template<> double Value::As<double>() const {
    assert(Type() == ValueType::Double);
    return _primitive.f64;
}

template<> const std::string & Value::As<std::string>() const {
    assert(Type() == ValueType::String);
    return _string;
}
template<> const SharedPtr<Model> & Value::As<SharedPtr<Model>>() const {
    assert(Type() == ValueType::Model);
    return _pModel;
}
template<> const SharedPtr<BaseModelCollection> & Value::As<SharedPtr<BaseModelCollection>>() const {
    assert(Type() == ValueType::Collection);
    return _pCollection;
}

void Value::_Copy(const Value & value) {
    switch(Type()) {
        case ValueType::String:
            new(&_string) std::string(value._string);
            break;
        case ValueType::Model:
            new(&_pModel) SharedPtr<Model>(value._pModel);
            break;
        case ValueType::Collection:
            new(&_pCollection) SharedPtr<BaseModelCollection>(value._pCollection);
            break;
        default: _primitive = value._primitive;
    }
}

void Value::_Move(Value && value) {
    switch(Type()) {
        case ValueType::String:
            new(&_string) std::string(std::move(value._string));
            break;
        case ValueType::Model:
            new(&_pModel) SharedPtr<Model>(std::move(value._pModel));
            break;
        case ValueType::Collection:
            new(&_pCollection) SharedPtr<BaseModelCollection>(std::move(value._pCollection));
            break;
        default: _primitive = value._primitive;
    }
}

void Value::_Clear() {
    switch(Type()) {
        case ValueType::String: _string.~basic_string(); break;
        case ValueType::Model: _pModel.~SharedPtr<Model>(); break;
        case ValueType::Collection: _pCollection.~SharedPtr<BaseModelCollection>(); break;
    }
}

}
