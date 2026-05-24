#include <mvvm/Resolve.h>

#include <charconv>
#include <system_error>

#include <mvvm/Value.h>

namespace mvvm {

namespace {

size_t NextDelimiter(std::string_view path) {
    for (size_t index = 1; index < path.size(); ++index)
        if (path[index] == '.' || path[index] == ':')
            return index;
    return path.size();
}

}

ResolvedValue::ResolvedValue(Model * pModel, const BaseProperty * pProperty) :
    _source(Source::Property),
    _pModel(pModel),
    _pProperty(pProperty) {}

ResolvedValue::ResolvedValue(BaseModelCollection * pCollection, uint32_t index) :
    _source(Source::Item),
    _pCollection(pCollection),
    _index(index) {}

Value ResolvedValue::Get() const {
    switch(_source) {
        case Source::Property: return _pProperty->Get(*_pModel);
        case Source::Item: return _pCollection->GetValue(_index);
    }
}

void ResolvedValue::Set(const Value & value) {
    switch(_source) {
        case Source::Property: _pProperty->Set(*_pModel, value); break;
        case Source::Item: _pCollection->SetValue(_index, value); break;
    }
}

ValueType ResolvedValue::Type() const {
    switch(_source) {
        case Source::Property: return _pProperty->Type();
        case Source::Item: return _pCollection->Type();
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
        return {{ pModel, pProperty }};

    auto nextPath = path.substr(splitIndex);

    switch(pProperty->Type()) {
        case ValueType::Model:
            return Resolve(
                pProperty->Get(*pModel).As<SharedPtr<Model>>().Get(),
                nextPath
            );
        case ValueType::Collection:
            return Resolve(
                pProperty->Get(*pModel).As<SharedPtr<BaseModelCollection>>().Get(),
                nextPath
            );
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
        return {{ pCollection, itemIndex }};

    auto nextPath = path.substr(splitIndex);

    switch(pCollection->Type()) {
        case ValueType::Model:
            return Resolve(
                static_cast<BaseModelPtrCollection *>(pCollection)->GetModel(itemIndex).Get(),
                nextPath
            );
        case ValueType::Collection:
            return {{ ResolveError::Type::UnexpectedCollection, nextPath }};
        default:
            return {{ ResolveError::Type::UnexpectedPrimitive, nextPath }};
    }
}

}
