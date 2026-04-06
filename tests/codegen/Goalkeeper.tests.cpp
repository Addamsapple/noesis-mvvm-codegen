#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

#include "Compare.h"
#include "Factory.h"

#include "generated/model/Goalkeeper.h"
#include "generated/viewmodel/Goalkeeper.h"

TEST_SUITE("Goalkeeper codegen") {
    TEST_CASE("Save percentage is synced") {
        mvvm::Cache cache;
        auto pModel = mvvm::SharedPtr<model::Goalkeeper>::Make();
        pModel->SetSavePercentage(0.5);
        auto pViewModel = Noesis::StaticPtrCast<viewmodel::Goalkeeper>(mvvm::ViewModel::CreateFromModel(pModel, { TEST_FACTORY, cache }));

        SUBCASE("Initial value is synced") {
            CHECK(pModel->GetSavePercentage() == 0.5);
            CHECK(VmEqualsM(pViewModel, pModel));
        }

        SUBCASE("Value changes are synced") {
            pModel->SetSavePercentage(1.0);

            CHECK(VmEqualsM(pViewModel, pModel));
        }
    }
}
