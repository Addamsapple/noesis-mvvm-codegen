#include <model/ManyPropModel.h>

#include <mvvm/ModelCollection.h>
#include <mvvm/Property.h>
#include <mvvm/PropertyList.h>

namespace model {

namespace {

const mvvm::PropertyList::Entry PROPERTIES[] = {
    {"Integer", &ManyPropModel::INTEGER_PROP},
    {"String", &ManyPropModel::STRING_PROP},
    {"Float", &ManyPropModel::FLOAT_PROP},
    {"Boolean", &ManyPropModel::BOOLEAN_PROP},
};

}

const mvvm::PropertyList & ManyPropModel::Properties() const {
    static const mvvm::PropertyList result(
        PROPERTIES,
        std::size(PROPERTIES),
        &mvvm::Model::Properties()
    );
    return result;
}

// ------ Integer ------
ManyPropModel::IntegerType ManyPropModel::GetInteger() const {
    return _integer;
}
void ManyPropModel::SetInteger(IntegerType value) {
    if (value != _integer) {
        auto oldValue = std::move(_integer);
        _integer = std::move(value);
        _NotifySubscribers(INTEGER_PROP, &oldValue, &value);
    }
}

// ------ String ------
const ManyPropModel::StringType & ManyPropModel::GetString() const {
    return _string;
}
void ManyPropModel::SetString(const StringType & value) {
    if (value != _string) {
        auto oldValue = std::move(_string);
        _string = value;
        _NotifySubscribers(STRING_PROP, &oldValue, &value);
    }
}

// ------ Float ------
ManyPropModel::FloatType ManyPropModel::GetFloat() const {
    return _float;
}
void ManyPropModel::SetFloat(FloatType value) {
    if (value != _float) {
        auto oldValue = std::move(_float);
        _float = std::move(value);
        _NotifySubscribers(FLOAT_PROP, &oldValue, &value);
    }
}

// ------ Boolean ------
ManyPropModel::BooleanType ManyPropModel::GetBoolean() const {
    return _boolean;
}
void ManyPropModel::SetBoolean(BooleanType value) {
    if (value != _boolean) {
        auto oldValue = std::move(_boolean);
        _boolean = std::move(value);
        _NotifySubscribers(BOOLEAN_PROP, &oldValue, &value);
    }
}

const mvvm::Property<ManyPropModel::IntegerType> ManyPropModel::INTEGER_PROP{
    &ManyPropModel::GetInteger
};
const mvvm::Property<ManyPropModel::StringType> ManyPropModel::STRING_PROP{
    &ManyPropModel::GetString
};
const mvvm::Property<ManyPropModel::FloatType> ManyPropModel::FLOAT_PROP{
    &ManyPropModel::GetFloat
};
const mvvm::Property<ManyPropModel::BooleanType> ManyPropModel::BOOLEAN_PROP{
    &ManyPropModel::GetBoolean
};

}