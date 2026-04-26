#pragma once

#include <functional>
#include <string_view>
#include <vector>

#include <mvvm/Property.h>
#include <mvvm/SubscriberId.h>

namespace mvvm {

class PropertyList;

class Model {
public:
    using Subscriber = std::function<void (const BaseProperty & property, const void * pOldValue, const void * pNewValue)>;
    using SubscriberId = mvvm::SubscriberId<Model>;

    virtual const PropertyList & Properties() const;

    const BaseProperty * FindProperty(std::string_view name) const;

    SubscriberId AddSubscriber(Subscriber subscriber);

    template<typename T>
    SubscriberId AddSubscriber(const Property<T> & property, typename Property<T>::Subscriber subscriber) {
        auto pProperty = &property;
        return AddSubscriber([=](const BaseProperty & changedProperty, const void * pOldValue, const void * pNewValue) {
            if (changedProperty == *pProperty)
                subscriber(Property<T>::Cast(pOldValue), Property<T>::Cast(pNewValue));
        });
    }

    void RemoveSubscriber(SubscriberId id);

    virtual ~Model() = default;

protected:
    void _NotifySubscribers(const BaseProperty & property, const void * pOldValue, const void * pNewValue);

private:
    std::vector<std::pair<SubscriberId, Subscriber>> _subscribers;
    SubscriberId _lastSubscriberId;
};

}
