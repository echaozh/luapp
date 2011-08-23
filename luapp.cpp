/// luapp.cpp -- cpp wrapper for lua

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#include "luapp.hpp"

using namespace std;

namespace luapp
{
    void lua::load (const char *s)
    {
        int ret;
        if ((ret = luaL_loadstring (l_, s))) {
            if (ret == LUA_ERRSYNTAX)
                throw std::invalid_argument ("bad chunk of lua code");
            else
                throw std::bad_alloc ();
        }
        pcall (0);
        clear ();
    }

    void lua::pcall (size_t argc)
    {
        int ret;
        if ((ret = lua_pcall (l_, argc, LUA_MULTRET, 0))) {
            std::string msg;
            *this >> msg;
            if (ret == LUA_ERRRUN)
                throw std::runtime_error (msg);
            else
                throw std::bad_alloc ();
        }
    }

    string lua::dequeue_str ()
    {
        const char *cp = lua_tostring (l_, 1);
        string s = cp ?: "";
        dequeue ();
        return s;
    }
}
