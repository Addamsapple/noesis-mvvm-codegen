#include <model/NoPropModel.h>

#include <mvvm/ModelCollection.h>
#include <mvvm/Property.h>
#include <mvvm/PropertyList.h>

namespace model {

namespace {

const mvvm::PropertyList::Entry PROPERTIES[] = {
};

}

const mvvm::PropertyList & NoPropModel::Properties() const {
    static const mvvm::PropertyList result(
        PROPERTIES,
        std::size(PROPERTIES),
        &mvvm::Model::Properties()
    );
    return result;
}


}