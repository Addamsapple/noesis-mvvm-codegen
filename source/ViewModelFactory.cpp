#include <mvvm/ViewModelFactory.h>

namespace mvvm {

ViewModelFactory::Function ViewModelFactory::Find(std::type_index type) const {
    if (auto iter = _map.find(type); iter != _map.end())
        return iter->second;
    return {};
}

ViewModelFactory::ViewModelFactory(Map map) : _map(std::move(map)) {}

ViewModelFactory ViewModelFactory::Builder::Build() && {
    return { std::move(_map) };
}

}
