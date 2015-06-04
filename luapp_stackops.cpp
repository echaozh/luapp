/// luapp_stackops.cpp -- cpp wrapper for lua stack operators

/// Author: Zhang Yichao <echaozh@gmail.com>
/// Created: 2011-08-21
///

#include "luapp.hpp"

using namespace std;

namespace luapp
{
    lua &operator << (lua &l, bool b)
    {
        lua_pushboolean (l.l (), b);
        return l;
    }
    lua &operator << (lua &l, ptrdiff_t n)
    {
        lua_pushinteger (l.l (), n);
        return l;
    }
    lua &operator << (lua &l, double d)
    {
        lua_pushnumber (l.l (), d);
        return l;
    }
    lua &operator << (lua &l, const string &s)
    {
        lua_pushstring (l.l (), s.c_str ());
        return l;
    }
    lua &operator << (lua &l, lua_CFunction f)
    {
        lua_pushcfunction (l.l (), f);
        return l;
    }

    lua &operator >> (lua &l, bool &b)
    {
        b = lua_toboolean (l.l (), -1);
        l.pop ();
        return l;
    }
    lua &operator >> (lua &l, ptrdiff_t &n)
    {
        n = lua_tointeger (l.l (), -1);
        l.pop ();
        return l;
    }
    lua &operator >> (lua &l, double &d)
    {
        d = lua_tonumber (l.l (), -1);
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
        const char *cp = lua_tostring (l.l (), -1);
        s = cp ?: "";
        l.pop ();
        return l;
    }
}
