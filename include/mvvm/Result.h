#pragma once

#include <type_traits>

namespace mvvm {

enum class ResultType {
    Value,
    Error
};

template<typename T, typename E>
class Result {
public:
    // ambiguity when T == E
    Result(T value) : _type(ResultType::Value) { new(&_value) T(std::move(value)); }
    Result(E value) : _type(ResultType::Error) { new(&_error) E(std::move(value)); }

    Result(const Result & value) : _type(value.Type()) { _Copy(value); }

    Result(Result && value) : _type(value.Type()) { _Move(std::move(value)); }

    static_assert(std::is_nothrow_move_constructible_v<T>,
        "noexcept move constructor required for strong exception guarantee");
    static_assert(std::is_nothrow_move_constructible_v<E>,
        "noexcept move constructor required from strong exception guarantee");

    Result & operator=(const Result & value) {
        if (&value != this) {
            Result temp(value);

            _Clear();
            _type = temp.Type();
            _Move(std::move(temp));
        }
        return *this;
    }

    Result & operator=(Result && value) {
        if (&value != this) {
            _Clear();
            _type = value.Type();
            _Move(std::move(value));
        }
        return *this;
    }

    bool HasValue() const { return _type == ResultType::Value; }

    explicit operator bool() const { return HasValue(); }

    T & Value() { return _value; }
    E & Error() { return _error; }

    const T & Value() const { return _value; }
    const E & Error() const { return _error; }

    ResultType Type() const { return _type; }

    ~Result() { _Clear(); }
private:
    void _Copy(const Result & value) {
        switch(value.Type()) {
            case ResultType::Value: new(&_value) T(value.Value()); break;
            case ResultType::Error: new(&_error) E(value.Error()); break;
        }
    }

    void _Move(Result && value) {
        switch(value.Type()) {
            case ResultType::Value: new(&_value) T(std::move(value.Value())); break;
            case ResultType::Error: new(&_error) E(std::move(value.Error())); break;
        }
    }

    void _Clear() {
        switch(_type) {
            case ResultType::Value: Value().~T(); break;
            case ResultType::Error: Error().~E(); break;
        }
    }

    ResultType _type;

    union {
        T _value;
        E _error;
    };
};

}
