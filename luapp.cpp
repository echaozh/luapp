/// luapp.cpp -- cpp wrapper for lua

/// Author: Zhang Yichao <echaozh@gmail.com>
/// Created: 2011-08-21
///

#include "luapp.hpp"

using namespace std;

namespace luapp
{
    void lua::load (const char *s) {
        int ret;
        if ((ret = luaL_loadstring (l (), s))) {
            if (ret == LUA_ERRSYNTAX)
                throw std::invalid_argument ("bad chunk of lua code");
            else
                throw std::bad_alloc ();
        }
        pcall (0);
        clear ();
    }

    void lua::load_lib (lua_CFunction f)
    {
        *this << f;
        pcall (0);
    }

    void lua::load_safe_libs ()
    {
        load_base ();
        load_package ();
        load_string ();
        load_table ();
        load_math ();
    }

    void lua::load_libs ()
    {
        luaL_openlibs (l ());
    }

    void lua::pcall (size_t argc)
    {
        int ret;
        if ((ret = lua_pcall (l (), argc, LUA_MULTRET, 0))) {
            std::string msg;
            *this >> msg;
            if (ret == LUA_ERRRUN)
                throw std::runtime_error (msg);
            else
                throw std::bad_alloc ();
        }
    }

    size_t lua::memory_consumption () {
        return lua_gc (l (), LUA_GCCOUNT, 0);
    }

    void lua::collect_garbage () {
        lua_gc (l (), LUA_GCCOLLECT, 0);
    }
}
