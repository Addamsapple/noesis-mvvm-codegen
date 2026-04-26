#include <mvvm/Model.h>

#include <algorithm>
#include <cassert>

#include <mvvm/PropertyList.h>

namespace mvvm {

const PropertyList & Model::Properties() const {
    static const PropertyList result;
    return result;
}

const BaseProperty * Model::FindProperty(std::string_view name) const {
    const auto & properties = Properties();
    auto iter = std::find_if(properties.begin(), properties.end(),
        [&](const PropertyList::Entry & entry) {
            return entry.name == name;
        });
    return iter == properties.end() ? nullptr : iter->pProperty;
}

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

void Model::_NotifySubscribers(const BaseProperty & property, const void * pOldValue, const void * pNewValue) {
    for (const auto & subscriber : _subscribers)
        subscriber.second(property, pOldValue, pNewValue);
}

}
