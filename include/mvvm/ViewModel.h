#pragma once

#include <functional>
#include <map>
#include <typeindex>
#include <type_traits>
#include <unordered_map>

#include <NsGui/DependencyObject.h>

#include <mvvm/Key.h>
#include <mvvm/Model.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/SubscriberId.h>
#include <mvvm/Subscription.h>

namespace mvvm {

class Cache;

// ViewModel

class ViewModel : public Noesis::DependencyObject {
public:
    using ModelType_ = Model;
    class Factory;

    struct Context {
        const Factory & factory;
        Cache & cache;
    };

    ViewModel(const Key<Factory> &, Context context);

    static Noesis::Ptr<ViewModel> CreateFromModel(const SharedPtr<Model> & pModel, Context context);
    Noesis::Ptr<ViewModel> CreateFromModel(const SharedPtr<Model> & pModel);

    const WeakPtr<Model> & GetModel() const;
    Context GetContext() const;

    ~ViewModel() override;

protected:
    enum class HandlerResult {
        NotHandled,
        Handled
    };

    virtual HandlerResult _HandleModelChanged(const BaseProperty & property, const void * oldValue, const void * newValue);

    virtual void _Observe(const SharedPtr<Model> & model);

    template<typename T>
    void _ObserveCollection(const Noesis::DependencyProperty * pProperty, const typename T::ModelCollectionType & pCollection) {
        if (!pCollection) {
            Noesis::DependencyObject::SetReadOnlyProperty<Noesis::Ptr<T>>(pProperty, nullptr);
            return;
        }

        auto pValue = Noesis::DependencyObject::GetValue<Noesis::Ptr<T>>(pProperty);
        if (!pValue) {
            pValue = Noesis::MakePtr<T>(_context);
            Noesis::DependencyObject::SetReadOnlyProperty<Noesis::Ptr<T>>(pProperty, pValue);
        }

        pValue->Observe(pCollection);
    }

private:
    WeakPtr<Model> _pModel;
    Subscription<Model> _subscription;

    Context _context;

    NS_IMPLEMENT_INLINE_REFLECTION_(ViewModel, Noesis::DependencyObject, "ViewModel")
};

// Factory

class ViewModel::Factory {
public:
    using Function = std::function<Noesis::Ptr<ViewModel> (Context context)>;
    class Builder;

    Function Find(std::type_index type) const;

private:
    using Map = std::unordered_map<std::type_index, Function>;

    Factory(Map map);

    template<typename T, std::enable_if_t<std::is_base_of_v<ViewModel, T>, int> = 0>
    static Function _Function() {
        return [](Context context) { return Noesis::Ptr(*new T({}, context)); };
    }

    const Map _map;
};

// Builder

class ViewModel::Factory::Builder {
public:
    template<typename T, std::enable_if_t<std::is_base_of_v<ViewModel, T>, int> = 0>
    void Register() {
        _map.emplace(typeid(typename T::ModelType_), Factory::_Function<T>());
    }

    Factory Build() &&;

private:
    Map _map;
};

template<typename... Ts>
ViewModel::Factory MakeFactory() {
    ViewModel::Factory::Builder builder;
    (builder.Register<Ts>(), ...);
    return std::move(builder).Build();
}

}
