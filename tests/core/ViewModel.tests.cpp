#include <doctest/doctest.h>

#include <NsCore/DynamicCast.h>

#include <mvvm/Cache.h>
#include <mvvm/ViewModel.h>

class MutableViewModel : public mvvm::ViewModel {
public:
    using mvvm::ViewModel::ViewModel;

    bool IsMutated() const { return _isMutated; }
    void Mutate() { _isMutated = true; }

private:
    bool _isMutated = false;
};

class RecursiveViewModel : public mvvm::ViewModel {
public:
    using mvvm::ViewModel::ViewModel;

protected:
    void _Observe(const mvvm::SharedPtr<mvvm::Model> & pModel) override {
        ViewModel::_Observe(pModel);
        CreateFromModel(pModel);
    }
};

template<typename... Ts>
struct TestContext {
    mvvm::ViewModel::Factory factory = mvvm::MakeFactory<Ts...>();
    mvvm::Cache cache;

    mvvm::ViewModel::Context Get() { return { factory, cache}; }
};

TEST_SUITE("ViewModel") {
    TEST_CASE("Empty view model is created from empty model") {
        TestContext<> context;

        CHECK(!mvvm::ViewModel::CreateFromModel({}, context.Get()));
    }

    TEST_CASE("Valid view model is created from valid model") {
        TestContext<mvvm::ViewModel> context;
        auto pModel = mvvm::SharedPtr<mvvm::Model>::Make();

        auto pViewModel = mvvm::ViewModel::CreateFromModel(pModel, context.Get());

        REQUIRE(pViewModel);
        CHECK(pViewModel->GetModel().Lock() == pModel);
    }

    TEST_CASE("Live view models are cached") {
        TestContext<mvvm::ViewModel> context;
        auto pModel = mvvm::SharedPtr<mvvm::Model>::Make();

        auto pViewModel = mvvm::ViewModel::CreateFromModel(pModel, context.Get());

        CHECK(mvvm::ViewModel::CreateFromModel(pModel, context.Get()) == pViewModel);
    }

    // address could get reused, so compare members instead
    TEST_CASE("Expired view models are not cached") {
        TestContext<MutableViewModel> context;
        auto pModel = mvvm::SharedPtr<mvvm::Model>::Make();
        {
            auto pViewModel = Noesis::StaticPtrCast<MutableViewModel>(mvvm::ViewModel::CreateFromModel(pModel, context.Get()));
            pViewModel->Mutate();
        }

        auto pNewViewModel = Noesis::StaticPtrCast<MutableViewModel>(mvvm::ViewModel::CreateFromModel(pModel, context.Get()));

        CHECK(!pNewViewModel->IsMutated());
    }

    TEST_CASE("Indirect recursion during synchronization does not cause stack overflow") {
        TestContext<RecursiveViewModel> context;
        auto pModel = mvvm::SharedPtr<mvvm::Model>::Make();

        auto pViewModel = mvvm::ViewModel::CreateFromModel(pModel, context.Get());

        // passed
    }
}
