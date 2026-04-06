#include <mvvm/BaseModelCollection.h>

#include <algorithm>
#include <cassert>

namespace mvvm {

BaseModelCollection::~BaseModelCollection() = default;

BaseModelCollection::SubscriberId BaseModelCollection::AddSubscriber(Subscriber subscriber) {
    assert(subscriber);

    _lastSubscriberId = _lastSubscriberId.Next({});
    _subscribers.push_back({_lastSubscriberId, std::move(subscriber)});

    return _lastSubscriberId;
}

void BaseModelCollection::RemoveSubscriber(SubscriberId id) {
    assert(id);

    // std::lower_bound would work, but probably not worth it
    auto iter = std::find_if(_subscribers.begin(), _subscribers.end(), [id](const auto & value) {
            return value.first == id;
    });

    assert(iter != _subscribers.end());
    _subscribers.erase(iter);
}

void BaseModelCollection::NotifySubscribers(Event event, uint32_t oldIndex, const void * pOldValue, uint32_t newIndex, const void * pNewValue) {
    for (const auto & subscriber : _subscribers)
        subscriber.second(event, oldIndex, pOldValue, newIndex, pNewValue);
}

}

