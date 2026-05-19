#pragma once

namespace mvvm {

enum class ValueType;

class Model;
class Value;

class BaseProperty {
public:
    BaseProperty() = default;
    BaseProperty(const BaseProperty & value) = delete;
    BaseProperty & operator=(const BaseProperty & value) = delete;

    bool operator==(const BaseProperty & other) const;

    virtual Value Get(const Model & model) const = 0;
    virtual void Set(Model & model, const Value & value) const = 0;

    virtual ValueType Type() const;
};

}
