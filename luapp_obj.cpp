/// luapp_obj.cpp -- cpp wrapper for lua

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#include "luapp.hpp"
#include "luapp_obj.hpp"

#include <iostream>

using namespace std;

namespace luapp
{
    namespace details
    {
        has_lua::has_lua (table_base &t)
            : l_ (t.l ())
        {
        }

        void global_env::extract (const string &name)
        {
            lua_getglobal (ll (), name.c_str ());
        }

        void global_env::extract (ssize_t index)
        {
            throw logic_error ("global enviroment cannot be indexed by "
                               "integers");
        }

        void global_env::set_at (const string &field)
        {
            lua_setglobal (ll (), field.c_str ());
        }

        void global_env::set_at (ssize_t index)
        {
            throw logic_error ("global enviroment cannot be indexed by "
                               "integers");
        }
    }

    void object::push () const
    {
        parent_.push ();
        if (name_.empty ())
            parent_.extract (index_);
        else
            parent_.extract (name_);
        parent_.dequeue ();
    }

    void object::pop () const
    {
        l_.pop ();
    }

    void object::set ()
    {
        if (name_.empty ())
            parent_.set_at (index_);
        else
            parent_.set_at (name_);
    }

    void table::create (size_t arr, size_t hash)
    {
        parent_.push ();
        lua_createtable (ll (), arr, hash);
        set ();
        parent_.pop ();
    }

    size_t table::array_size () const
    {
        push ();
        size_t n = lua_objlen (ll (), -1);
        pop ();
        return n;
    }

    void table::extract (const string &field)
    {
        lua_getfield (ll (), -1, field.c_str ());
    }

    void table::extract (ssize_t index)
    {
        l_ << index;
        lua_gettable (ll (), -2);
    }

    void table::set_at (const string &field)
    {
        lua_setfield (ll (), -2, field.c_str ());
    }

    void table::set_at (ssize_t index)
    {
        l_ << index;
        lua_pushvalue (ll (), -2);
        lua_remove (ll (), -3);
        lua_settable (ll (), -3);
    }

    void function::define (lua_CFunction func)
    {
        parent_.push ();
        l_ << func;
        set ();
        parent_.pop ();
    }
}
