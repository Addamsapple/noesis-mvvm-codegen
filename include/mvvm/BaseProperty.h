#pragma once

namespace mvvm {

enum class ValueType;

class Model;

class BaseProperty {
public:

    BaseProperty() = default;
    BaseProperty(const BaseProperty & value) = delete;
    BaseProperty & operator=(const BaseProperty & value) = delete;

    bool operator==(const BaseProperty & other) const;

    virtual void Get(const Model & model, void * pValue) const = 0;
    virtual void Set(Model & model, const void * pValue) const = 0;

    virtual ValueType Type() const;
};

}
