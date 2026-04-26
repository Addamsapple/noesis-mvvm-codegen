#pragma once

#include <mvvm/BaseModelCollection.h>

namespace mvvm {

class Model;

template<typename T>
class SharedPtr;

class BaseModelPtrCollection : public BaseModelCollection {
public:
    virtual void InsertModel(SharedPtr<Model> value, uint32_t index) = 0;
    virtual void AddModel(SharedPtr<Model> value) = 0;

    virtual SharedPtr<Model> GetModel(uint32_t index) = 0;
    virtual void SetModel(SharedPtr<Model> value, uint32_t index) = 0;

    // TODO: custom iterator
};

}
