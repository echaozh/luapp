/// luapp_impl.hpp -- template function impl for lua wrapper

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_IMPL_HPP_
#define INCLUDED_LUAPP_IMPL_HPP_ 1

#include "luapp.hpp"
#include "luapp_obj.hpp"

namespace luapp
{
    template <typename Container>
    lua &operator << (lua &l, const Container &c)
    {
        temp_table t (l);
        t.create (c.size ());
        for (size_t i = 0; i < c.size (); ++i) {
            value<typename Container::value_type> v (t, i + 1);
            v.set (c[i]);
        }
    }

    // we only convert items at indices 1 up to #arr
    template <typename Container>
    lua &operator >> (lua &l, Container &c)
    {
        temp_table t (l);
        c.resize (t.array_size ());
        for (size_t i = 0; i < c.size (); ++i) {
            value<typename Container::value_type> v (t, i + 1);
            c[i] = v.get ();
        }
        l.pop ();
    }
}

#endif // INCLUDED_LUAPP_IMPL_HPP_
