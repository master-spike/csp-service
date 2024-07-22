#pragma once

#include <cassert>
#include <concepts>
#include <set>
#include <type_traits>
#include <iostream>
#include <limits>

namespace csplib {

template<std::integral T>
class interval_set {
private:
    // represents an interval [l, u)
    struct interval {
        T lower;
        T upper;

        interval(T lo, T hi)
        : lower(lo), upper(hi) {}

        bool operator==(interval const&) const = default;

        bool operator<(interval const& other) const {
            return (lower != other.lower)
                ? (lower < other.lower)
                : (upper < other.upper);
        }
    };
public:

    using value_type = T;
    using reference = T&;
    using const_reference = T const&;

    void add_value(value_type val) {
        add_range(val, val+1);
    }

    void add_range(value_type val_lo, value_type val_hi) {

        if (val_lo >= val_hi) return;

        if (empty()) {
            m_intervals.emplace_hint(m_intervals.end(), val_lo, val_hi);
            return;
        }

        auto it = m_intervals.lower_bound(interval(val_lo, val_lo));
        
        while(it != m_intervals.end() && it->lower <= val_hi) {
            val_hi = std::max(val_hi, it->upper);
            it = m_intervals.erase(it);
        }

        if (it != m_intervals.begin()) {
            --it;
            if (it->upper >= val_lo) {
                val_lo = it->lower;
                val_hi = std::max(it->upper, val_hi);
                it = m_intervals.erase(it);
            }
            else {
                ++it;
            }
            
        }

        m_intervals.emplace_hint(it, val_lo, val_hi);

    }

    void remove_value(value_type val) {
        remove_range(val, val+1);
    }

    void remove_range(value_type val_lo, value_type val_hi) {
        if (val_lo >= val_hi) return;

        if (empty()) return;

        auto it = m_intervals.lower_bound(interval(val_lo, val_lo));



        // erase all overlapping intervals, if
        // an existing interval is sliced, note down the extremes
        // for readdition later

        value_type slice_hi = val_hi;
        value_type slice_lo = val_lo;

        while (it != m_intervals.end() && val_hi > it->lower) {
            slice_hi = it->upper;
            it = m_intervals.erase(it);
        }

        if (it != m_intervals.begin()) {
            --it;
            if (val_lo < it->upper) {
                slice_lo = it->lower;
                slice_hi = std::max(it->upper, slice_hi);
                it = m_intervals.erase(it);
            }
            else {
                ++it;
            }
        }

        // [slice_lo ...........]     [...][......... slice_hi)
        // remove :         [////////////////)
        // [slice_lo, val_lo) -   -   -   -  [val_hi, slice_hi)

        // replace the cut intervals if applicable
        if (slice_lo < val_lo) m_intervals.emplace_hint(it, slice_lo, val_lo);
        if (slice_hi > val_hi) m_intervals.emplace_hint(it, val_hi, slice_hi);

    }

    bool contains(value_type val) const {

        interval test_interval(val, std::numeric_limits<value_type>::max());

        if (empty()) return false;

        auto it = m_intervals.upper_bound(test_interval);
        if (it == m_intervals.begin()) return false;

        --it;

        return it->lower <= val && it->upper > val;

    }

    bool empty() const {
        return m_intervals.empty();
    }
    
private:

    std::set<interval> m_intervals;
};

}
