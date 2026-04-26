#pragma once

#include <string>
#include <type_traits>

#include <NsCore/Boxing.h>
#include <NsGui/ObservableCollection.h>

#include <mvvm/ModelCollection.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/Subscription.h>
#include <mvvm/Traits.h>
#include <mvvm/ViewModel.h>

namespace mvvm {

namespace detail {
    template<typename T, typename = void>
    struct ItemTypes {
        static_assert(AlwaysFalseV<T>, "Invalid item type for ViewModelCollection");
    };

    template<typename T>
    struct ItemTypes<T, std::enable_if_t<std::is_base_of_v<ViewModel, T>>> {
        using Model = SharedPtr<typename T::ModelType_>;
        using ViewModel = T;
    };

    template<typename T>
    struct ItemTypes<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
        using Model = T;
        using ViewModel = Noesis::Boxed<T>;
    };

    template<typename T>
    struct ItemTypes<T, std::enable_if_t<std::is_same_v<T, Noesis::String>>> {
        using Model = std::string;
        using ViewModel = Noesis::Boxed<T>;
    };
}

template<typename T>
class ViewModelCollection : public Noesis::ObservableCollection<typename detail::ItemTypes<T>::ViewModel> {
public:
    using ModelCollectionType = SharedPtr<ModelCollection<typename detail::ItemTypes<T>::Model>>;

    ViewModelCollection(ViewModel::Context context) : _context(context) {}

    void Observe(const ModelCollectionType & pCollection) {
        this->Clear();

        _pModelCollection = pCollection;

        for (auto & modelItem : *pCollection)
            this->Add(_CreateItem((void *) &modelItem));

        _subscription = Subscribe(_pModelCollection.template StaticCast<BaseModelCollection>(),
            [this](Event event, uint32_t oldIndex, const void * pOldValue, uint32_t newIndex, const void * pNewValue) {
                switch(event) {
                    case Event::ItemAdded:
                        this->Insert(newIndex, _CreateItem(pNewValue));
                        break;
                    case Event::ItemMoved:
                        this->Move(oldIndex, newIndex);
                        break;
                    case Event::ItemRemoved:
                        this->RemoveAt(oldIndex);
                        break;
                    case Event::ItemReplaced:
                        this->Set(newIndex, _CreateItem(pNewValue));
                        break;
                    case Event::PreCleared:
                        break;
                    case Event::Cleared:
                        this->Clear();
                        break;
                }
            }
        );
    }

private:
    using Event = BaseModelCollection::Event;

    auto _CreateItem(const void * pValue) {
        if constexpr (std::is_arithmetic_v<T>)
            return Noesis::StaticPtrCast<Noesis::Boxed<T>>(Noesis::Boxing::Box(*static_cast<const T *>(pValue)));

        if constexpr (std::is_same_v<T, Noesis::String>)
            return Noesis::StaticPtrCast<Noesis::Boxed<T>>(Noesis::Boxing::Box(static_cast<const std::string *>(pValue)->c_str()));

        if constexpr (std::is_base_of_v<ViewModel, T>) {
            auto pModelItem = *static_cast<const typename detail::ItemTypes<T>::Model * const>(pValue);
            return Noesis::StaticPtrCast<T>(ViewModel::CreateFromModel(pModelItem, _context));
        }
    }

    WeakPtr<ModelCollection<typename detail::ItemTypes<T>::Model>> _pModelCollection;
    Subscription<BaseModelCollection> _subscription;
    
    ViewModel::Context _context;

    NS_IMPLEMENT_INLINE_REFLECTION_(ViewModelCollection, Noesis::ObservableCollection<typename detail::ItemTypes<T>::ViewModel>, Noesis::IdOf("Collection<", Noesis::IdOf<T>(), ">"))
};

}
