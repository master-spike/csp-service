#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "csplib/interval_set.hpp"

#include <type_traits>
#include <array>
#include <algorithm>

namespace csplib {

static_assert(std::is_same_v<interval_set<int>::value_type, int>, "");

TEST_CASE("interval_set<int> : iterators") {

    std::array<int, 12> sample_values = {
        1,2,3,4,5,6,11,12,13,14,15,16
    };

    interval_set<int> test_iset;

    std::for_each(sample_values.begin(), sample_values.end(), [&test_iset](int v) {
        test_iset.add_value(v);
    });

    CHECK(std::equal(test_iset.begin(), test_iset.end(), sample_values.begin(), sample_values.end()));
    CHECK(std::equal(test_iset.rbegin(), test_iset.rend(), sample_values.rbegin(), sample_values.rend()));

    auto it = test_iset.begin();
    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);

    auto rit = test_iset.rbegin();
    CHECK(*rit == 16);
    ++rit;
    CHECK(*rit == 15);
    for (int i = 0; i < 5; ++i) {
        ++rit;
    }
    CHECK(*rit == 6);

    int count = 0;
    for (auto v : test_iset) {
        static_assert(std::is_same_v<decltype(v), decltype(test_iset)::value_type>, "iterator dereference type incorrect");
        ++count;
    }

    CHECK(count == sample_values.size());
    CHECK(std::distance(test_iset.cbegin(), test_iset.cend()) == sample_values.size());

}


TEST_CASE("interval_set<int> : adding values") {

    interval_set<int> set;

    CHECK(!set.contains(0));
    CHECK(set.empty());


    set.add_range(-10, 10);

    CHECK(set.contains(0));
    CHECK(!set.contains(10));
    CHECK(set.contains(-10));
    CHECK(!set.contains(-11));

    set.add_value(10);

    CHECK(set.contains(10));
    CHECK(!set.contains(11));

    
    set.add_range(-40, -30);

    CHECK(set.contains(-40));
    CHECK(set.contains(-35));
    CHECK(!set.contains(-30));
    CHECK(set.contains(-10));
    CHECK(!set.contains(-11));

    set.add_value(5);

    CHECK(set.contains(5));
    CHECK(set.contains(0));
    CHECK(set.contains(10));

}

TEST_CASE("interval_set<int> : removing values") {
    interval_set<int> set;
    set.add_range(0, 1000);

    CHECK(set.contains(0));
    CHECK(!set.contains(-1));
    CHECK(set.contains(999));
    CHECK(!set.contains(1000));

    set.remove_range(-10, 0);

    CHECK(set.contains(0));
    CHECK(!set.contains(-1));
    CHECK(set.contains(999));
    CHECK(!set.contains(1000));

    set.remove_value(0);

    CHECK(!set.contains(0));
    CHECK(set.contains(1));
    
    set.remove_range(0, 10);

    CHECK(!set.contains(0));
    CHECK(!set.contains(9));
    CHECK(set.contains(10));
    CHECK(set.contains(999));
    CHECK(!set.contains(1000));

    set.remove_range(100, 200);

    CHECK(set.contains(99));
    CHECK(set.contains(200));
    CHECK(set.contains(999));
    CHECK(!set.contains(100));
    CHECK(!set.contains(199));
    CHECK(!set.contains(150));

    set.remove_range(980, 1000);

    CHECK(!set.contains(980));
    CHECK(!set.contains(999));
    CHECK(!set.contains(1000));
    CHECK(set.contains(979));

}

}
