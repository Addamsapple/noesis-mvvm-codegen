#pragma once

namespace mvvm {

template<typename T>
class Key {
private:
    Key() {}
    Key(const Key &) {}

    friend T;
};

}
