/// luapp_stackops.hpp -- cpp wrapper for lua stack operators (<</>>)

/// Author: Zhang Yichao <echaozh@gmail.com>
/// Created: 2011-08-21
///

#ifndef INCLUDED_LUAPP_STACKOPS_HPP_
#define INCLUDED_LUAPP_STACKOPS_HPP_ 1

#include <lua.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

namespace luapp
{
    class lua;
    lua &operator << (lua &l, bool b);
    lua &operator << (lua &l, ptrdiff_t n);
    template <typename Integral>
    typename std::enable_if<std::is_integral<Integral>::value, lua &>::type
    operator << (lua &l, Integral n);
    lua &operator << (lua &l, double d);
    template <typename FP>
    typename std::enable_if<std::is_floating_point<FP>::value, lua &>::type
    operator << (lua &l, FP f);
    lua &operator << (lua &l, const std::string &s);
    lua &operator << (lua &l, lua_CFunction f);
    // template <typename T, size_t N>
    // lua &operator << (lua &l, const std::array<T, N> &arr);
    template <typename Container>
    typename std::enable_if<std::is_class<Container>::value, lua &>::type
    operator << (lua &l, const Container &c);

    lua &operator >> (lua &l, bool &b);
    lua &operator >> (lua &l, ptrdiff_t &n);
    template <typename Integral>
    typename std::enable_if<std::is_integral<Integral>::value, lua &>::type
    operator >> (lua &l, Integral &n);
    lua &operator >> (lua &l, double &d);
    template <typename FP>
    typename std::enable_if<std::is_floating_point<FP>::value, lua &>::type
    operator >> (lua &l, FP &f);
    lua &operator >> (lua &l, std::string &s);
    template <typename T, size_t N>
    lua &operator >> (lua &l, std::array<T, N> &arr);
    template <typename Container>
    typename std::enable_if<std::is_class<Container>::value, lua &>::type
    operator >> (lua &l, Container &c);
}

#endif // INCLUDED_LUAPP_STACKOPS_HPP_
