#include <doctest/doctest.h>

#include <mvvm/Model.h>

namespace {

class TestModel : public mvvm::Model {
public:
    void SetInt(int value) {
        auto oldValue = _int;
        _int = value;
        _NotifySubscribers(INT_PROPERTY, &oldValue, &_int);
    }

    static inline const mvvm::Property<int> INT_PROPERTY;
private:
    int _int = 0;
};

class TestCallback {
public:
    TestCallback(int & counter) : _counter(counter) {}

    void operator()(const mvvm::BaseProperty & property, const void * pOldValue, const void * pNewValue) {
        ++_counter;
    }
private:
    int & _counter;
};

}

TEST_SUITE("Model") {
    TEST_CASE("Properties are equal to themselves") {
        mvvm::Property<int> a;
        mvvm::BaseProperty & aRef = a;

        CHECK(aRef == a);
    }

    TEST_CASE("Different properties are not equal") {
        mvvm::Property<int> a;
        mvvm::Property<int> b;

        mvvm::BaseProperty & aRef = a;
        mvvm::BaseProperty & bRef = b;

        CHECK(!(aRef == bRef));
    }

    TEST_CASE("Subscriber IDs are unique") {
        TestModel model;
        int counter = 0;

        auto id1 = model.AddSubscriber(TestCallback(counter));
        auto id2 = model.AddSubscriber(TestCallback(counter));
        auto id3 = model.AddSubscriber(TestCallback(counter));

        CHECK((id1 && id2 && id3));
        CHECK(!(id1 == id2));
        CHECK(!(id1 == id3));
        CHECK(!(id2 == id3));
    }

    TEST_CASE("Subscribers trigger on notification") {
        TestModel model;
        int counter1 = 0;
        int counter2 = 0;
        auto id1 = model.AddSubscriber(TestCallback(counter1));
        auto id2 = model.AddSubscriber(TestCallback(counter2));

        SUBCASE("0 notifications") {
            CHECK(counter1 == 0);
            CHECK(counter2 == 0);
        }

        SUBCASE("1 notification") {
            model.SetInt(0);

            CHECK(counter1 == 1);
            CHECK(counter2 == 1);
        }

        SUBCASE("2 notifications") {
            model.SetInt(0);
            model.SetInt(0);

            CHECK(counter1 == 2);
            CHECK(counter2 == 2);
        }
    }

    TEST_CASE("Ex-subscribers do not trigger on notification") {
        TestModel model;
        int counter = 0;
        auto id = model.AddSubscriber(TestCallback(counter));

        model.RemoveSubscriber(id);
        model.SetInt(0);

        CHECK(counter == 0);
    }
}
