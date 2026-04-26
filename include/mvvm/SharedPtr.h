# pragma once

#include <cstddef>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

namespace mvvm {

namespace detail {

struct ControlBlock {
    int strongCount;
    int weakCount;

    void * const pManagedValue;
    void (* const pDeleter)(void *);
};

// for tag dispatch
struct UseOverload {};

// SFINAE 

template<typename T, typename U>
using EnableIfConvertibleT = std::enable_if_t<std::is_convertible_v<T *, U *>, int>;

template<typename T, typename U>
using EnableIfCompatibleT = std::enable_if_t<std::is_convertible_v<T *, U *> || std::is_convertible_v<U *, T *>, int>;

}

template<typename T>
class SharedPtr {
public:
    using Type = T;

    SharedPtr() : SharedPtr(nullptr) {}

    SharedPtr(std::nullptr_t) : SharedPtr(nullptr, nullptr) {}

    SharedPtr(const SharedPtr & value) : SharedPtr(value, detail::UseOverload{}) {}

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    SharedPtr(const SharedPtr<U> & value, detail::UseOverload = {}) {
        if (value._pControl) {
            _pValue = value._pValue;
            _pControl = value._pControl;
            ++_pControl->strongCount;
        } else {
            _pValue = nullptr;
            _pControl = nullptr;
        }
    }

    SharedPtr(SharedPtr && value) : SharedPtr(std::move(value), detail::UseOverload{}) {}

    // aliasing constructor
    template<typename U>
    SharedPtr(T & value, const SharedPtr<U> & managedValue) {
        if (managedValue._pControl) {
            _pValue = &value;
            _pControl = managedValue._pControl;
            ++_pControl->strongCount;
        } else {
            _pValue = nullptr;
            _pControl = nullptr;
        }
    }

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    SharedPtr(SharedPtr<U> && value, detail::UseOverload = {}) : SharedPtr(value._pValue, value._pControl) {
        value._pValue = nullptr;
        value._pControl = nullptr;
    }

    ~SharedPtr() { _Clear(); }

    // does not support alignas(...) T
    template<typename... Ts>
    static SharedPtr Make(Ts && ... args) {
        // round up sizeof(ControlBlock) to next multiple of alignof(T) to get value offset
        constexpr std::size_t valueOffset = (sizeof(detail::ControlBlock) + alignof(T) - 1) & ~(alignof(T) - 1);
        constexpr std::size_t bufferSize = valueOffset + sizeof(T);

        auto buffer = (char *) ::operator new(bufferSize);

        T * pValue;
        try {
            pValue = new(buffer + valueOffset) T(std::forward<Ts>(args)...);
        } catch (...) {
            ::operator delete(buffer);
            throw;
        }

        auto pControl = new(buffer) detail::ControlBlock{
            1, 0, pValue, [](void * pValue) { static_cast<T *>(pValue)->~T(); }
        };

        return SharedPtr(pValue, pControl);
    }

    template<typename U>
    SharedPtr<U> StaticCast() const {
        if (_pControl)
            ++_pControl->strongCount;
        return SharedPtr<U>(static_cast<U *>(_pValue), _pControl);
    }

    template<typename U>
    SharedPtr<U> DynamicCast() const {
        if (auto pValue = dynamic_cast<U*>(_pValue)) {
            ++_pControl->strongCount;
            return SharedPtr<U>(pValue, _pControl);
        }
        return nullptr;
    }

    SharedPtr & operator=(const SharedPtr & value) { return operator=<T>(value); }

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    SharedPtr & operator=(const SharedPtr<U> & value) {
        if (value._pControl) {
            // increment first to support self copy assignment
            ++value._pControl->strongCount;
            _Clear();
            _pValue = value._pValue;
            _pControl = value._pControl;
        } else
            _Clear();

        return *this;
    }

    SharedPtr & operator=(SharedPtr && value) { return operator=<T>(value); }

    // does not support self move assignment
    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    SharedPtr & operator=(SharedPtr<U> && value) {
        _Clear();

        _pValue = value._pValue;
        _pControl = value._pControl;

        value._pValue = nullptr;
        value._pControl = nullptr;

        return *this;
    }

    explicit operator bool() const { return _pControl != nullptr; }

    T & operator*() const { return *_pValue; }
    T * operator->() const { return _pValue; }
    T * Get() const { return _pValue; }

    void Reset() {
        _Clear();

        _pValue = nullptr;
        _pControl = nullptr;
    }

    size_t Hash() const { return std::hash<detail::ControlBlock *>{}(_pControl); }

private:
    SharedPtr(T * pValue, detail::ControlBlock * pControl) : _pValue(pValue), _pControl(pControl) {}

    void _Clear() {
        if (!_pControl)
            return;

        if (_pControl->strongCount != 1) {
            --_pControl->strongCount;
            return;
        }

        _pControl->pDeleter(_pControl->pManagedValue);
        if (_pControl->weakCount == 0)
            ::operator delete((void *) _pControl);
        else
            --_pControl->strongCount;
    }

    T * _pValue;
    detail::ControlBlock * _pControl;

    template<typename U>
    friend class SharedPtr;

    template<typename U>
    friend class WeakPtr;
};

template<typename T>
class WeakPtr {
public:
    WeakPtr() : WeakPtr(nullptr) {}

    WeakPtr(std::nullptr_t) : WeakPtr(nullptr, nullptr) {}

