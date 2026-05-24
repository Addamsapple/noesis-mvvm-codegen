#pragma once

#include <cstdint>
#include <string_view>

#include <mvvm/Result.h>
#include <mvvm/BaseModelPtrCollection.h>
#include <mvvm/BaseProperty.h>
#include <mvvm/Model.h>
#include <mvvm/ValueType.h>

namespace mvvm {

class ResolvedValue {
public:
    ResolvedValue(Model * pModel, const BaseProperty * pProperty);
    ResolvedValue(BaseModelCollection * pCollection, uint32_t index);

    Value Get() const;
    void Set(const Value & value);

    ValueType Type() const;

private:
    enum class Source {
        Property,
        Item
    } _source;

    Model * _pModel;
    const BaseProperty * _pProperty;

    BaseModelCollection * _pCollection;
    uint32_t _index;
};

struct ResolveError {
    enum class Type {
        NullObject,
        EmptyPath,
        MissingPropertyDelimiter,
        MissingItemDelimiter,
        PropertyNotFound,
        InvalidIndex,
        ItemNotFound,
        UnexpectedCollection,
        UnexpectedPrimitive,
    } type;

    std::string_view path;
};

using ResolveResult = Result<ResolvedValue, ResolveError>;

ResolveResult Resolve(Model * pModel, std::string_view path);
ResolveResult Resolve(BaseModelCollection * pCollection, std::string_view path);

}
