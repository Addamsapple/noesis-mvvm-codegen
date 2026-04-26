#include <doctest/doctest.h>

#include <iterator>

#include <mvvm/Property.h>
#include <mvvm/PropertyList.h>

TEST_SUITE("PropertyList") {
    TEST_CASE("Empty list has size 0") {
        const mvvm::PropertyList list;

        CHECK(std::distance(list.begin(), list.end()) == 0);
    }

    TEST_CASE("Populated list is not empty") {
        const mvvm::PropertyList::Entry properties[] = {"prop", nullptr};
        const mvvm::PropertyList list(properties, 1, nullptr);

        CHECK(std::distance(list.begin(), list.end()) != 0);
    }

    TEST_CASE("Range-based for loop iterates over all properties in top-down order") {
        const mvvm::Property<int> prop0;
        const mvvm::Property<int> prop1;
        const mvvm::Property<int> prop2;

        const mvvm::PropertyList::Entry entries0[] = {"0", &prop0};
        const mvvm::PropertyList::Entry entries1[] = {"1", &prop1};
        const mvvm::PropertyList::Entry entries2[] = {"2", &prop2};

        const mvvm::PropertyList list0(entries0, std::size(entries0), nullptr);
        const mvvm::PropertyList list1(entries1, std::size(entries1), &list0);
        const mvvm::PropertyList list2(entries2, std::size(entries2), &list1);

        const mvvm::PropertyList::Entry orderedEntries[] = {*entries0, *entries1, *entries2};

        int index = 0;
        for (const auto & entry : list2) {
            CHECK(entry.name == orderedEntries[index].name);
            CHECK(entry.pProperty == orderedEntries[index].pProperty);
            ++index;
        }
    }
}
