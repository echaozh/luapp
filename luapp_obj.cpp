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
        void pseudo_table::extract (ssize_t index)
        {
            throw logic_error ("global/local enviroment cannot be indexed by "
                               "integers");
        }

        void pseudo_table::set_at (ssize_t index)
        {
            throw logic_error ("global/local enviroment cannot be indexed by "
                               "integers");
        }

        void global_env::extract (const string &name)
        {
            lua_getglobal (l (), name.c_str ());
        }

        void global_env::set_at (const string &field)
        {
            lua_setglobal (l (), field.c_str ());
        }

        void local_env::extract (const string &name)
        {
            throw logic_error ("local enviroment cannot be indexed");
        }

        void local_env::set_at (const string &field)
        {
            throw logic_error ("local enviroment cannot be indexed");
        }
    }

    // void object::make_global (const string &name)
    // {
    //     if (!local_)
    //         throw logic_error ("only local objects can be made global");
    //     if (l_.top () != 1)
    //         throw logic_error ("don't know where we are on stack");

    //     lua_setglobal (l (), name.c_str ());
    //     // NOTE: if we fail to alloc the name, there will be a stray global var
    //     //       but it does no bad
    //     name_ = name;
    //     // NOTE: if we fail to alloc the env, the name itself is not looked at
    //     details::table_base *env = new details::global_env (l_);
    //     // NOTE: these never fail
    //     delete (env_);
    //     parent_ = env_ = env;
    //     local_ = false;
    // }

    void object::push ()
    {
        if (local_)
            // already on stack, do nothing
            ;
        else if (name_.empty ())
            parent_->extract (index_);
        else
            parent_->extract (name_);
    }

    void object::pop ()
    {
        if (!local_)
            l_.pop ();
    }

    void object::set ()
    {
        if (local_)
            // nowhere to set, do nothing
            ;
        else if (name_.empty ())
            parent_->set_at (index_);
        else
            parent_->set_at (name_);
    }

    void table::create (size_t arr, size_t hash)
    {
        parent_->push ();
        lua_createtable (l (), arr, hash);
        set ();
        parent_->pop ();
    }

    void table::extract (const string &field)
    {
        push ();
        lua_getfield (l (), -1, field.c_str ());
        lua_replace (l (), -2);
    }

    void table::extract (ssize_t index)
    {
        push ();
        l_ << index;
        lua_gettable (l (), -2);
        lua_replace (l (), -2);
    }

    void table::set_at (const string &field)
    {
        lua_setfield (l (), -2, field.c_str ());
    }

    void table::set_at (ssize_t index)
    {
        l_ << index;
        lua_pushvalue (l (), -2);
        lua_remove (l (), -3);
        lua_settable (l (), -3);
    }

    void function::define (lua_CFunction func)
    {
        parent_->push ();
        l_ << func;
        set ();
        parent_->pop ();
    }
}
