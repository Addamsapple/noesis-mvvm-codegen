#pragma once

#include <NsGui/DependencyObject.h>

#include <mvvm/ViewModel.h>
#include <mvvm/ViewModelCollection.h>

namespace model {
class NoPropViewModel;
}

namespace viewmodel {

class NoPropViewModel : public mvvm::ViewModel {
public:
    using ParentType_ = mvvm::ViewModel;
    using ModelType_ = model::NoPropViewModel;



    using ParentType_::ParentType_;

    mvvm::WeakPtr<ModelType_> GetModel() const;


protected:
    HandlerResult _HandleModelChanged(
        const mvvm::BaseProperty & property,
        const void * oldValue,
        const void * newValue
    ) override;

    void _Observe(const mvvm::SharedPtr<mvvm::Model> & model) override;

private:

    NS_DECLARE_REFLECTION(NoPropViewModel, ParentType_)
};

}
