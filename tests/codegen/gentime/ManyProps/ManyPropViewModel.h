#pragma once

#include <NsGui/DependencyObject.h>

#include <mvvm/ViewModel.h>
#include <mvvm/ViewModelCollection.h>

namespace model {
class ManyPropViewModel;
}

namespace viewmodel {

class ManyPropViewModel : public mvvm::ViewModel {
public:
    using ParentType_ = mvvm::ViewModel;
    using ModelType_ = model::ManyPropViewModel;

    using IntegerType = int;
    using StringType = Noesis::String;
    using FloatType = float;
    using BooleanType = bool;

    static inline const Noesis::DependencyProperty * INTEGER_PROP = nullptr;
    static inline const Noesis::DependencyProperty * STRING_PROP = nullptr;
    static inline const Noesis::DependencyProperty * FLOAT_PROP = nullptr;
    static inline const Noesis::DependencyProperty * BOOLEAN_PROP = nullptr;

    using ParentType_::ParentType_;

    mvvm::WeakPtr<ModelType_> GetModel() const;

    IntegerType GetInteger() const;
    const StringType & GetString() const;
    FloatType GetFloat() const;
    BooleanType GetBoolean() const;

protected:
    HandlerResult _HandleModelChanged(
        const mvvm::BaseProperty & property,
        const void * oldValue,
        const void * newValue
    ) override;

    void _Observe(const mvvm::SharedPtr<mvvm::Model> & model) override;

private:
    void SetInteger(IntegerType value);
    void SetString(const char * pValue);
    void SetFloat(FloatType value);
    void SetBoolean(BooleanType value);

    NS_DECLARE_REFLECTION(ManyPropViewModel, ParentType_)
};

}
