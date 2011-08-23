/// luapp_obj.hpp -- cpp wrapper for lua objects

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_OBJ_HPP_
#define INCLUDED_LUAPP_OBJ_HPP_ 1

#include <lua.hpp>

#include <stdexcept>
#include <string>

namespace luapp
{
    namespace details
    {
        struct has_lua
        {
        public:
            has_lua (lua &l) : l_ (l) {}
            virtual ~has_lua () {}
        protected:
            lua_State * &l () const {return l_.l_;}
        protected:
            mutable lua &l_;
        };
        struct table_base : public virtual has_lua
        {
        public:
            table_base (lua &l) : has_lua (l) {}
        public:
            virtual void push () = 0;
            virtual void pop () = 0;
            virtual void extract (const std::string &field) = 0;
            virtual void extract (ssize_t index) = 0;
            virtual void set_at (const std::string &field) = 0;
            virtual void set_at (ssize_t index) = 0;
        };
        struct pseudo_table : public table_base
        {
        public:
            pseudo_table (lua &l) : has_lua (l), table_base (l) {}
        public:
            virtual void push () {}
            virtual void pop () {}
            virtual void extract (ssize_t index);
            virtual void set_at (ssize_t index);
        };
        struct global_env : public pseudo_table
        {
        public:
            global_env (lua &l) : has_lua (l), pseudo_table (l) {}
        public:
            virtual void extract (const std::string &field);
            virtual void set_at (const std::string &field);
        };
        struct local_env : public pseudo_table
        {
        public:
            local_env (lua &l) : has_lua (l), pseudo_table (l) {}
        public:
            virtual void extract (const std::string &field);
            virtual void set_at (const std::string &field);
        };
    }

    class object : public virtual details::has_lua
    {
    public:
        object (lua &l)
            : details::has_lua (l), env_ (new details::local_env (l)),
              parent_ (env_), local_ (true) {}
        object (lua &l, const std::string &name)
            : details::has_lua (l), env_ (new details::global_env (l)),
              parent_ (env_), local_ (false), name_ (name) {}
        object (lua &l, details::table_base &parent, const std::string &name)
            : details::has_lua (l), env_ (0), parent_ (&parent), local_ (false),
              name_ (name) {}
        object (lua &l, details::table_base &parent, ssize_t index)
            : details::has_lua (l), env_ (0), parent_ (&parent), local_ (false),
              index_ (index)
            {}
        virtual ~object () {if (env_) delete env_;}

    protected:
        virtual void push ();
        virtual void pop ();
        void set ();

    private:
        details::table_base *env_;
    protected:
        details::table_base *parent_;
        bool local_;
        std::string name_;
        ssize_t index_;
    };

    class table : public details::table_base, public object
    {
    public:
        table (lua &l)
            : details::has_lua (l), details::table_base (l), object (l) {}
        table (lua &l, const std::string &name)
            : details::has_lua (l), details::table_base (l), object (l, name) {}
        table (lua &l, table &parent, const std::string &name)
            : details::has_lua (l), details::table_base (l),
              object (l, parent, name) {}
        table (lua &l, table &parent, ssize_t index)
            : details::has_lua (l), details::table_base (l),
              object (l, parent, index) {}

    public:
        void create (size_t arr = 0, size_t hash = 0);

    public:
        virtual void push () {object::push ();}
        virtual void pop () {object::pop ();}
        virtual void extract (const std::string &field);
        virtual void extract (ssize_t index);
        virtual void set_at (const std::string &field);
        virtual void set_at (ssize_t index);
    };

    class function : public object
    {
    public:
        function (lua &l, const std::string &name)
            : details::has_lua (l), object (l, name) {}
        function (lua &l, table &parent, const std::string &name)
            : details::has_lua (l), object (l, parent, name) {}
        function (lua &l, table &parent, ssize_t index)
            : details::has_lua (l), object (l, parent, index) {}

    public:
        void define (lua_CFunction func);

    public:
        template <typename Result0, typename... Argv>
        Result0 call (Argv&&... argv)
            {
                invoke (std::forward<Argv> (argv)...);
                return pop_result<Result0> ();
            }

        template <typename... Argv>
        void invoke (Argv&&... argv)
            {
                if (local_ && l_.top () != 1) {
                    throw std::logic_error ("local function is not at stack "
                                            "bottom");
                } else if (!local_ && !l_.empty ()) {
                    throw std::logic_error ("stack not empty before function "
                                            "call");
                }

                push ();
                push_args (std::forward<Argv> (argv)...);
                l_.pcall (sizeof... (Argv));
            }
        size_t pop_results () {l_.clear (); return 0;}
        template <typename Result0, typename... Results>
        size_t pop_results (Result0 &result0, Results&&... results)
            {
                return dequeue_result (result0)
                    ? 1 + pop_results (std::forward<Results> (results)...)
                    : 0;
            }
        template <typename Result0>
        Result0 pop_result () {Result0 r; pop_results (r); return r;}

    private:
        void push_args () {}
        template <typename Arg0, typename... Argv>
        void push_args (const Arg0 &arg0, Argv&&... argv)
            {l_ << arg0; push_args (std::forward<Argv> (argv)...);}
        template <typename Result>
        bool dequeue_result (Result &result)
            {
                if (l_.empty ())
                    return false;
                l_ >> result;
                return true;
            }
    };

    template <typename T>
    class value : public object
    {
    public:
        value (lua &l, const std::string &name)
            : details::has_lua (l), object (l, name) {}
        value (lua &l, table &parent, const std::string &name)
            : details::has_lua (l), object (l, parent, name) {}
        value (lua &l, table &parent, ssize_t index)
            : details::has_lua (l), object (l, parent, index) {}

    public:
        T get () {push (); T v; l_ >> v; return v;}
        void set (const T &v)
            {
                parent_->push ();
                l_ << v;
                set ();
                parent_->pop ();
            }

    private:
        void set () {object::set ();}
    };
}

#endif // INCLUDED_LUAPP_OBJ_HPP_

