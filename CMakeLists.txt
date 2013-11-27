cmake_minimum_required (VERSION 2.6)
project (luapp)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
include (FindLua52.cmake)

add_library (luapp luapp.cpp luapp_stackops.cpp luapp_obj.cpp)

add_definitions (-std=c++0x -Wall -Werror -D_GNU_SOURCE)

find_package (Lua52 REQUIRED)

include_directories (${LUA_INCLUDE_DIR})
target_link_libraries (luapp ${LUA_LIBRARIES})

install (TARGETS luapp ARCHIVE DESTINATION lib)
install (FILES luapp.hpp luapp_stackops.hpp luapp_obj.hpp luapp_impl.hpp
  DESTINATION include/luapp)
