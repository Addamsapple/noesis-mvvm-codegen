#pragma once

#include <type_traits>

namespace mvvm {
    template<typename T>
    struct AlwaysFalse : std::false_type {};
}

