#pragma once

#include <cstdint>
#include <string_view>

#include <mvvm/Result.h>
#include <mvvm/BaseModelPtrCollection.h>
#include <mvvm/BaseProperty.h>
#include <mvvm/Model.h>
#include <mvvm/ValueType.h>

namespace mvvm {

struct DeferredValue {
    Value Evaluate() const;

    enum class Type {
        Property,
        Item
    } type;

    Model * pModel;
    const BaseProperty * pProperty;

    BaseModelCollection * pCollection;
    uint32_t index;
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

using ResolveResult = Result<DeferredValue, ResolveError>;

ResolveResult Resolve(Model * pModel, std::string_view path);
ResolveResult Resolve(BaseModelCollection * pCollection, std::string_view path);

}
