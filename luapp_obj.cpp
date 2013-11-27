/// luapp_obj.cpp -- cpp wrapper for lua

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#include "luapp.hpp"
#include "luapp_obj.hpp"

#include <lua.h>

using namespace std;

namespace luapp
{
    namespace details
    {
        has_lua::has_lua (const table &parent)
            : l_ (parent.l ())
        {
        }

        void global_env::get_at (const string &name) const
        {
            lua_getglobal (ll (), name.c_str ());
        }

        void global_env::get_at (ssize_t index) const
        {
            throw logic_error ("global enviroment cannot be indexed by "
                               "integers");
        }

        void global_env::set_at (const string &field) const
        {
            lua_setglobal (ll (), field.c_str ());
        }

        void global_env::set_at (ssize_t index) const
        {
            throw logic_error ("global enviroment cannot be indexed by "
                               "integers");
        }
    }

    object::object (const table &parent, const string &name)
        : details::has_lua (parent), env_ (0), parent_ (parent), name_ (name)
    {
    }
    object::object (const table &parent, ssize_t index)
        : details::has_lua (parent), env_ (0), parent_ (parent), index_ (index)
    {
    }

    bool object::is_nil () const
    {
        push ();
        bool res = lua_isnil (ll (), -1);
        pop ();
        return res;
    }

    void object::push () const
    {
        parent_.push ();
        if (name_.empty ())
            parent_.get_at (index_);
        else
            parent_.get_at (name_);
        parent_.dequeue ();
    }

    void object::pop () const
    {
        l_.pop ();
    }

    void object::set () const
    {
        if (name_.empty ())
            parent_.set_at (index_);
        else
            parent_.set_at (name_);
    }

    void table::create (size_t arr, size_t hash) const
    {
        parent_.push ();
        lua_createtable (ll (), arr, hash);
        set ();
        parent_.pop ();
    }

    size_t table::array_size () const
    {
        push ();
        lua_len (ll (), -1);
        size_t n;
        l_ >> n;
        pop ();
        return n;
    }

    deque<string> table::string_keys () const
    {
        deque<string> keys;

        push ();
        lua_pushnil (ll ());
        while (lua_next (ll (), -2)) {
            if (lua_isstring (ll (), -2))
                keys.push_back (lua_tostring (ll (), -2));
            l_.pop ();
        }
        pop ();
        return keys;
    }

    void table::get_at (const string &field) const
    {
        lua_getfield (ll (), -1, field.c_str ());
    }

    void table::get_at (ssize_t index) const
    {
        l_ << index;
        lua_gettable (ll (), -2);
    }

    void table::set_at (const string &field) const
    {
        lua_setfield (ll (), -2, field.c_str ());
    }

    void table::set_at (ssize_t index) const
    {
        l_ << index;
        lua_pushvalue (ll (), -2);
        lua_remove (ll (), -3);
        lua_settable (ll (), -3);
    }

    void function::define (lua_CFunction func) const
    {
        parent_.push ();
        l_ << func;
        set ();
        parent_.pop ();
    }
}
