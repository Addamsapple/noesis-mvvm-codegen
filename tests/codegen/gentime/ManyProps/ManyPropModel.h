#pragma once

#include <memory>
#include <string>

#include <mvvm/Model.h>
#include <mvvm/SharedPtr.h>

namespace mvvm {

template<typename T>
class ModelCollection;

template<typename T>
class Property;

}

namespace model {

class ManyPropModel : public mvvm::Model {
public:
    using IntegerType = int;
    using StringType = std::string;
    using FloatType = float;
    using BooleanType = bool;

    const mvvm::PropertyList & Properties() const override;

    // ------ Integer ------
    IntegerType GetInteger() const;
    void SetInteger(IntegerType value);

    // ------ String ------
    const StringType & GetString() const;
    void SetString(const StringType & value);

    // ------ Float ------
    FloatType GetFloat() const;
    void SetFloat(FloatType value);

    // ------ Boolean ------
    BooleanType GetBoolean() const;
    void SetBoolean(BooleanType value);

    static const mvvm::Property<IntegerType> INTEGER_PROP;
    static const mvvm::Property<StringType> STRING_PROP;
    static const mvvm::Property<FloatType> FLOAT_PROP;
    static const mvvm::Property<BooleanType> BOOLEAN_PROP;

private:
    IntegerType _integer;
    StringType _string;
    FloatType _float;
    BooleanType _boolean;
};

}
