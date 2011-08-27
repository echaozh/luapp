/// luapp_impl.hpp -- template function impl for lua wrapper

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_IMPL_HPP_
#define INCLUDED_LUAPP_IMPL_HPP_ 1

#include "luapp.hpp"
#include "luapp_obj.hpp"

#include <iostream>

namespace luapp
{
    template <typename Integral>
    typename std::enable_if<std::is_integral<Integral>::value, lua &>::type
    operator << (lua &l, Integral n)
    {
        return l << (ptrdiff_t) n;
    }
    template <typename FP>
    typename std::enable_if<std::is_floating_point<FP>::value, lua &>::type
    operator << (lua &l, FP f)
    {
        return l << (double) f;
    }
    template <typename Container>
    typename std::enable_if<std::is_class<Container>::value, lua &>::type
    operator << (lua &l, const Container &c)
    {
        temp_table t (l);
        t.create (c.size ());
        for (size_t i = 0; i < c.size (); ++i) {
            value<typename Container::value_type> v (t, i + 1);
            v.set (c[i]);
        }
    }

    template <typename Integral>
    typename std::enable_if<std::is_integral<Integral>::value, lua &>::type
    operator >> (lua &l, Integral &n)
    {
        uint64_t m;
        l >> m;
        n = m;
        return l;
    }
    template <typename FP>
    typename std::enable_if<std::is_floating_point<FP>::value, lua &>::type
    operator >> (lua &l, FP &f)
    {
        double d;
        l >> d;
        f = d;
        return l;
    }
    template <typename T, size_t N>
    lua &operator >> (lua &l, std::array<T, N> &arr)
    {
        temp_table t (l);
        if (t.array_size () < N)
            throw std::out_of_range ("not enough items to fill array");
        for (size_t i = 0; i < N; ++i)
            arr[i] = value<T> (t, i + 1).get ();
        l.pop ();
        return l;
    }
    // we only convert items at indices 1 up to #arr
    template <typename Container>
    typename std::enable_if<std::is_class<Container>::value, lua &>::type
    operator >> (lua &l, Container &c)
    {
        temp_table t (l);
        c.resize (t.array_size ());
        for (size_t i = 0; i < c.size (); ++i)
            c[i] = value<typename Container::value_type> (t, i + 1).get ();
        l.pop ();
        return l;
    }
}

#endif // INCLUDED_LUAPP_IMPL_HPP_
