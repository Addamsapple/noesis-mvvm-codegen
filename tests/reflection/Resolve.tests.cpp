#include <doctest/doctest.h>

#include <mvvm/Property.h>
#include <mvvm/PropertyList.h>
#include <mvvm/ModelCollection.h>
#include <mvvm/Resolve.h>
#include <mvvm/Result.h>

namespace {

class ChildModel;

class ParentModel : public mvvm::Model {
public:
    const mvvm::SharedPtr<ChildModel> & GetChild() const {
        return _pChild;
    }
    void SetChild(const mvvm::SharedPtr<ChildModel> & pChild) {
        _pChild = pChild;
    }

    int32_t GetNum() const {
        return _num;
    }
    void SetNum(int32_t num) {
        _num = num;
    }

    static inline const mvvm::Property<mvvm::SharedPtr<ChildModel>> PROP_CHILD{
        &ParentModel::GetChild, &ParentModel::SetChild
    };
    static inline const mvvm::Property<int32_t> PROP_NUM{
        &ParentModel::GetNum, &ParentModel::SetNum
    };

    static inline const mvvm::PropertyList::Entry PROPERTIES[] = {
        {"child", &PROP_CHILD},
        {"num", &PROP_NUM}
    };

    const mvvm::PropertyList & Properties() const override {
        static const mvvm::PropertyList result(PROPERTIES, std::size(PROPERTIES), nullptr);
        return result;
    }

private:
    mvvm::SharedPtr<ChildModel> _pChild;
    int32_t _num;
};

class ChildModel : public mvvm::Model {
public:
    const mvvm::SharedPtr<ParentModel> & GetParent() const {
        return _pParent;
    }
    void SetParent(const mvvm::SharedPtr<ParentModel> & pParent) {
        _pParent = pParent;
    }

    static inline const mvvm::Property<mvvm::SharedPtr<ParentModel>> PROP_PARENT{
        &ChildModel::GetParent
    };

    static inline const mvvm::PropertyList::Entry PROPERTIES[] = {
        {"parent", &PROP_PARENT}
    };

    const mvvm::PropertyList & Properties() const override {
        static const mvvm::PropertyList result(PROPERTIES, std::size(PROPERTIES), nullptr);
        return result;
    }

private:
    mvvm::SharedPtr<ParentModel> _pParent;
};

}

TEST_SUITE("Resolve") {
    TEST_CASE("Invalid index produces an error") {
        mvvm::ModelCollection<int> collection;

        SUBCASE("Invalid string") {
            auto result = mvvm::Resolve(&collection, ":123abc");

            REQUIRE(!result.HasValue());
            CHECK(result.Error().type
                == mvvm::ResolveError::Type::InvalidIndex);
            CHECK(result.Error().path == ":123abc");
        }

        SUBCASE("Index out of bounds") {
            auto result = mvvm::Resolve(&collection, ":3");

            REQUIRE(!result.HasValue());
            CHECK(result.Error().type
                == mvvm::ResolveError::Type::ItemNotFound);
            CHECK(result.Error().path == ":3");
        }
    }

    TEST_CASE("Valid index is traversed") {
        mvvm::ModelCollection<int> collection;
        collection.Add(10);
        collection.Add(20);

        auto result = mvvm::Resolve(&collection, ":1");

        REQUIRE(result.HasValue());
        CHECK(static_cast<mvvm::ModelCollection<int> *>(
            result.Value().pCollection)->Get(result.Value().index) == 20);
    }

    TEST_CASE("Invalid property produces an error") {
        ParentModel parent;

        SUBCASE("Root") {
            auto result = mvvm::Resolve(&parent, ".number");

            REQUIRE(!result.HasValue());
            CHECK(result.Error().type
                == mvvm::ResolveError::Type::PropertyNotFound);
            CHECK(result.Error().path == ".number");
        }

        SUBCASE("Not root") {
            auto result = mvvm::Resolve(&parent, ".num.a");

            REQUIRE(!result.HasValue());
            CHECK(result.Error().type
                == mvvm::ResolveError::Type::UnexpectedPrimitive);
            CHECK(result.Error().path == ".a");
        }
    }

    TEST_CASE("Valid property is traversed") {
        ParentModel parent;
        parent.SetNum(100);

        auto result = mvvm::Resolve(&parent, ".num");

        REQUIRE(result.HasValue());

        int32_t num;
        result.Value().pProperty->Get(*result.Value().pModel, &num);
        CHECK(num == 100);
    }

    TEST_CASE("Circular path is traversed") {
        auto pParent = mvvm::SharedPtr<ParentModel>::Make();
        auto pChild = mvvm::SharedPtr<ChildModel>::Make();
        pParent->SetNum(200);
        pParent->SetChild(pChild);
        pChild->SetParent(pParent);

        auto result = mvvm::Resolve(
            pParent.Get(), ".child.parent.child.parent.child.parent.num"
        );

        REQUIRE(result.HasValue());

        int32_t num;
        result.Value().pProperty->Get(*result.Value().pModel, &num);
        CHECK(num == 200);
    }
}
