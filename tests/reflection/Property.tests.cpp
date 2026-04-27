#include <doctest/doctest.h>

#include <mvvm/Model.h>
#include <mvvm/ModelCollection.h>
#include <mvvm/Property.h>

namespace {

class TestModel : public mvvm::Model {
public:
    // int32_t
    int32_t GetInt32() const {
        return _i32;
    }
    void SetInt32(int32_t value) {
        _i32 = value;
    }

    // Model
    const mvvm::SharedPtr<TestModel> & GetModel() const {
        return _pModel;
    }
    void SetModel(const mvvm::SharedPtr<TestModel> & value) {
        _pModel = value;
    }

    // Collection
    const mvvm::SharedPtr<mvvm::ModelCollection<int>> & GetCollection() const {
        return _pCollection;
    }
    void SetCollection(const mvvm::SharedPtr<mvvm::ModelCollection<int>> & value) {
        _pCollection = value;
    }

    // Properties
    static inline mvvm::Property<int32_t> INT32_PROP{
        &TestModel::GetInt32, &TestModel::SetInt32
    };
    static inline mvvm::Property<mvvm::SharedPtr<TestModel>> MODEL_PROP{
        &TestModel::GetModel, &TestModel::SetModel
    };
    static inline mvvm::Property<mvvm::SharedPtr<mvvm::ModelCollection<int>>> COLLECTION_PROP{
        &TestModel::GetCollection, &TestModel::SetCollection
    };

private:
    int32_t _i32 = 0;
    mvvm::SharedPtr<TestModel> _pModel;
    mvvm::SharedPtr<mvvm::ModelCollection<int>> _pCollection;
};

}

TEST_SUITE("Property") {
    TEST_CASE("Contained type is consistent with reported one") {
        // signed
        SUBCASE("int8_t") {
            CHECK(mvvm::Property<int8_t>().Type() == mvvm::ValueType::Int8);
        }
        SUBCASE("int16_t") {
            CHECK(mvvm::Property<int16_t>().Type() == mvvm::ValueType::Int16);
        }
        SUBCASE("int32_t") {
            CHECK(mvvm::Property<int32_t>().Type() == mvvm::ValueType::Int32);
        }
        SUBCASE("int64_t") {
            CHECK(mvvm::Property<int64_t>().Type() == mvvm::ValueType::Int64);
        }

        // unsigned
        SUBCASE("uint8_t") {
            CHECK(mvvm::Property<uint8_t>().Type() == mvvm::ValueType::UInt8);
        }
        SUBCASE("uint16_t") {
            CHECK(mvvm::Property<uint16_t>().Type() == mvvm::ValueType::UInt16);
        }
        SUBCASE("uint32_t") {
            CHECK(mvvm::Property<uint32_t>().Type() == mvvm::ValueType::UInt32);
        }
        SUBCASE("uint64_t") {
            CHECK(mvvm::Property<uint64_t>().Type() == mvvm::ValueType::UInt64);
        }

        // floating-point
        SUBCASE("float") {
            CHECK(mvvm::Property<float>().Type() == mvvm::ValueType::Float);
        }
        SUBCASE("double") {
            CHECK(mvvm::Property<double>().Type() == mvvm::ValueType::Double);
        }

        // class
        SUBCASE("string") {
            CHECK(mvvm::Property<std::string>().Type() == mvvm::ValueType::String);
        }
        SUBCASE("Model") {
            CHECK(mvvm::Property<mvvm::SharedPtr<mvvm::Model>>().Type()
                == mvvm::ValueType::Model);
        }
        SUBCASE("Collection") {
            CHECK(mvvm::Property<mvvm::SharedPtr<mvvm::ModelCollection<int>>>().Type()
                == mvvm::ValueType::Collection);
        }
    }

    TEST_CASE("Contained value is returned by getter") {
        TestModel model;

        SUBCASE("int32_t") {
            model.SetInt32(5);

            int32_t output;
            TestModel::INT32_PROP.Get(model, static_cast<void *>(&output));

            CHECK(output == 5);
        }

        SUBCASE("Model") {
            auto pModel = mvvm::SharedPtr<TestModel>::Make();
            model.SetModel(pModel);

            mvvm::SharedPtr<mvvm::Model> output;
            TestModel::MODEL_PROP.Get(model, static_cast<void *>(&output));

            CHECK(output == pModel);
        }

        SUBCASE("Collection") {
            auto pCollection = mvvm::SharedPtr<mvvm::ModelCollection<int>>::Make();
            model.SetCollection(pCollection);

            mvvm::SharedPtr<mvvm::BaseModelCollection> output;
            TestModel::COLLECTION_PROP.Get(model, static_cast<void *>(&output));

            CHECK(output == pCollection);
        }
    }

    TEST_CASE("Contained value is overwritten by setter") {
        TestModel model;

        SUBCASE("int32_t") {
            int32_t input = 10;

            TestModel::INT32_PROP.Set(model, static_cast<void *>(&input));

            CHECK(model.GetInt32() == input);
        }

        SUBCASE("Model") {
            auto input = mvvm::SharedPtr<TestModel>::Make();
            auto castInput = input.StaticCast<mvvm::Model>();

            TestModel::MODEL_PROP.Set(model, static_cast<const void *>(&castInput));

            CHECK(model.GetModel() == input);
        }

        SUBCASE("Collection") {
            auto input = mvvm::SharedPtr<mvvm::ModelCollection<int>>::Make();
            auto castInput = input.StaticCast<mvvm::BaseModelCollection>();

            TestModel::COLLECTION_PROP.Set(model, static_cast<const void *>(&castInput));

            CHECK(model.GetCollection() == input);
        }
    }
}
