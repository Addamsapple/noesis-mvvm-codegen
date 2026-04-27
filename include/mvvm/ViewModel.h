#pragma once

#include <NsGui/DependencyObject.h>

#include <mvvm/Key.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/SubscriberId.h>
#include <mvvm/Subscription.h>

namespace mvvm {

class BaseProperty;
class Cache;
class Model;
class ViewModelFactory;

class ViewModel : public Noesis::DependencyObject {
public:
    using ModelType_ = Model;

    struct Context {
        const ViewModelFactory & factory;
        Cache & cache;
    };

    ViewModel(const Key<ViewModelFactory> &, Context context);

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

}
