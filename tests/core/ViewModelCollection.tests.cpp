#include <doctest/doctest.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>
#include <mvvm/ViewModelCollection.h>

TEST_SUITE("ViewModelCollection") {
    TEST_CASE("View model synchronizes to model") {
        // TODO: context not used by primitive collections
        auto factory = mvvm::MakeFactory<>();
        mvvm::Cache cache;

        auto pModel = mvvm::SharedPtr<mvvm::ModelCollection<int>>::Make();
        mvvm::ViewModelCollection<int> viewModel({ factory, cache });

        SUBCASE("Model is populated before syncing") {
            pModel->Add(100);
            pModel->Add(200);
            pModel->Add(300);

            viewModel.Observe(pModel);

            REQUIRE(viewModel.Count() == 3);
            CHECK(viewModel.Get(0)->mValue == 100);
            CHECK(viewModel.Get(1)->mValue == 200);
            CHECK(viewModel.Get(2)->mValue == 300);
        }

        SUBCASE("Model is populated after syncing") {
            viewModel.Observe(pModel);

            SUBCASE("Append") {
                pModel->Add(100);
                pModel->Add(200);

                REQUIRE(viewModel.Count() == 2);
                CHECK(viewModel.Get(0)->mValue == 100);
                CHECK(viewModel.Get(1)->mValue == 200);
            }

            SUBCASE("Insert") {
                pModel->Insert(100, 0);
                pModel->Insert(200, 0);

                REQUIRE(viewModel.Count() == 2);
                CHECK(viewModel.Get(0)->mValue == 200);
                CHECK(viewModel.Get(1)->mValue == 100);
            }

            SUBCASE("Move") {
                pModel->Add(1);
                pModel->Add(2);
                pModel->Add(3);

                pModel->Move(2, 0);

                CHECK(viewModel.Get(0)->mValue == 3);
                CHECK(viewModel.Get(1)->mValue == 1);
                CHECK(viewModel.Get(2)->mValue == 2);
            }

            SUBCASE("Remove") {
                pModel->Add(0);
                pModel->Add(1);

                pModel->Remove(0);

                CHECK(viewModel.Count() == 1);
                CHECK(viewModel.Get(0)->mValue == 1);
            }

            SUBCASE("Replace") {
                pModel->Add(100);

                pModel->Set(200, 0);

                CHECK(viewModel.Get(0)->mValue == 200);
            }

            SUBCASE("Clear") {
                pModel->Add(0);

                pModel->Clear();

                CHECK(viewModel.Count() == 0);
            }
        }
    }
}

