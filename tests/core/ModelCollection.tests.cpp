#include <doctest/doctest.h>

#include <optional>
#include <vector>

#include <mvvm/ModelCollection.h>

using IntCollection = mvvm::ModelCollection<int>;

using Event = mvvm::BaseModelCollection::Event;

namespace {

struct Record {
    Event event;

    uint32_t oldIndex;
    uint32_t newIndex;

    std::optional<int> oldValue;
    std::optional<int> newValue;
};

class TestCallback {
public:
    using Records = std::vector<Record>;

    TestCallback(Records & records) : _records(records) {}

    void operator()(Event event, uint32_t oldIndex, const void * pOldValue,  uint32_t newIndex, const void * pNewValue) {
        Record record;

        record.event = event;
        record.oldIndex = oldIndex;
        record.newIndex = newIndex;

        if (pOldValue) record.oldValue = *static_cast<const int *>(pOldValue);
        if (pNewValue) record.newValue = *static_cast<const int *>(pNewValue);

        _records.push_back(record);
    }

private:
    Records & _records;
};

}

TEST_SUITE("ModelCollection") {
    TEST_CASE("Initial size is 0") {
        IntCollection list;

        CHECK(list.Size() == 0);
    }

    TEST_CASE("Appended items do not shift existing ones") {
        IntCollection list;

        list.Add(100);
        list.Add(200);

        REQUIRE(list.Size() == 2);
        CHECK(list.Get(0) == 100);
        CHECK(list.Get(1) == 200);
    }

    TEST_CASE("Inserted items shift subsequent ones") {
        IntCollection list;

        list.Insert(100, 0);
        list.Insert(99, 0);

        REQUIRE(list.Size() == 2);
        CHECK(list.Get(0) == 99);
        CHECK(list.Get(1) == 100);
    }

    TEST_CASE("Replaced items are overwritten") {
        IntCollection list;
        list.Add(100);
        list.Add(200);
        list.Add(300);

        list.Set(400, 0);
        list.Set(600, 2);

        CHECK(list.Get(0) == 400);
        CHECK(list.Get(1) == 200);
        CHECK(list.Get(2) == 600);
    }

    TEST_CASE("Size is 0 after clearing") {
        IntCollection list;
        list.Add(0);
        list.Add(0);
        list.Add(0);

        list.Clear();

        CHECK(list.Size() == 0);
    }

    TEST_CASE("Moving items shifts others within range") {
        IntCollection list;
        list.Add(11);
        list.Add(12);
        list.Add(13);
        list.Add(14);

        SUBCASE("Shift right") {
            list.Move(0, 2);

            CHECK(list.Get(0) == 12);
            CHECK(list.Get(1) == 13);
            CHECK(list.Get(2) == 11);
            CHECK(list.Get(3) == 14);
        }

        SUBCASE("Shift left") {
            list.Move(3, 1);

            CHECK(list.Get(0) == 11);
            CHECK(list.Get(1) == 14);
            CHECK(list.Get(2) == 12);
            CHECK(list.Get(3) == 13);
        }

        SUBCASE("Don't shift") {
            list.Move(1, 1);

            CHECK(list.Get(0) == 11);
            CHECK(list.Get(1) == 12);
            CHECK(list.Get(2) == 13);
            CHECK(list.Get(3) == 14);
        }
    }

    TEST_CASE("Removed items shift subsequent ones") {
        IntCollection list;
        list.Add(10);
        list.Add(20);
        list.Add(30);

        list.Remove(1);

        CHECK(list.Size() == 2);
        CHECK(list.Get(0) == 10);
        CHECK(list.Get(1) == 30);
    }

    TEST_CASE("Range-based for loop iterates over all items") {
        IntCollection list;
        list.Add(1);
        list.Add(2);
        list.Add(3);
        int sum = 0;

        for (const auto & item : list)
            sum += item;

        CHECK(sum == 6);
    }

    TEST_CASE("Subscribers trigger on notification") {
        IntCollection list;
        std::vector<Record> records;
        auto id = list.AddSubscriber(TestCallback(records));

        SUBCASE("Addition") {
            list.Insert(100, 0);
            list.Insert(200, 1);

            CHECK(records.back().event == Event::ItemAdded);
            CHECK(records.back().newIndex == 1);
            REQUIRE(records.back().newValue.has_value());
            CHECK(*records.back().newValue == 200);
        }

        SUBCASE("Replacement") {
            list.Add(100);
            list.Add(200);
            list.Set(300, 1);

            CHECK(records.back().event == Event::ItemReplaced);
            CHECK(records.back().newIndex == 1);
            REQUIRE(records.back().oldValue.has_value());
            REQUIRE(records.back().newValue.has_value());
            CHECK(*records.back().oldValue == 200);
            CHECK(*records.back().newValue == 300);
        }

        SUBCASE("Movement") {
            list.Add(100);
            list.Add(200);
            list.Move(1, 0);

            CHECK(records.back().event == Event::ItemMoved);
            CHECK(records.back().oldIndex == 1);
            CHECK(records.back().newIndex == 0);
        }

        SUBCASE("Removal") {
            list.Add(100);
            list.Add(200);
            list.Remove(1);

            CHECK(records.back().event == Event::ItemRemoved);
            CHECK(records.back().oldIndex == 1);
            REQUIRE(records.back().oldValue.has_value());
            CHECK(*records.back().oldValue == 200);
        }

        SUBCASE("Clearing") {
            list.Clear();

            CHECK(records.front().event == Event::PreCleared);
            CHECK(records.back().event == Event::Cleared);
        }
    }
}
