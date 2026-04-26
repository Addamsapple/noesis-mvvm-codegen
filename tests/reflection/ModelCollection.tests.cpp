#include <doctest/doctest.h>

#include <mvvm/Model.h>
#include <mvvm/ModelCollection.h>

namespace {

class SubModel : public mvvm::Model {};

}

using SubModelPtr = mvvm::SharedPtr<SubModel>;

TEST_SUITE("ModelCollection Reflection") {
    TEST_CASE("Contained type is consistent with reported one") {
        // signed
        SUBCASE("int8_t") {
            CHECK(mvvm::ModelCollection<int8_t>().Type() == mvvm::ValueType::Int8);
        }
        SUBCASE("int16_t") {
            CHECK(mvvm::ModelCollection<int16_t>().Type() == mvvm::ValueType::Int16);
        }
        SUBCASE("int32_t") {
            CHECK(mvvm::ModelCollection<int32_t>().Type() == mvvm::ValueType::Int32);
        }
        SUBCASE("int64_t") {
            CHECK(mvvm::ModelCollection<int64_t>().Type() == mvvm::ValueType::Int64);
        }

        // unsigned
        SUBCASE("uint8_t") {
            CHECK(mvvm::ModelCollection<uint8_t>().Type() == mvvm::ValueType::UInt8);
        }
        SUBCASE("uint16_t") {
            CHECK(mvvm::ModelCollection<uint16_t>().Type() == mvvm::ValueType::UInt16);
        }
        SUBCASE("uint32_t") {
            CHECK(mvvm::ModelCollection<uint32_t>().Type() == mvvm::ValueType::UInt32);
        }
        SUBCASE("uint64_t") {
            CHECK(mvvm::ModelCollection<uint64_t>().Type() == mvvm::ValueType::UInt64);
        }

        // floating-point
        SUBCASE("float") {
            CHECK(mvvm::ModelCollection<float>().Type() == mvvm::ValueType::Float);
        }
        SUBCASE("double") {
            CHECK(mvvm::ModelCollection<double>().Type() == mvvm::ValueType::Double);
        }

        // class
        SUBCASE("string") {
            CHECK(mvvm::ModelCollection<std::string>().Type() == mvvm::ValueType::String);
        }
        SUBCASE("Model") {
            CHECK(mvvm::ModelCollection<mvvm::SharedPtr<mvvm::Model>>().Type()
                == mvvm::ValueType::Model);
        }
        SUBCASE("Collection") {
            // TODO: add nested collection support
            // CHECK(mvvm::ModelCollection<mvvm::ModelCollection<int>>().Type()
            //     == mvvm::ValueType::Collection);
        }
    }

    TEST_CASE("Collection of model subcasses can use generic model interface") {
        mvvm::ModelCollection<SubModelPtr> collection;
        mvvm::BaseModelCollection * pCollection = &collection;

        CHECK(dynamic_cast<mvvm::BaseModelPtrCollection *>(pCollection));
    }

    TEST_CASE("Collection of primitives cannot use generic model interface") {
        mvvm::ModelCollection<float> collection;
        mvvm::BaseModelCollection * pCollection = &collection;

        CHECK(!dynamic_cast<mvvm::BaseModelPtrCollection *>(pCollection));
    }

    TEST_CASE("Items are inserted using generic model interface") {
        mvvm::ModelCollection<SubModelPtr> collection;

        collection.InsertModel(SubModelPtr::Make(), 0);

        CHECK(collection.Get(0) != SubModelPtr());
    }

    TEST_CASE("Items are appended using generic model interface") {
        mvvm::ModelCollection<SubModelPtr> collection;

        collection.AddModel(SubModelPtr::Make());

        CHECK(collection.Get(0) != SubModelPtr());
    }

    TEST_CASE("Items are retrieved using generic model interface") {
        mvvm::ModelCollection<SubModelPtr> collection;
        collection.Add(SubModelPtr::Make());

        CHECK(collection.GetModel(0) != SubModelPtr());
    }

    TEST_CASE("Items are overwritten using generic model interface") {
        mvvm::ModelCollection<SubModelPtr> collection;
        auto pModel = SubModelPtr::Make();
        collection.Add(pModel);

        collection.SetModel(SubModelPtr::Make(), 0);

        CHECK(collection.Get(0) != pModel);
    }
}
