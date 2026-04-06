#pragma once

#include <unordered_map>

#include <mvvm/SharedPtr.h>

namespace mvvm {

template<typename T>
class Key;

class Model;
class ViewModel;

class Cache {
public:
    ViewModel * Find(WeakPtr<const Model> pModel) const;

    void Insert(Key<ViewModel>, WeakPtr<const Model> pModel, ViewModel * pViewModel);
    void Erase(Key<ViewModel>, WeakPtr<const Model> pModel);

private:
    std::unordered_map<WeakPtr<const Model>, ViewModel *> _map;
};

}
