#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include <mvvm/SubscriberId.h>

namespace mvvm {

class BaseModelCollection {
public:
    enum class Event {
        ItemAdded,
        ItemMoved,
        ItemRemoved,
        ItemReplaced,
        PreCleared,
        Cleared
    };

    using Subscriber = std::function<void (Event event, uint32_t oldIndex, const void * pOldValue,  uint32_t newIndex, const void * pNewValue)>;
    using SubscriberId = mvvm::SubscriberId<BaseModelCollection>;

    virtual ~BaseModelCollection() = 0;

    SubscriberId AddSubscriber(Subscriber subscriber);
    void RemoveSubscriber(SubscriberId id);

protected:
    void _NotifySubscribers(Event event, uint32_t oldIndex, const void * pOldValue, uint32_t newIndex, const void * pNewValue);

private:
    std::vector<std::pair<SubscriberId, Subscriber>> _subscribers;
    SubscriberId _lastSubscriberId;
};

}

