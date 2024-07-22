#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "csplib/interval_set.hpp"

#include <type_traits>

namespace csplib {

static_assert(std::is_same_v<interval_set<int>::value_type, int>, "");


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



