/// luapp.hpp -- cpp wrapper for lua

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_HPP_
#define INCLUDED_LUAPP_HPP_ 1

#include "luapp_stackops.hpp"

#include <lua.hpp>

#include <cstdint>
#include <stdexcept>
#include <string>

namespace luapp
{
    namespace details {struct has_lua;}
    class function;
    class lua
    {
    public:
        lua () : l_ (luaL_newstate ()), close_ (true)
            {if (!l_) throw std::bad_alloc ();}
        lua (lua_State *l) : l_ (l), close_ (false) {}
        ~lua () {if (close_) lua_close (l_);}

    public:
        bool empty () const {return !top ();}
        size_t top () const {return lua_gettop (l_);}
        void pop (size_t n = 1) {lua_pop (l_, n);}
        void clear () {lua_settop (l_, 0);}
        void remove (ssize_t n) {lua_remove (l_, n);}

    public:
        void load_base () {luaopen_base (l_);}
        void load (const char *s); // also runs the loaded chunk

    private:
        friend lua &operator << (lua &l, ptrdiff_t n);
        friend lua &operator << (lua &l, double n);
        friend lua &operator << (lua &l, lua_CFunction f);
        friend lua &operator >> (lua &l, int32_t &n);
        friend lua &operator >> (lua &l, uint32_t &n);
        friend lua &operator >> (lua &l, std::string &s);
        void push_int (ptrdiff_t n) {lua_pushinteger (l_, n);}
        void push_number (ptrdiff_t n) {lua_pushnumber (l_, n);}
        void push_func (lua_CFunction f) {lua_pushcfunction (l_, f);}
        ptrdiff_t pop_int ()
            {ptrdiff_t n = lua_tointeger (l_, -1); pop (); return n;}
        std::string pop_str ();

    private:
        void pcall (size_t argc);

    public:
        friend class details::has_lua;
        friend class function;
        mutable lua_State *l_;
        const bool close_;
    };
}

#include "luapp_obj.hpp"
#include "luapp_impl.hpp"

#endif // INCLUDED_LUAPP_HPP_
