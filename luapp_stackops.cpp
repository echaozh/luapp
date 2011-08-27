/// luapp_stackops.cpp -- cpp wrapper for lua stack operators

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#include "luapp.hpp"

#include <iostream>

using namespace std;

namespace luapp
{
    lua &operator << (lua &l, bool b)
    {
        lua_pushboolean (l.l_, b);
        return l;
    }
    lua &operator << (lua &l, ptrdiff_t n)
    {
        lua_pushinteger (l.l_, n);
        return l;
    }
    lua &operator << (lua &l, double d)
    {
        lua_pushnumber (l.l_, d);
        return l;
    }
    lua &operator << (lua &l, const string &s)
    {
        lua_pushstring (l.l_, s.c_str ());
        return l;
    }
    lua &operator << (lua &l, lua_CFunction f)
    {
        lua_pushcfunction (l.l_, f);
        return l;
    }

    lua &operator >> (lua &l, bool &b)
    {
        b = lua_toboolean (l.l_, -1);
        l.pop ();
        return l;
    }
    lua &operator >> (lua &l, ptrdiff_t &n)
    {
        n = lua_tointeger (l.l_, -1);
        l.pop ();
        return l;
    }
    lua &operator >> (lua &l, double &d)
    {
        d = lua_tonumber (l.l_, -1);
        l.pop ();
        return l;
    }
    lua &operator >> (lua &l, float &f)
    {
        double d;
        l >> d;
        f = d;
        return l;
    }
    lua &operator >> (lua &l, std::string &s)
    {
        const char *cp = lua_tostring (l.l_, -1);
        s = cp ?: "";
        l.pop ();
        return l;
    }
}
