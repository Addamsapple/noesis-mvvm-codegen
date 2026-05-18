#include <mvvm/Resolve.h>

#include <charconv>
#include <system_error>

namespace mvvm {

namespace {

size_t NextDelimiter(std::string_view path) {
    for (size_t index = 1; index < path.size(); ++index)
        if (path[index] == '.' || path[index] == ':')
            return index;
    return path.size();
}

}

ResolveResult Resolve(Model * pModel, std::string_view path) {
    if (!pModel)
        return {{ ResolveError::Type::NullObject, path }};
    if (path.empty())
        return {{ ResolveError::Type::EmptyPath, path }};
    if (path[0] != '.')
        return {{ ResolveError::Type::MissingPropertyDelimiter, path }};

    auto splitIndex = NextDelimiter(path);

    auto pProperty = pModel->FindProperty(path.substr(1, splitIndex - 1));
    if (!pProperty)
        return {{ ResolveError::Type::PropertyNotFound, path }};

    if (splitIndex == path.size())
        return {{
            ResolvedValue::Type::Property,
            pModel,
            pProperty,
            nullptr,
            0
        }};

    auto nextPath = path.substr(splitIndex);

    switch(pProperty->Type()) {
        case ValueType::Model: {
            SharedPtr<Model> pResolvedModel;
            pProperty->Get(*pModel, &pResolvedModel);
            return Resolve(pResolvedModel.Get(), nextPath);
        }
        case ValueType::Collection: {
            SharedPtr<BaseModelCollection> pResolvedCollection;
            pProperty->Get(*pModel, &pResolvedCollection);
            return Resolve(pResolvedCollection.Get(), nextPath);
        }
        default:
            return {{ ResolveError::Type::UnexpectedPrimitive, nextPath }};
    }
}

ResolveResult Resolve(BaseModelCollection * pCollection, std::string_view path) {
    if (!pCollection)
        return {{ ResolveError::Type::NullObject, path }};
    if (path.empty())
        return {{ ResolveError::Type::EmptyPath, path }};
    if (path[0] != ':')
        return {{ ResolveError::Type::MissingItemDelimiter, path }};

    auto splitIndex = NextDelimiter(path);

    uint32_t itemIndex;
    auto conversion = std::from_chars(
        path.data() + 1,
        path.data() + splitIndex,
        itemIndex
    );

    if (conversion.ptr != path.data() + splitIndex
        || conversion.ec == std::errc::result_out_of_range)
        return {{ ResolveError::Type::InvalidIndex, path }};
    if (itemIndex >= pCollection->Size())
        return {{ ResolveError::Type::ItemNotFound, path }};

    if (splitIndex == path.size())
        return {{
            ResolvedValue::Type::Item,
            nullptr,
            nullptr,
            pCollection,
            itemIndex
        }};

    auto nextPath = path.substr(splitIndex);

    switch(pCollection->Type()) {
        case ValueType::Model: {
            auto pModel = static_cast<BaseModelPtrCollection *>(pCollection)
                ->GetModel(itemIndex);
            return Resolve(pModel.Get(), nextPath);
        }
        case ValueType::Collection:
            return {{ ResolveError::Type::UnexpectedCollection, nextPath }};
        default:
            return {{ ResolveError::Type::UnexpectedPrimitive, nextPath }};
    }
}

}
