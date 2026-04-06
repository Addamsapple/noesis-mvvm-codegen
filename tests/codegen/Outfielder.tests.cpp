#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

#include "Compare.h"
#include "Factory.h"

#include "generated/model/Outfielder.h"
#include "generated/viewmodel/Outfielder.h"

TEST_SUITE("Outfielder codegen") {
    TEST_CASE("Goals is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Outfielder>::Make();
        pModel->SetGoals(10);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Outfielder>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetGoals() == 10);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetGoals(-20);

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }
}
