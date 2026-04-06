#include <mvvm/ViewModel.h>

#include <cassert>

#include <mvvm/Cache.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/Subscription.h>

namespace mvvm {

// ViewModel

ViewModel::ViewModel(const Key<Factory> &, Context context) : _context(context) {}

Noesis::Ptr<ViewModel> ViewModel::CreateFromModel(const SharedPtr<Model> & pModel, Context context) {
    if (!pModel) return {};

    if (auto pViewModel = context.cache.Find(pModel))
        return Noesis::Ptr(pViewModel);

    const auto function = context.factory.Find(typeid(*pModel));
    assert(function);

    auto result = function(context);
    context.cache.Insert({}, pModel, result.GetPtr());
    result->_Observe(pModel);
    return result;
}

Noesis::Ptr<ViewModel> ViewModel::CreateFromModel(const SharedPtr<Model> & pModel) {
    return CreateFromModel(pModel, _context);
}

const WeakPtr<Model> & ViewModel::GetModel() const {
    return _pModel;
}

ViewModel::Context ViewModel::GetContext() const {
    return _context;
}

ViewModel::~ViewModel() {
    if(!_pModel)
        return;
    _context.cache.Erase({}, _pModel);
}

ViewModel::HandlerResult ViewModel::_HandleModelChanged(const BaseProperty & property, const void * oldValue, const void * newValue) {
    return HandlerResult::NotHandled;
}

void ViewModel::_Observe(const SharedPtr<Model> & pModel) {
    _pModel = pModel;

    _subscription = Subscribe(_pModel,
        [this](const BaseProperty & property, const void * oldValue, const void * newValue) {
            _HandleModelChanged(property, oldValue, newValue);
        }
    );
}

// Factory

ViewModel::Factory::Function ViewModel::Factory::Find(std::type_index type) const {
    if (auto iter = _map.find(type); iter != _map.end())
        return iter->second;
    return {};
}

ViewModel::Factory::Factory(Map map) : _map(std::move(map)) {}

// Builder

ViewModel::Factory ViewModel::Factory::Builder::Build() && {
    return Factory(std::move(_map));
}

}
