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

class NoPropModel : public mvvm::Model {
public:

    const mvvm::PropertyList & Properties() const override;


private:
};

}
