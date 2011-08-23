/// luapp_stackops.hpp -- cpp wrapper for lua stack operators (<</>>)

/// Copyright (c) 2011 Vobile. All rights reserved.
/// Author: Zhang Yichao <zhang_yichao@vobile.cn>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_STACKOPS_HPP_
#define INCLUDED_LUAPP_STACKOPS_HPP_ 1

#include <lua.hpp>

#include <cstddef>
#include <cstdint>
#include <string>

namespace luapp
{
    class lua;
    lua &operator << (lua &l, ptrdiff_t n);
    lua &operator << (lua &l, lua_CFunction f);

    lua &operator >> (lua &l, int32_t &n);
    lua &operator >> (lua &l, uint32_t &n);
    lua &operator >> (lua &l, std::string &s);
}

#endif // INCLUDED_LUAPP_STACKOPS_HPP_
