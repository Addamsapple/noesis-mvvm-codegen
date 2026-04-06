#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

#include "Compare.h"
#include "Factory.h"

#include "generated/model/League.h"
#include "generated/model/Player.h"
#include "generated/model/Team.h"

#include "generated/viewmodel/League.h"
#include "generated/viewmodel/Player.h"
#include "generated/viewmodel/Team.h"

TEST_SUITE("Team codegen") {
    TEST_CASE("Players are synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Team>::Make();
        auto pPlayers = model::Team::PlayersType::Make();
        pPlayers->Add(mvvm::SharedPtr<model::Player>::Make());
        pModel->SetPlayers(pPlayers);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Team>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetPlayers() == pPlayers);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetPlayers(model::Team::PlayersType::Make());
            pModel->GetPlayers()->Add(mvvm::SharedPtr<model::Player>::Make());

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }

    TEST_CASE("Captain is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Team>::Make();
        auto pCaptain = model::Team::CaptainType::Make();
        pModel->SetCaptain(pCaptain);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Team>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetCaptain() == pCaptain);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetCaptain(model::Team::CaptainType::Make());

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }

    TEST_CASE("Parent is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Team>::Make();
        auto pParent = model::Team::ParentType::Make();
        pModel->SetParent(pParent);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Team>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetParent() == pParent);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetParent(model::Team::ParentType::Make());

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }
}
