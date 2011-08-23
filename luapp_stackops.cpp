/// luapp_stackops.cpp -- cpp wrapper for lua stack operators

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#include "luapp.hpp"

using namespace std;

namespace luapp
{
    lua &operator << (lua &l, ptrdiff_t n)
    {
        l.push_int (n);
        return l;
    }
    lua &operator << (lua &l, lua_CFunction f)
    {
        l.push_func (f);
        return l;
    }

    lua &operator >> (lua &l, int32_t &n)
    {
        n = l.dequeue_int ();
        return l;
    }
    lua &operator >> (lua &l, uint32_t &n)
    {
        n = l.dequeue_int ();
        return l;
    }
    lua &operator >> (lua &l, std::string &s)
    {
        s = l.dequeue_str ();
        return l;
    }
}
