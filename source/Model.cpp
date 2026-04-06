#include <mvvm/Model.h>

#include <algorithm>
#include <cassert>

namespace mvvm {

Model::SubscriberId Model::AddSubscriber(Subscriber subscriber) {
    assert(subscriber);

    _lastSubscriberId = _lastSubscriberId.Next({});
    _subscribers.push_back({_lastSubscriberId, std::move(subscriber)});

    return _lastSubscriberId;
}

void Model::RemoveSubscriber(SubscriberId id) {
    assert(id);

    // std::lower_bound would work, but probably not worth it
    auto iter = std::find_if(_subscribers.begin(), _subscribers.end(), [id](const auto & value) {
            return value.first == id;
    });

    assert(iter != _subscribers.end());
    _subscribers.erase(iter);
}

void Model::NotifySubscribers(const BaseProperty & property, const void * pOldValue, const void * pNewValue) {
    for (const auto & subscriber : _subscribers)
        subscriber.second(property, pOldValue, pNewValue);
}

}
