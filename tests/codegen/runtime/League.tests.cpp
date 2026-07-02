#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

#include "Compare.h"
#include "Factory.h"

#include "model/League.h"
#include "model/Team.h"

#include "viewmodel/League.h"
#include "viewmodel/Team.h"

TEST_SUITE("League codegen") {
    TEST_CASE("Teams is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::League>::Make();
        auto pTeams = model::League::TeamsType::Make();
        pTeams->Add(mvvm::SharedPtr<model::Team>::Make());
        pModel->SetTeams(pTeams);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::League>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetTeams() == pTeams);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetTeams(model::League::TeamsType::Make());
            pModel->GetTeams()->Add(mvvm::SharedPtr<model::Team>::Make());

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }
}
