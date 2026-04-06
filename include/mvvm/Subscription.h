#pragma once

#include <cassert>

#include <mvvm/SharedPtr.h>
#include <mvvm/SubscriberId.h>

namespace mvvm {

template<typename T>
class Subscription {
public:
    Subscription() = default;

    Subscription(const WeakPtr<T> & pModel, SubscriberId<T> id) : _pModel(pModel), _id(id) {}

    Subscription(const Subscription &) = delete;

    Subscription(Subscription && value) : _pModel(std::move(value._pModel)), _id(value._id) {}

    ~Subscription() { _Clear(); }

    Subscription & operator=(const Subscription &) = delete;

    Subscription & operator=(Subscription && value) {
        _Clear();

        _pModel = std::move(value._pModel);
        _id = value._id;

        value._id = {};

        return *this;
    }

private:
    void _Clear() {
        if (!_pModel.Expired() && _id)
            _pModel->RemoveSubscriber(_id);
    }

    WeakPtr<T> _pModel;
    SubscriberId<T> _id;
};

template<typename T>
Subscription<T> Subscribe(const WeakPtr<T> & pSubject, typename T::Subscriber subscriber) {
    assert(!pSubject.Expired());
    return Subscription<T>(pSubject, pSubject->AddSubscriber(std::move(subscriber)));
}

}
