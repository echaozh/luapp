#include <luapp/luapp.hpp>

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

static int cpp_func (lua_State *ll)
{
    luapp::lua l (ll);
    double a, b;
    l >> a >> b;
    l << (a >= b ? a : b);
    return 1;
}

int main () {
    luapp::lua l;
    l.load_libs ();
    l.load ("print('testing luapp')");

    luapp::table t (l, "t");
    t.create ();

    l.load ("io.write('testing getting/setting field to table ...')");
    luapp::value<int> (t, "v").set (1);
    assert (luapp::value<int> (t, "v").get () == 1);
    l.load ("print(' \033[01;32mpassed\033[0m')");

    l.load ("io.write('testing getting/setting array entry to table ...')");
    luapp::value<string> (t, 2).set ("2");
    assert (luapp::value<string> (t, 2).get () == "2");
    l.load ("print(' \033[01;32mpassed\033[0m')");

    l.load ("io.write('testing defining/calling c++ functions ...')");
    luapp::function (t, "f").define (&cpp_func);
    assert (luapp::function (t, "f").call<int> (2.0, 3) == 3);
    l.load ("print(' \033[01;32mpassed\033[0m')");

    l.load ("print('all tests passed :-)')");
    return 0;
}
