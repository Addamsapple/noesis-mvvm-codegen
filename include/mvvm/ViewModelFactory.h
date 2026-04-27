#pragma once

#include <functional>
#include <typeindex>
#include <type_traits>
#include <unordered_map>

#include <NsCore/Ptr.h>

#include <mvvm/ViewModel.h>

namespace mvvm {

class ViewModelFactory {
public:
    using Function = std::function<Noesis::Ptr<ViewModel> (ViewModel::Context)>;
    class Builder;

    Function Find(std::type_index type) const;

private:
    using Map = std::unordered_map<std::type_index, Function>;

    ViewModelFactory(Map map);

    template<typename T, std::enable_if_t<std::is_base_of_v<ViewModel, T>, int> = 0>
    static Function _Function() {
        return [](ViewModel::Context context) { return Noesis::Ptr(*new T({}, context)); };
    }

    const Map _map;
};

// Builder

class ViewModelFactory::Builder {
public:
    template<typename T, std::enable_if_t<std::is_base_of_v<ViewModel, T>, int> = 0>
    void Register() {
        _map.emplace(typeid(typename T::ModelType_), ViewModelFactory::_Function<T>());
    }

    ViewModelFactory Build() &&;

private:
    Map _map;
};

template<typename... Ts>
ViewModelFactory MakeFactory() {
    ViewModelFactory::Builder builder;
    (builder.Register<Ts>(), ...);
    return std::move(builder).Build();
}

}
