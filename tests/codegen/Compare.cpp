#include "Compare.h"

#include "generated/model/League.h"
#include "generated/model/Team.h"
#include "generated/model/Player.h"
#include "generated/model/Goalkeeper.h"
#include "generated/model/Outfielder.h"
#include "generated/model/Midfielder.h"

#include "generated/viewmodel/League.h"
#include "generated/viewmodel/Team.h"
#include "generated/viewmodel/Player.h"
#include "generated/viewmodel/Goalkeeper.h"
#include "generated/viewmodel/Outfielder.h"
#include "generated/viewmodel/Midfielder.h"

bool VmEqualsM(Noesis::Ptr<viewmodel::Midfielder> pViewModel, mvvm::SharedPtr<model::Midfielder> pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    return pViewModel->GetPasses() == pModel->GetPasses();
}

bool VmEqualsM(Noesis::Ptr<viewmodel::Outfielder> pViewModel, mvvm::SharedPtr<model::Outfielder> pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    if (pViewModel->GetGoals() != pModel->GetGoals())
        return false;

    if (auto pDerivedModel = pModel.DynamicCast<model::Midfielder>())
        return VmEqualsM(Noesis::DynamicPtrCast<viewmodel::Midfielder>(pViewModel), pDerivedModel);
    return pViewModel->GetClassType() == Noesis::TypeOf<viewmodel::Outfielder>();
}

bool VmEqualsM(Noesis::Ptr<viewmodel::Goalkeeper> pViewModel, mvvm::SharedPtr<model::Goalkeeper> pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    return pViewModel->GetSavePercentage() == pModel->GetSavePercentage();
}

template<typename T>
bool VmTracksM(Noesis::Ptr<T> pViewModel, mvvm::SharedPtr<typename T::ModelType_> pModel) {
    if(!pModel || !pViewModel)
        return !pModel && !pViewModel;
    return pViewModel->GetModel().Lock() == pModel;
}

template<typename T>
bool VmEqualsM(Noesis::Ptr<mvvm::ViewModelCollection<T>> pViewModel, typename mvvm::ViewModelCollection<T>::ModelCollectionType pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    if (pViewModel->Count() != pModel->Size())
        return false;
    for (auto i = 0; i < pModel->Size(); ++i) {
        auto pItemModel = pModel->Get(i);
        auto pItemViewModel = Noesis::Ptr(pViewModel->Get(i));

        if (!VmTracksM(pItemViewModel, pItemModel) || !VmEqualsM(pItemViewModel, pItemModel))
            return false;
    }
    return true;
}

bool VmEqualsM(Noesis::Ptr<viewmodel::League> pViewModel, mvvm::SharedPtr<model::League> pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    if (!pModel->GetTeams())
        return pViewModel->GetTeams() == nullptr;
    return VmEqualsM(pViewModel->GetTeams(), pModel->GetTeams());
}

bool VmEqualsM(Noesis::Ptr<viewmodel::Team> pViewModel, mvvm::SharedPtr<model::Team> pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    if (!VmEqualsM(pViewModel->GetCaptain(), pModel->GetCaptain()))
        return false;
    if (!VmTracksM(pViewModel->GetParent(), pModel->GetParent()))
        return false;
    return VmEqualsM(pViewModel->GetPlayers(), pModel->GetPlayers());
}

bool VmEqualsM(Noesis::Ptr<viewmodel::Player> pViewModel, mvvm::SharedPtr<model::Player> pModel) {
    if (!pModel || !pViewModel)
        return !pModel && !pViewModel;
    if (pViewModel->GetName() != pModel->GetName().c_str())
        return false;
    if (!VmTracksM(pViewModel->GetParent(), pModel->GetParent()))
        return false;

    if (auto pDerivedModel = pModel.DynamicCast<model::Goalkeeper>())
        return VmEqualsM(Noesis::DynamicPtrCast<viewmodel::Goalkeeper>(pViewModel), pDerivedModel);
    if (auto pDerivedModel = pModel.DynamicCast<model::Outfielder>())
        return VmEqualsM(Noesis::DynamicPtrCast<viewmodel::Outfielder>(pViewModel), pDerivedModel);
    return pViewModel->GetClassType() == Noesis::TypeOf<viewmodel::Player>();
}
