#include <mvvm/BaseProperty.h>

#include <mvvm/ValueType.h>

namespace mvvm {

bool BaseProperty::operator==(const BaseProperty & other) const {
    return this == &other;
}

ValueType BaseProperty::Type() const { return ValueType::Unknown; }

}
