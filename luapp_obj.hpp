/// luapp_obj.hpp -- cpp wrapper for lua objects

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_OBJ_HPP_
#define INCLUDED_LUAPP_OBJ_HPP_ 1

#include <lua.hpp>

#include <cassert>
#include <stdexcept>
#include <string>

namespace luapp
{
    namespace details
    {
        class table_base;
        struct has_lua
        {
        public:
            has_lua (lua &l) : l_ (l) {}
            has_lua (table_base &t);
            virtual ~has_lua () {}
        protected:
            lua_State * &ll () const {return l_.l_;}
        protected:
            mutable lua &l_;
        };
        struct table_base : public virtual has_lua
        {
        public:
            table_base (lua &l) : has_lua (l) {}
        public:
            virtual void push () const = 0;
            virtual void pop () const = 0;
            virtual void dequeue () const = 0;
            virtual void extract (const std::string &field) const = 0;
            virtual void extract (ssize_t index) const = 0;
            virtual void set_at (const std::string &field) const = 0;
            virtual void set_at (ssize_t index) const = 0;
        private:
            friend class has_lua;
            lua &l () const {return l_;}
        };
        struct global_env : public table_base
        {
        public:
            global_env (lua &l) : has_lua (l), table_base (l) {}
        public:
            virtual void push () const {}
            virtual void pop () const {}
            virtual void dequeue () const {}
            virtual void extract (const std::string &field) const;
            virtual void extract (ssize_t index) const;
            virtual void set_at (const std::string &field) const;
            virtual void set_at (ssize_t index) const;
        };
    }

    class object : public virtual details::has_lua
    {
    public:
        object (lua &l, const std::string &name)
            : details::has_lua (l), env_ (new details::global_env (l)),
              parent_ (*env_), name_ (name) {}
        object (const details::table_base &parent, const std::string &name)
            : details::has_lua (parent), env_ (0), parent_ (parent),
              name_ (name) {}
        object (const details::table_base &parent, ssize_t index)
            : details::has_lua (parent), env_ (0), parent_ (parent),
              index_ (index) {}
        virtual ~object () {if (env_) delete env_;}

    public:
        bool is_nil () const;

    protected:
        virtual void push () const;
        virtual void pop () const;
        virtual void set () const;

    private:
        details::table_base *env_;
    protected:
        const details::table_base &parent_;
        std::string name_;
        ssize_t index_;
    };

    class table : public details::table_base, public object
    {
    public:
        table (lua &l, const std::string &name)
            : details::has_lua (l), details::table_base (l), object (l, name) {}
        table (const table &parent, const std::string &name)
            : details::has_lua (parent), details::table_base (l_),
              object (parent, name) {}
        table (const table &parent, ssize_t index)
            : details::has_lua (parent), details::table_base (l_),
              object (parent, index) {}

    public:
        void create (size_t arr = 0, size_t hash = 0) const;
        size_t array_size () const;

    public:
        virtual void push () const {object::push ();}
        virtual void pop () const {object::pop ();}
        virtual void dequeue () const {l_.remove (-2);}
        virtual void extract (const std::string &field) const;
        virtual void extract (ssize_t index) const;
        virtual void set_at (const std::string &field) const;
        virtual void set_at (ssize_t index) const;

    private:
        friend class details::has_lua;
    };

    class function : public object
    {
    public:
        function (lua &l, const std::string &name)
            : details::has_lua (l), object (l, name) {}
        function (const table &parent, const std::string &name)
            : details::has_lua (parent), object (parent, name) {}
        function (const table &parent, ssize_t index)
            : details::has_lua (parent), object (parent, index) {}

    public:
        void define (lua_CFunction func) const;

    public:
        template <typename Result0, typename... Argv>
        Result0 call (Argv&&... argv) const
            {
                invoke (std::forward<Argv> (argv)...);
                return pop_result<Result0> ();
            }

        template <typename... Argv>
        void invoke (Argv&&... argv) const
            {
                if (!l_.empty ()) {
                    throw std::logic_error ("stack not empty before function "
                                            "call");
                }

                push ();
                push_args (std::forward<Argv> (argv)...);
                l_.pcall (sizeof... (Argv));
            }
        size_t pop_results () const {l_.clear (); return 0;}
        template <typename Result0, typename... Results>
        size_t pop_results (Result0 &result0, Results&&... results) const
            {
                if (l_.empty ())
                    return 0;
                const size_t count = sizeof... (results) + 1;
                while (count < l_.top ())
                    l_.pop ();
                const size_t n = l_.top ();
                pop_results (l_.top (), result0,
                             std::forward<Results> (results)...);
                return n;
            }
        template <typename Result0>
        Result0 pop_result () const
            {Result0 r = Result0 (); pop_results (r); return r;}

    private:
        void push_args () const {}
        template <typename Arg0, typename... Argv>
        void push_args (const Arg0 &arg0, Argv&&... argv) const
            {l_ << arg0; push_args (std::forward<Argv> (argv)...);}
        void pop_results (size_t n) const {assert (false);}
        template <typename Result0, typename... Results>
        void pop_results (size_t n, Result0 &result0, Results&&... results)
            const
            {
                assert (n != 0);
                if (n != 1)
                    pop_results (n - 1, std::forward<Results> (results)...);
                l_ >> result0;
            }
    };

    template <typename T>
    class value : public object
    {
    public:
        value (lua &l, const std::string &name)
            : details::has_lua (l), object (l, name) {}
        value (const table &parent, const std::string &name)
            : details::has_lua (parent), object (parent, name) {}
        value (const table &parent, ssize_t index)
            : details::has_lua (parent), object (parent, index) {}

    public:
        T get () const
            {
                push ();
                T v;
                l_ >> v;
                return v;
            }
        void set (const T &v) const
            {
                parent_.push ();
                l_ << v;
                set ();
                parent_.pop ();
            }

    private:
        virtual void set () const {object::set ();}
    };

    class temp_table : public table
    {
    public:
        temp_table (lua &l)
            : details::has_lua (l), table (l, "") {}

    public:
        virtual void push () const {}
        virtual void pop () const {}
        virtual void dequeue () const {}

    private:
        virtual void set () const {}
    };
}

#endif // INCLUDED_LUAPP_OBJ_HPP_

