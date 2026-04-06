#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

#include "Compare.h"
#include "Factory.h"

#include "generated/model/Player.h"
#include "generated/model/Team.h"

#include "generated/viewmodel/Player.h"
#include "generated/viewmodel/Team.h"

TEST_SUITE("Player codegen") {
    TEST_CASE("Name is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Player>::Make();
        pModel->SetName("AAA");
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Player>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetName() == "AAA");
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetName("BBB");

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }

    TEST_CASE("Parent is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Player>::Make();
        auto pParent = model::Player::ParentType::Make();
        pModel->SetParent(pParent);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Player>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetParent() == pParent);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetParent(model::Player::ParentType::Make());

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }
}
