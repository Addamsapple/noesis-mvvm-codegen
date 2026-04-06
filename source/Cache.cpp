#include <mvvm/Cache.h>

#include <mvvm/Key.h>
#include <mvvm/Model.h>
#include <mvvm/ViewModel.h>

namespace mvvm {

ViewModel * Cache::Find(WeakPtr<const Model> pModel) const {
    if (auto iter = _map.find(pModel); iter != _map.end())
        return iter->second;
    return nullptr;
}

void Cache::Insert(Key<ViewModel>, WeakPtr<const Model> pModel, ViewModel * pViewModel) {
    _map.emplace(pModel, pViewModel);
}

void Cache::Erase(Key<ViewModel>, WeakPtr<const Model> pModel) {
    if (auto iter = _map.find(pModel); iter != _map.end())
        _map.erase(iter);
}

}
