#ifdef __linux__
#define _GNU_SOURCE // for strtok_r
#endif
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#include <lua.h> // include lua_xxx function, base api
#include <lauxlib.h> // include luaL_xxx function
#include <lualib.h>

static void print_stack(lua_State *L)
{
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++)
	{
		int type = lua_type(L, i);
		switch (type)
		{
		case LUA_TNIL:
			printf("nil");
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("%s", lua_tostring(L, i));
			break;
		default:
			printf("%s", lua_typename(L, i));
			break;
		}
		printf("  ");
	}
	printf("\n");
} // print_stack end

int test0()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	int flag = 1;
	double d = 3.14;
	int i = 13;
	const char *b1 = "hello";
	const char *b2 = "world";

	// check if has x space in stack
	int ret = lua_checkstack(L, 6);
	printf("ret = %d\n", ret);

	// get num in stack
	int top = lua_gettop(L);
	printf("top = %d\n", top);

	// push into stack
	lua_pushnil(L);
	lua_pushboolean(L, flag);
	lua_pushnumber(L, d);
	lua_pushinteger(L, i);
	lua_pushlstring(L, b1, strlen(b1));
	lua_pushstring(L, b2);

	top = lua_gettop(L);
	printf("top = %d\n", top);

	for (int i = 1; i <= top; i++)
	{
		int type = lua_type(L, i);
		switch (type)
		{
		case LUA_TNIL:
			printf("[%d] nil\n", i);
			break;
		case LUA_TBOOLEAN:
			printf("[%d] boolean: %d\n", i, lua_toboolean(L, i));
			break;
		case LUA_TNUMBER:
			printf("[%d] number: %g\n", i, lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("[%d] string: %s\n", i, lua_tostring(L, i));
			break;
		default:
			printf("[%d] %s\n", i, lua_typename(L, i));
			break;
		}
	}
	printf("\n");

	size_t size = 0;
	for (int i = -1; i >= -top; i--)
	{
		int type = lua_type(L, i);
		switch (type)
		{
		case LUA_TNIL:
			printf("[%d] nil\n", i);
			break;
		case LUA_TBOOLEAN:
			printf("[%d] boolean: %d\n", i, lua_toboolean(L, i));
			break;
		case LUA_TNUMBER:
			printf("[%d] number: %g\n", i, lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("[%d] string: %s\n", i, lua_tolstring(L, i, &size));
			break;
		default:
			printf("[%d] %s\n", i, lua_typename(L, i));
			break;
		}
	}
	printf("\n");

	print_stack(L);
	printf("\n");

	lua_pushvalue(L, -3);
	printf("after pushvalue -3\n");
	print_stack(L);
	printf("\n");

	lua_remove(L, 4);
	printf("after remove 4\n");
	print_stack(L);
	printf("\n");

	lua_insert(L, 2);
	printf("after insert 2\n");
	print_stack(L);
	printf("\n");

	lua_replace(L, -4);
	printf("after replace -4\n");
	print_stack(L);
	printf("\n");

	lua_pop(L, 1);
	printf("after pop 1\n");
	print_stack(L);
	printf("\n");

	lua_settop(L, -3);
	printf("after settop -3\n");
	print_stack(L);
	printf("\n");

	lua_settop(L, 0);
	printf("after settop 0\n");
	print_stack(L);
	printf("top=%d\n", lua_gettop(L));
	printf("\n");

	lua_close(L);
	return 0;
} // test0 end

static void print_rgb_table(lua_State *L, const char *tablename)
{
	do
	{
		lua_getglobal(L, tablename);
		if (!lua_istable(L, -1))
		{
			fprintf(stderr, "%s not a table\n", tablename);
			lua_pop(L, 1);
			break;
		}

		int r,g,b;
		
		lua_pushstring(L, "r");
		lua_gettable(L, -2);
		if (!lua_isnumber(L, -1))
		{
			fprintf(stderr, "%s.%s not a integer\n", tablename, "r");
			lua_pop(L, 1);
			break;
		}
		r = lua_tointeger(L, -1);
		lua_pop(L, 1);
		
		lua_pushstring(L, "g");
		lua_gettable(L, -2);
		if (!lua_isnumber(L, -1))
		{
			fprintf(stderr, "%s.%s not a integer\n", tablename, "g");
			lua_pop(L, 1);
			break;
		}
		g = lua_tointeger(L, -1);
		lua_pop(L, 1);
		
		lua_pushstring(L, "b");
		lua_gettable(L, -2);
		if (!lua_isnumber(L, -1))
		{
			fprintf(stderr, "%s.%s not a integer\n", tablename, "b");
			lua_pop(L, 1);
			break;
		}
		b = lua_tointeger(L, -1);
		lua_pop(L, 1);

		printf("tablename=%s r=%d g=%d b=%d\n", tablename, r, g, b);

		lua_pop(L, 1);

	} while (0);
} // print_rgb_table end

int test1()
{
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		if (luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		// get global element from lua, push into stack
		{
		printf("---- 1\n");
		// get num from lua
		lua_getglobal(L, "width");
		lua_getglobal(L, "height");

		if (!lua_isnumber(L, 1))
		{
			fprintf(stderr, "stack 1 not a number\n");
			break;
		}

		if (!lua_isnumber(L, 2))
		{
			fprintf(stderr, "stack 2 not a number\n");
			break;
		}

		// int width = lua_tointeger(L, -2);
		// int height = lua_tointeger(L, -1);
		int width = lua_tonumber(L, -2);
		int height = lua_tonumber(L, -1);
		printf("width=%d height=%d\n", width, height);

		lua_pop(L, 2);
		printf("\n");
		}

		// get table field from lua
		{
		printf("---- 2\n");
		// 1. get table
		const char *tablename = "background";
		lua_getglobal(L, tablename);
		if (!lua_istable(L, -1))
		{
			fprintf(stderr, "stack -2 not a table\n");
			break;
		}
			
		// 2. push key
		const char *key = "r";
		lua_pushstring(L, key);

		// 3. get value in table
		lua_gettable(L, -2);
		if (!lua_isnumber(L, -1))
		{
			fprintf(stderr, "stack -1 not a number\n");
			break;
		}
		int value = lua_tonumber(L, -1);
		printf("tablename=%s key=%s value=%d\n", tablename, key, value);

		int top = lua_gettop(L);
		printf("top=%d\n", top);

		// 4. pop value
		lua_pop(L, 1);

		//////////////////////////////////////

		// use lua_getfield()
		// table is already in -1
		key = "g";
		lua_getfield(L, -1, key); // same as lua_pushstring() + lua_gettable()
		if (!lua_isnumber(L, -1))
		{
			fprintf(stderr, "stack -1 not a number\n");
			break;
		}
		value = lua_tonumber(L, -1);
		printf("tablename=%s key=%s value=%d\n", tablename, key, value);

		top = lua_gettop(L);
		printf("top=%d\n", top);

		lua_pop(L, 1);

		// clean stack
		lua_settop(L, 0);
		printf("\n");
		}

		// get table field from lua
		{
		printf("---- 3\n");
		// 1. get table
		const char *tablename = "background2";
		lua_getglobal(L, tablename);
		if (!lua_istable(L, -1))
		{
			fprintf(stderr, "stack -2 not a table\n");
			break;
		}
			
		// 2. push key
		const char *key = "r";
		lua_pushstring(L, key);

		// 3. get value
		lua_gettable(L, -2);
		if (!lua_isnumber(L, -1))
		{
			fprintf(stderr, "stack -1 not a number\n");
			break;
		}
		int value = lua_tonumber(L, -1);
		printf("tablename=%s key=%s value=%d\n", tablename, key, value);

		// 4. pop value
		lua_pop(L, 1);

		lua_settop(L, 0);
		printf("\n");
		}

		// set table field in lua
		{
		printf("---- 4\n");
		// 1. get table from global
		const char *tablename = "background";
		lua_getglobal(L, tablename);
		if (!lua_istable(L, -1))
		{
			fprintf(stderr, "stack -2 not a table\n");
			break;
		}
			
		// 2. push key
		const char *key = "r";
		lua_pushstring(L, key);
			
		// 3. push value
		int value = 100;
		lua_pushnumber(L, value);

		// 4. set kv in table, table in -3 now
		lua_settable(L, -3);

		int top = lua_gettop(L);
		printf("top=%d\n", top);
		printf("\n");

		//////////////////////////////////////

		// use lua_setfield()
		// table is already in -1
		// 1. push value first
		value = 200;
		lua_pushnumber(L, value);

		// 2. set field
		key = "g";
		lua_setfield(L, -2, key); // same as lua_pushstring(key) + lua_insert(-2) + lua_settable()

		//////////////////////////////////////

		key = "r";
		lua_pushstring(L, key);
		lua_gettable(L, -2);
		value = lua_tonumber(L, -1);
		printf("tablename=%s key=%s value=%d\n", tablename, key, value);
		lua_pop(L, 1);

		key = "g";
		lua_getfield(L, -1, key);
		value = lua_tonumber(L, -1);
		printf("tablename=%s key=%s value=%d\n", tablename, key, value);
		lua_pop(L, 1);


		// clean stack
		lua_settop(L, 0);
		printf("\n");
		}

		// create a table in lua
		{
		printf("---- 5\n");
		const char *tablename = "background3";
		// 1. new a table 
		lua_newtable(L);
		// 2. set kv
		const char *key = "r"; int value = 20;
		lua_pushnumber(L, value);
		lua_setfield(L, -2, key);
		key = "g"; value = 30;
		lua_pushnumber(L, value);
		lua_setfield(L, -2, key);
		key = "b"; value = 40;
		lua_pushnumber(L, value);
		lua_setfield(L, -2, key);
		// 3. set table in lua
		lua_setglobal(L, tablename);

		int top = lua_gettop(L);
		printf("top=%d\n", top);

		print_rgb_table(L, "background");
		print_rgb_table(L, "background2");
		print_rgb_table(L, "background3");
		print_rgb_table(L, "background4");

		}

	} while (0);

	lua_close(L);

	return 0;
} // test1 end

int test2()
{
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		if (luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		// call lua function
		{
		const char *funcname = "func_t2";
		double x = 20, y = 50;
		double ret;

		// 1. get function
		lua_getglobal(L, funcname);

		// 2. push param
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);

		// 3. protect call
		if (lua_pcall(L, 2, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		
		ret = lua_tonumber(L, -1);
		printf("ret=%g\n", ret);

		// after pcall, fucntion and input params will be removed from stack
		int top = lua_gettop(L);
		printf("top=%d\n", top);

		lua_pop(L, 1);
		}


	} while (0);

	lua_close(L);

	return 0;
} // test2 end

// function for lua
static int l_sin(lua_State *L)
{
	// 1. get param from lua_State
	// double d = lua_tonumber(L, 1);
	double d = luaL_checknumber(L, 1); // if not a number, throw error msg. else return number

	// 2. push result into stack
	lua_pushnumber(L, sin(d));

	// 3. return result count
	return 1;
}

int test3()
{
	// register c function to lua, and lua call it
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		// 1. push c function into lua
		lua_pushcfunction(L, l_sin);
		// 2. name it
		lua_setglobal(L, "mysin");


		lua_getglobal(L, "func_t3"); // this function will call c func
		lua_pushnumber(L, 3.1415926/6);
		if (lua_pcall(L, 1, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		double result = lua_tonumber(L, -1);
		printf("result=%g\n", result);


	} while (0);

	lua_close(L);

	return 0;
} // test3 end


static int cfunc_4add(lua_State *L)
{
	double d1 = luaL_checknumber(L, 1);
	double d2 = luaL_checknumber(L, 2);
	lua_pushnumber(L, d1+d2);
	return 1;
}

static int cfunc_4sub(lua_State *L)
{
	double d1 = luaL_checknumber(L, 1);
	double d2 = luaL_checknumber(L, 2);
	lua_pushnumber(L, d1-d2);
	return 1;
}

static int cfunc_4times(lua_State *L)
{
	double d1 = luaL_checknumber(L, 1);
	double d2 = luaL_checknumber(L, 2);
	lua_pushnumber(L, d1*d2);
	return 1;
}

static int cfunc_4div(lua_State *L)
{
	double d1 = luaL_checknumber(L, 1);
	double d2 = luaL_checknumber(L, 2);
	lua_pushnumber(L, d1/d2);
	return 1;
}

// cfunction list
static const struct luaL_Reg cfunc4_list [] =
{
	{"add", cfunc_4add} // table key and function
,	{"sub", cfunc_4sub}
,	{"times", cfunc_4times}
,	{"div", cfunc_4div}
,	{NULL, NULL} // must null end
};

int test4()
{
	// register list of c function to lua, and lua call it
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		lua_getglobal(L, "_VERSION");
		const char * pv = lua_tostring(L, -1);
		char version[50];
		snprintf(version, sizeof(version), "%s", pv);
		lua_pop(L, 1);
		printf("version=%s\n", version);

		const char *libname = "cfunc4";
		if (strcmp(version, "Lua 5.1") == 0)
		{
			// register c function list as a table into lua, table will stay in stack
			luaL_register(L, libname, cfunc4_list);
		}

		int top = lua_gettop(L);
		printf("top=%d\n", top);


		double x, y, result;
		const char *op;

		x = 10, y = 20;
		op = "add";
		lua_getglobal(L, "func_t4"); // this function will call c func
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushstring(L, op);
		if (lua_pcall(L, 3, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		result = lua_tonumber(L, -1);
		printf("x=%g y=%g op=%s result=%g\n", x, y, op, result);
		lua_pop(L, 1);

		x = 10, y = 20;
		op = "sub";
		lua_getglobal(L, "func_t4"); // this function will call c func
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushstring(L, op);
		if (lua_pcall(L, 3, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		result = lua_tonumber(L, -1);
		printf("x=%g y=%g op=%s result=%g\n", x, y, op, result);
		lua_pop(L, 1);

		x = 10, y = 20;
		op = "times";
		lua_getglobal(L, "func_t4"); // this function will call c func
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushstring(L, op);
		if (lua_pcall(L, 3, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		result = lua_tonumber(L, -1);
		printf("x=%g y=%g op=%s result=%g\n", x, y, op, result);
		lua_pop(L, 1);

		x = 10, y = 20;
		op = "div";
		lua_getglobal(L, "func_t4"); // this function will call c func
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushstring(L, op);
		if (lua_pcall(L, 3, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		result = lua_tonumber(L, -1);
		printf("x=%g y=%g op=%s result=%g\n", x, y, op, result);
		lua_pop(L, 1);

		top = lua_gettop(L);
		printf("top=%d\n", top);

		// error test
		op = "add";
		lua_getglobal(L, "func_t4");
		lua_pushstring(L, "a");
		lua_pushstring(L, "b");
		lua_pushstring(L, op);
		if (lua_pcall(L, 3, 1, 0) != 0)
		{
			fprintf(stderr, "normal pcall error: %s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		result = lua_tonumber(L, -1);
		printf("op=%s result=%g\n", op, result);
		lua_pop(L, 1);

	} while (0);

	lua_close(L);

	return 0;
} // test4 end

static void print_array(lua_State *L)
{
	if (!lua_istable(L, -1))
	{
		fprintf(stderr, "-1 not a table\n");
		return;
	}

	int size = lua_objlen(L, -1);
	for (int i = 1; i <= size; i++)
	{
		lua_pushnumber(L, i);
		lua_gettable(L, -2);

		int type = lua_type(L, -1);
		switch (type)
		{
		case LUA_TNIL:
			printf("nil");
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(L, -1) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(L, -1));
			break;
		case LUA_TSTRING:
			printf("%s", lua_tostring(L, -1));
			break;
		default:
			printf("%s", lua_typename(L, -1));
			break;
		}
		printf("  ");
		lua_pop(L, 1);
	}
	printf("\n");
}

int test5()
{
	int ret;
	ret = 0;
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// get and set from a table array
	do
	{
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		const char *tablename = "test5array";
		lua_getglobal(L, tablename);
		if (!lua_istable(L, -1))
		{
			fprintf(stderr, "%s not a table\n", tablename);
			lua_pop(L, 1);
			break;
		}
		int size = lua_objlen(L, -1);
		printf("size=%d\n", size);
		print_array(L);

		int index = 1;
		lua_pushinteger(L, index);
		lua_rawget(L, -2);
		if (!lua_isstring(L, -1))
		{
			fprintf(stderr, "-1 not a string\n");
			lua_pop(L, 1);
			break;
		}
		const char *value = lua_tostring(L, -1);
		printf("[%d]value=%s\n", index, value);
		lua_pop(L, 1);

		index = 2;
		lua_rawgeti(L, -1, index);
		if (!lua_isstring(L, -1))
		{
			fprintf(stderr, "-1 not a string\n");
			lua_pop(L, 1);
			break;
		}
		value = lua_tostring(L, -1);
		printf("[%d]value=%s\n", index, value);
		lua_pop(L, 1);

		// set into array
		lua_pushstring(L, "hello");
		lua_rawseti(L, -2, size+1);
		print_array(L);

	} while (0);

	lua_close(L);

	return 0;
} // test5 end

int test6()
{
	int ret;
	ret = 0;
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// handle string
	do
	{
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		const char *tablename = "test6array";
		lua_newtable(L);

		int index = 1;
		const char *v1 = "qwerty123456";
		lua_pushlstring(L, v1+2, 5);
		lua_rawseti(L, -2, index++);

		lua_pushfstring(L, "%s-%d", "masha", time(NULL));
		lua_rawseti(L, -2, index++);

		print_array(L);

		lua_setglobal(L, tablename);

	} while (0);

	lua_close(L);

	return 0;
} // test6 end


static int str_upper(lua_State *L)
{
	size_t l;
	size_t i;

	// lua string buffer
	luaL_Buffer b;

	// get input string
	const char *s = luaL_checklstring(L, 1, &l);

	// init lua string buffer
	luaL_buffinit(L, &b);

	for (i = 0; i < l; i++)
	{
		// add char into buffer
		luaL_addchar(&b, toupper((unsigned char)(s[i])));
	}

	// push buffer into stack
	luaL_pushresult(&b);

	return 1;
}

int test7()
{
	int ret;
	ret = 0;
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		lua_pushcfunction(L, str_upper);
		lua_setglobal(L, "c_str_upper");


		lua_getglobal(L, "func_t7");
		if (!lua_isfunction(L, -1))
		{
			fprintf(stderr, "%s is not a function\n", "func_t7");
			lua_pop(L, 1);
			break;
		}

		const char *buffer = "Hello World 123";
		lua_pushstring(L, buffer);
		if (lua_pcall(L, 1, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		const char *result = lua_tostring(L, -1);
		printf("result=%s\n", result);

	} while (0);

	lua_close(L);

	return 0;
} // test7 end

int test_tmp()
{
	/*
	int ret;
	ret = 0;
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

	} while (0);

	lua_close(L);
	*/

	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] = 
{
	test0
,	test1
,	test2
,	test3
,	test4
,	test5
,	test6
,	test7
};

int main(int argc, char **argv) 
{
	int ret;
	int testcase;
	int maxcase;

	ret = 0;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;
	if (argc > 1) {
		// run all test case
		if (!strcmp(argv[1], "all")) {
			printf("stu:run_all_case\n");
			for (int i=0; i<maxcase; i++) {
				printf("stu:RUN testcase[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0) {
					printf("stu:case[%d] %d\n", i, ret);	
					return 0;
				}
			}
			return 0;
		}

		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) {
			testcase = maxcase - 1;
		}
	}

	printf("stu:RUN testcase[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0) {
		printf("stu:case[%d] %d\n", testcase, ret);	
	}

	return ret;
}
