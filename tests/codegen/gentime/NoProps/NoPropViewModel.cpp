#include <viewmodel/NoPropViewModel.h>

#include <NsCore/ReflectionImplement.h>
#include <NsGui/DependencyData.h>

#include <model/NoPropViewModel.h>
#include <mvvm/Property.h>

namespace viewmodel {

mvvm::WeakPtr<NoPropViewModel::ModelType_> NoPropViewModel::GetModel() const {
    return mvvm::ViewModel::GetModel().StaticCast<ModelType_>();
}

void NoPropViewModel::_Observe(const mvvm::SharedPtr<mvvm::Model> & pModel) {
    ParentType_::_Observe(pModel);


}

mvvm::ViewModel::HandlerResult NoPropViewModel::_HandleModelChanged(
    const mvvm::BaseProperty & property,
    const void * pOldValue,
    const void * pNewValue
) {
    if (ParentType_::_HandleModelChanged(property, pOldValue, pNewValue) == HandlerResult::Handled)
        return HandlerResult::Handled;


    return HandlerResult::NotHandled;
}

NS_IMPLEMENT_REFLECTION(NoPropViewModel, "viewmodel.NoPropViewModel") {
    Noesis::DependencyData * data = NsMeta<Noesis::DependencyData>(Noesis::TypeOf<SelfClass>());

}

}
