#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

#include "Compare.h"
#include "Factory.h"

#include "generated/model/Midfielder.h"
#include "generated/viewmodel/Midfielder.h"

TEST_SUITE("Midfielder codegen") {
    TEST_CASE("Passes is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Midfielder>::Make();
        pModel->SetPasses(100);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Midfielder>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetPasses() == 100);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetPasses(200);

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }
}
