#pragma once

#include <functional>
#include <vector>

#include <mvvm/SubscriberId.h>

namespace mvvm {

struct BaseProperty {
    BaseProperty() = default;
    BaseProperty(const BaseProperty & value) = delete;
    BaseProperty & operator=(const BaseProperty & value) = delete;

    bool operator==(const BaseProperty & other) const {
        return this == &other;
    }
};

template<typename T>
struct Property : BaseProperty {
    using Subscriber = std::function<void (const T & oldValue, const T & newValue)>;

    static const T & Cast(const void * pValue) {
        return *static_cast<const T *>(pValue);
    }
};

class Model {
public:
    using Subscriber = std::function<void (const BaseProperty & property, const void * pOldValue, const void * pNewValue)>;
    using SubscriberId = mvvm::SubscriberId<Model>;

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
    void NotifySubscribers(const BaseProperty & property, const void * pOldValue, const void * pNewValue);

private:
    std::vector<std::pair<SubscriberId, Subscriber>> _subscribers;
    SubscriberId _lastSubscriberId;
};

}

