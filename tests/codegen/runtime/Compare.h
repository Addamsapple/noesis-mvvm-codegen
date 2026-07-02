#pragma once

#include <NsCore/Ptr.h>

#include <mvvm/SharedPtr.h>

namespace model { class League; }
namespace model { class Team; }
namespace model { class Player; }

namespace viewmodel { class League; }
namespace viewmodel { class Team; }
namespace viewmodel { class Player; }

bool VmEqualsM(Noesis::Ptr<viewmodel::League> pViewModel, mvvm::SharedPtr<model::League> pModel);
bool VmEqualsM(Noesis::Ptr<viewmodel::Team> pViewModel, mvvm::SharedPtr<model::Team> pModel);
bool VmEqualsM(Noesis::Ptr<viewmodel::Player> pViewModel, mvvm::SharedPtr<model::Player> pModel);
