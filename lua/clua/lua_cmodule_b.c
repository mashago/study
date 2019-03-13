#include "stdlib.h"
#include "lua.h"
#include "lauxlib.h"

static const char *METATABLE_NAME = "cmodule_b";
typedef struct st {
    int a;
} st;

static int _call(lua_State *L)
{
    st **ppst = (st**)luaL_checkudata(L, 1, METATABLE_NAME);
    printf("a=%d\n", (*ppst)->a);
    return 0;
}

static int _new(lua_State *L)
{
    st *pst = malloc(sizeof(st));
    pst->a = 20;

    st **ppst = (st**)lua_newuserdata(L, sizeof(st*));
    *ppst = pst;

    luaL_getmetatable(L, METATABLE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

int luaopen_cmodule_b(lua_State *L)
{
    luaL_checkversion(L);

    luaL_Reg l[] = 
    {
        {"call", _call},
        {NULL, NULL}
    };
    luaL_newmetatable(L, METATABLE_NAME);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, l, 0);

    lua_pushcclosure(L, _new, 0);
    
    return 1;
}

