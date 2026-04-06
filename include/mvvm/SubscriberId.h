#pragma once

#include <mvvm/Key.h>

namespace mvvm {

template<typename T>
class [[nodiscard]] SubscriberId {
public:
    SubscriberId() = default;

    SubscriberId Next(Key<T>) const { return SubscriberId(_id + 1); }

    explicit operator bool() const { return _id != 0; }

    bool operator==(const SubscriberId & value) const { return _id == value._id; }

private:
    SubscriberId(size_t id) : _id(id) {}

    size_t _id = 0;
};

}
