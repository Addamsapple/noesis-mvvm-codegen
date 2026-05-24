#include <doctest/doctest.h>

#include <ostream>

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

    TEST_CASE("Valid index is resolved") {
        mvvm::ModelCollection<int> collection;
        collection.Add(10);
        collection.Add(20);

        auto result = mvvm::Resolve(&collection, ":1");

        REQUIRE(result.HasValue());
        CHECK(result.Value().Get().As<int>() == 20);
    }

    TEST_CASE("Resolved index can be overwritten") {
        mvvm::ModelCollection<int> collection;
        collection.Add(0);

        mvvm::Resolve(&collection, ":0").Value().Set(333);

        CHECK(collection.Get(0) == 333);
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

    TEST_CASE("Valid property is resolved") {
        ParentModel parent;
        parent.SetNum(100);

        auto result = mvvm::Resolve(&parent, ".num");

        REQUIRE(result.HasValue());
        CHECK(result.Value().Get().As<int32_t>() == 100);
    }

    TEST_CASE("Resolved property can be overwritten") {
        ParentModel parent;
        parent.SetNum(0);

        mvvm::Resolve(&parent, ".num").Value().Set(999);

        CHECK(parent.GetNum() == 999);
    }

    TEST_CASE("Circular path is resolved") {
        auto pParent = mvvm::SharedPtr<ParentModel>::Make();
        auto pChild = mvvm::SharedPtr<ChildModel>::Make();
        pParent->SetNum(200);
        pParent->SetChild(pChild);
        pChild->SetParent(pParent);

        auto result = mvvm::Resolve(
            pParent.Get(), ".child.parent.child.parent.child.parent.num"
        );

        REQUIRE(result.HasValue());
        CHECK(result.Value().Get().As<int32_t>() == 200);
    }
}
