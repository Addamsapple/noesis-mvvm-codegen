#pragma once

#include <cstdint>
#include <string>

#include <mvvm/SharedPtr.h>
#include <mvvm/ValueTraits.h>
#include <mvvm/ValueType.h>

namespace mvvm {

class BaseModelCollection;
class Model;

class Value {
public:
    Value(int8_t value);
    Value(int16_t value);
    Value(int32_t value);
    Value(int64_t value);

    Value(uint8_t value);
    Value(uint16_t value);
    Value(uint32_t value);
    Value(uint64_t value);

    Value(float value);
    Value(double value);

    Value(std::string value);
    Value(SharedPtr<Model> value);
    Value(SharedPtr<BaseModelCollection> value);

    Value(const Value & value);
    Value(Value && value);

    Value & operator=(const Value & value);
    Value & operator=(Value && value);

    ValueType Type() const;

    ~Value();

    template<typename T>
    typename ValueHandle<T>::Type As() const;

private:
    void _Copy(const Value & value);
    void _Move(Value && value);
    void _Clear();

    ValueType _type;

    union {
        union {
            int8_t i8;
            int16_t i16;
            int32_t i32;
            int64_t i64;

            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64;

            float f32;
            double f64;
        } _primitive;

        std::string _string;
        SharedPtr<Model> _pModel;
        SharedPtr<BaseModelCollection> _pCollection;
    };
};

}