    WeakPtr(const WeakPtr & value) : WeakPtr(value, detail::UseOverload{}) {}

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    WeakPtr(const WeakPtr<U> & value, detail::UseOverload = {}) {
        if (value._pControl) {
            _pValue = value._pValue;
            _pControl = value._pControl;
            ++_pControl->weakCount;
        } else {
            _pValue = nullptr;
            _pControl = nullptr;
        }
    }

    WeakPtr(WeakPtr && value) : WeakPtr(std::move(value), detail::UseOverload{}) {}

    // aliasing constructor
    template<typename U>
    WeakPtr(T & value, const WeakPtr<U> & managedValue) {
        if (managedValue._pControl) {
            _pValue = &value;
            _pControl = managedValue._pControl;
            ++_pControl->weakCount;
        } else {
            _pValue = nullptr;
            _pControl = nullptr;
        }
    }

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    WeakPtr(WeakPtr<U> && value, detail::UseOverload = {}) : WeakPtr(value._pValue, value._pControl) {
        value._pValue = nullptr;
        value._pControl = nullptr;
    }

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    WeakPtr(const SharedPtr<U> & value) {
        if (value._pControl) {
            _pValue = value._pValue;
            _pControl = value._pControl;
            ++_pControl->weakCount;
        } else {
            _pValue = nullptr;
            _pControl = nullptr;
        }
    }

    ~WeakPtr() { _Clear(); }

    template<typename U>
    WeakPtr<U> StaticCast() const {
        if (_pControl)
            ++_pControl->weakCount;
        return WeakPtr<U>(static_cast<U *>(_pValue), _pControl);
    }

    template<typename U>
    WeakPtr<U> DynamicCast() const {
        if (auto pValue = dynamic_cast<U*>(_pValue)) {
            ++_pControl->weakCount;
            return WeakPtr<U>(pValue, _pControl);
        }
        return nullptr;
    }

    WeakPtr & operator=(const WeakPtr & value) { return operator=<T>(value); }

    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    WeakPtr & operator=(const WeakPtr<U> & value) {
        if (value._pControl) {
            // increment first to support self copy assignment
            ++value._pControl->weakCount;
            _Clear();
            _pValue = value._pValue;
            _pControl = value._pControl;
        } else
            _Clear();

        return *this;
    }

    WeakPtr & operator=(WeakPtr && value) { return operator=<T>(std::move(value)); }

    // does not support self move assignment
    template<typename U, detail::EnableIfConvertibleT<U, T> = 0>
    WeakPtr & operator=(WeakPtr<U> && value) {
        _Clear();

        _pValue = value._pValue;
        _pControl = value._pControl;

        value._pValue = nullptr;
        value._pControl = nullptr;

        return *this;
    }

    explicit operator bool() const { return _pControl != nullptr; }

    T & operator*() const { return *_pValue; }
    T * operator->() const { return _pValue; }
    T * Get() const { return _pValue; }

    bool Expired() const { return !_pControl || _pControl->strongCount == 0; }

    template<typename U = T, detail::EnableIfConvertibleT<T, U> = 0>
    SharedPtr<U> Lock() const {
        if (Expired())
            return SharedPtr<U>();

        ++_pControl->strongCount;
        return SharedPtr<U>(_pValue, _pControl);
    }

    void Reset() {
        _Clear();

        _pValue = nullptr;
        _pControl = nullptr;
    }

    size_t Hash() const { return std::hash<detail::ControlBlock *>{}(_pControl); }

private:
    WeakPtr(T * pValue, detail::ControlBlock * pControl) : _pValue(pValue), _pControl(pControl) {}

    void _Clear() {
        if (!_pControl)
            return;

        if (_pControl->weakCount != 1) {
            --_pControl->weakCount;
            return;
        }

        if (_pControl->strongCount == 0)
            ::operator delete((void *) _pControl);
        else
            --_pControl->weakCount;
    }

    T * _pValue;
    detail::ControlBlock * _pControl;

    template<typename U>
    friend class WeakPtr;
};

// comparison operators

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator==(const SharedPtr<T> & a, const SharedPtr<U> & b) {
    return a.Get() == b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator!=(const SharedPtr<T> & a, const SharedPtr<U> & b) {
    return a.Get() != b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator<(const SharedPtr<T> & a, const SharedPtr<U> & b) {
    return a.Get() < b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator>(const SharedPtr<T> & a, const SharedPtr<U> & b) {
    return a.Get() > b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator==(const WeakPtr<T> & a, const WeakPtr<U> & b) {
    return a.Get() == b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator!=(const WeakPtr<T> & a, const WeakPtr<U> & b) {
    return a.Get() != b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator<(const WeakPtr<T> & a, const WeakPtr<U> & b) {
    return a.Get() < b.Get();
}

template<typename T, typename U, detail::EnableIfCompatibleT<T, U> = 0>
bool operator>(const WeakPtr<T> & a, const WeakPtr<U> & b) {
    return a.Get() > b.Get();
}

}

// hash functions

namespace std {

template<typename T>
struct hash<mvvm::SharedPtr<T>> {
    size_t operator()(const mvvm::SharedPtr<T> & value) const {
        return value.Hash();
    }
};

template<typename T>
struct hash<mvvm::WeakPtr<T>> {
    size_t operator()(const mvvm::WeakPtr<T> & value) const {
        return value.Hash();
    }
};

}
