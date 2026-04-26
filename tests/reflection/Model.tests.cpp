#include <doctest/doctest.h>

#include <mvvm/Model.h>
#include <mvvm/PropertyList.h>

namespace {

class TestModel : public mvvm::Model {
public:
    static inline const mvvm::Property<int> PROP_1;
    static inline const mvvm::Property<double> PROP_2;

    static inline const mvvm::PropertyList::Entry PROPERTIES[] = {
        {"prop1", &TestModel::PROP_1},
        {"prop2", &TestModel::PROP_2},
    };

    const mvvm::PropertyList & Properties() const override {
        static const mvvm::PropertyList result(PROPERTIES, std::size(PROPERTIES), nullptr);
        return result;
    }
};

}

TEST_SUITE("Model Reflection") {
    TEST_CASE("Valid properties are found") {
        mvvm::SharedPtr<mvvm::Model> pModel = mvvm::SharedPtr<TestModel>::Make();

        CHECK(pModel->FindProperty("prop1") == &TestModel::PROP_1);
    }

    TEST_CASE("Invalid properties are not found") {
        mvvm::SharedPtr<mvvm::Model> pModel = mvvm::SharedPtr<TestModel>::Make();

        CHECK(pModel->FindProperty("prop0") == nullptr);
    }
}
