#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include <mvvm/SubscriberId.h>

namespace mvvm {

class Value;
enum class ValueType;

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

    virtual void Move(uint32_t oldIndex, uint32_t newIndex) = 0;
    virtual void Remove(uint32_t index) = 0;
    virtual void Clear() = 0;
    virtual uint32_t Size() const = 0;

    // TODO: opaque interfaces for Insert/Add
    virtual Value GetValue(uint32_t index) const = 0;
    virtual void SetValue(uint32_t index, const Value & value) = 0;

    virtual ValueType Type() const;

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

