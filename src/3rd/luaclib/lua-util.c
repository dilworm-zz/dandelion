#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <lua.h>
#include <lauxlib.h>

static int
lget_realtime(lua_State* L) {
    struct timespec ti;
    clock_gettime(CLOCK_REALTIME, &ti);
    lua_newtable(L);
    lua_pushstring(L, "sec");
    lua_pushinteger(L, ti.tv_sec);
    lua_rawset(L, -3);
    lua_pushstring(L, "usec");
    lua_pushinteger(L, ti.tv_nsec/1000);
    lua_rawset(L, -3);
    return 1;
}

int 
luaopen_util_core(lua_State* L) {
    luaL_checkversion(L);
    luaL_Reg l[] = {
        { "get_realtime", lget_realtime },
        {NULL, NULL},
    };

    luaL_newlib(L, l);
    return 1;
} 


