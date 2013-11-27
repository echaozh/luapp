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
        lua () : l_ (luaL_newstate ()), close_ (true) {
            if (!l_) throw std::bad_alloc ();
        }
        lua (lua_State *l) : l_ (l), close_ (false) {}
        ~lua () {if (close_) lua_close (l_);}

    public:
        bool empty () const {return !top ();}
        size_t top () const {return lua_gettop (l ());}
        void pop (size_t n = 1) {lua_pop (l (), n);}
        void clear () {lua_settop (l (), 0);}
        void remove (ssize_t n) {lua_remove (l (), n);}

    public:
#define loadlib_defun(name) void load_##name () {load_lib (&luaopen_##name);}
        loadlib_defun (base);
        loadlib_defun (package);
        loadlib_defun (string);
        loadlib_defun (table);
        loadlib_defun (math);
        loadlib_defun (io);
        loadlib_defun (os);
        loadlib_defun (debug);
#undef loadlib_defun
        void load_safe_libs ();
        void load_libs ();
        void load (const char *s); // also runs the loaded chunk
        operator lua_State *() const {return l_;}

    public:
        size_t memory_consumption ();
        void collect_garbage ();

    private:
        friend lua &operator << (lua &l, bool b);
        friend lua &operator << (lua &l, ptrdiff_t n);
        friend lua &operator << (lua &l, double n);
        friend lua &operator << (lua &l, const std::string &s);
        friend lua &operator << (lua &l, lua_CFunction f);
        friend lua &operator >> (lua &l, bool &b);
        friend lua &operator >> (lua &l, ptrdiff_t &n);
        friend lua &operator >> (lua &l, double &s);
        friend lua &operator >> (lua &l, std::string &s);

    private:
        lua_State *l () const {return l_;}
        void pcall (size_t argc);
        void load_lib (lua_CFunction f);

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
