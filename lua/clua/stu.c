#ifdef __linux__
#define _GNU_SOURCE // for strtok_r
#endif
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#include <lua.h> // include lua_xxx function, base api
#include <lauxlib.h> // include luaL_xxx function
#include <lualib.h>
#include <luaconf.h>

static void print_stack(lua_State *L)
{
	/*
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
	*/
	int top = lua_gettop(L);
	printf("top=%d\n", top);
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
			printf("[%d] string: %s\n", i, lua_tostring(L, i));
			break;
		default:
			printf("[%d] %s\n", i, lua_typename(L, i));
			break;
		}
	}
	printf("\n");
} // print_stack end

int test0()
{
	lua_State *L = luaL_newstate(); // open lua
	luaL_openlibs(L); // open standard lib

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

	/*
	 * 		  top
	 *		-1 | 6
	 *		-2 | 5
	 *		-3 | 4
	 *		-4 | 3
	 *		-5 | 2
	 *		-6 | 1
	 * 		bottom
	 */

	top = lua_gettop(L);
	printf("top = %d\n", top);

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

	for (int i = top; i >= 1; i--)
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

	print_stack(L);
	printf("\n");

	// copy target value and push top
	lua_pushvalue(L, -3);
	printf("after pushvalue -3\n");
	print_stack(L);
	printf("\n");

	// remove target pos value
	lua_remove(L, -4);
	printf("after remove -4\n");
	print_stack(L);
	printf("\n");

	// pop top value, insert to target pos
	lua_insert(L, -5);
	printf("after insert -5\n");
	print_stack(L);
	printf("\n");

	// pop top value, set to target pos
	lua_replace(L, -4);
	printf("after replace -4\n");
	print_stack(L);
	printf("\n");

	// pop num value
	lua_pop(L, 2);
	printf("after pop 2\n");
	print_stack(L);
	printf("\n");

	// set new top, if now top < new top, will push nil. or pop value to match new top
	lua_settop(L, -2);
	printf("after settop -2\n");
	print_stack(L);
	printf("\n");

	// set top 0 will clear stack
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

static void register_cmodule(lua_State *L, const char *module_name, const struct luaL_Reg *cfuncs)
{
	#ifndef __LUA_5_2
	{
		// for lua5.1
		// register c function array as a function table into lua global environment, table will stay in stack
		luaL_register(L, module_name, cfuncs);
	}
	#else
	{
		// for lua5.2+, register c module, just like new a table and set functions into the table, and name it
		// 1. new a table
		// 2. set funcs into table
		// 3. set table into global
		// lua_newtable(L); // new a table to store funcs
		// luaL_setfuncs(L, cfuncs, 0); // set funcs into table
		// lua_setglobal(L, module_name); // name the table

		// luaL_newlib = lua_newtable + luaL_setfuncs
		luaL_newlib(L, cfuncs);
		lua_setglobal(L, module_name); // name the table
	}
	#endif
}

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

		lua_settop(L, 0);
		printf("\n");
		}

		// get table field from lua
		// 1. getglobal to get table
		// 2. push key, now table in -2
		// 3. get value, gettable(-2), key will pop auto
		// now value in -1, table in -2
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
			lua_settop(L, 0);
			printf("\n");
		}

		// use lua_getfield()
		// 1. getblobal get table, now table in -1
		// 2. getfield(-1, key) 
		// now value in -1, table in -2
		{
			printf("---- 3\n");
			// 1. get table
			const char *tablename = "background";
			lua_getglobal(L, tablename);
			if (!lua_istable(L, -1))
			{
				fprintf(stderr, "stack -2 not a table\n");
				break;
			}

			// 2.
			const char *key = "g";
			lua_getfield(L, -1, key); // same as lua_pushstring() + lua_gettable()
			if (!lua_isnumber(L, -1))
			{
				fprintf(stderr, "stack -1 not a number\n");
				break;
			}
			int value = lua_tonumber(L, -1);
			printf("tablename=%s key=%s value=%d\n", tablename, key, value);

			int top = lua_gettop(L);
			printf("top=%d\n", top);

			lua_pop(L, 1);

			// clean stack
			lua_settop(L, 0);
			printf("\n");
		}

		// get table field from lua
		{
			printf("---- 4\n");
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
		// 1. getglobal to get table
		// 2. push key
		// 3. push value, now value in -1, key in -2, table in -3
		// 4. settable(-3), key and value will pop auto
		{
			printf("---- 5\n");
			// 1. get table from global
			const char *tablename = "background";
			lua_getglobal(L, tablename);
			if (!lua_istable(L, -1))
			{
				fprintf(stderr, "stack -1 not a table\n");
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

			// key and value will pop auto
			int top = lua_gettop(L);
			printf("top=%d\n", top);

			key = "r";
			lua_pushstring(L, key);
			lua_gettable(L, -2);
			value = lua_tonumber(L, -1);
			printf("tablename=%s key=%s value=%d\n", tablename, key, value);

			lua_pop(L, 1);
			lua_settop(L, 0);
			printf("\n");
		}


		// use lua_setfield() to set into table
		// 1. getglobal to get table
		// 2. push value, now table in -2
		// 3. setfield(-2, key)
		{
			printf("---- 6\n");
			// 1. get table from global
			const char *tablename = "background";
			lua_getglobal(L, tablename);
			if (!lua_istable(L, -1))
			{
				fprintf(stderr, "stack -1 not a table\n");
				break;
			}

			// 2. push value 
			int value = 200;
			lua_pushnumber(L, value);

			// 3. set field
			const char * key = "g";
			lua_setfield(L, -2, key); // assert table in -2, logic same as lua_pushstring(key) + lua_insert(-2) + lua_settable()

			// key and value will pop auto
			int top = lua_gettop(L);
			printf("top=%d\n", top);

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
		// 1. new table, now table in -1
		// 2. setglobal(tablename), set a name for table, and table will pop
		{
			printf("---- 7\n");
			const char *tablename = "background3";
			// 1. new a table 
			lua_newtable(L);
			int top = lua_gettop(L);
			printf("top=%d\n", top);

			const char *key = "r"; int value = 20;
			lua_pushnumber(L, value);
			lua_setfield(L, -2, key);

			key = "g"; value = 30;
			lua_pushnumber(L, value);
			lua_setfield(L, -2, key);

			key = "b"; value = 40;
			lua_pushnumber(L, value);
			lua_setfield(L, -2, key);

			// 2. set table in lua
			lua_setglobal(L, tablename);
			top = lua_gettop(L);
			printf("top=%d\n", top);
			printf("\n");
		}

		print_rgb_table(L, "background");
		print_rgb_table(L, "background2");
		print_rgb_table(L, "background3");
		print_rgb_table(L, "background4");

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
		// 1. getglobal, get function
		// 2. push param
		// 3. pcall(L, param num, ret num, err-function-index)
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
			// lua_pcall(L, param_num, ret_num, error_function_index)
			// after pcall, fucntion and input params will be removed from stack
			if (lua_pcall(L, 2, 1, 0) != 0)
			{
				fprintf(stderr, "%s\n", lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			}
			
			ret = lua_tonumber(L, -1);
			printf("ret=%g\n", ret);

			int top = lua_gettop(L);
			printf("top=%d\n", top);

			lua_settop(L, 0);
		}

	} while (0);

	lua_close(L);

	return 0;
} // test2 end

// a c function will call by lua
// typedef int (*lua_CFunction)(lua_State *L)
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
		// luaL_dofile() == luaL_loadfile() + lua_pcall()
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		// 1. push c function for lua
		lua_pushcfunction(L, l_sin);

		// 2. name it in lua global environment, therefore lua can get this function
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
	double d1 = luaL_checknumber(L, -2);
	double d2 = luaL_checknumber(L, -1);
	// or
	// double d1 = luaL_checknumber(L, 1);
	// double d2 = luaL_checknumber(L, 2);
	printf("d1=%g d2=%g\n", d1, d2);
	lua_pushnumber(L, d1+d2);
	return 1;
}

static int cfunc_4sub(lua_State *L)
{
	double d1 = luaL_checknumber(L, -2);
	double d2 = luaL_checknumber(L, -1);
	printf("d1=%g d2=%g\n", d1, d2);
	lua_pushnumber(L, d1-d2);
	return 1;
}

static int cfunc_4times(lua_State *L)
{
	double d1 = luaL_checknumber(L, -2);
	double d2 = luaL_checknumber(L, -1);
	printf("d1=%g d2=%g\n", d1, d2);
	lua_pushnumber(L, d1*d2);
	return 1;
}

static int cfunc_4div(lua_State *L)
{
	double d1 = luaL_checknumber(L, -2);
	double d2 = luaL_checknumber(L, -1);
	printf("d1=%g d2=%g\n", d1, d2);
	lua_pushnumber(L, d1/d2);
	return 1;
}

// cfunction list
static const struct luaL_Reg cfunc_list [] =
{
	{"add", cfunc_4add} // table key and function
,	{"sub", cfunc_4sub}
,	{"times", cfunc_4times}
,	{"div", cfunc_4div}
,	{NULL, NULL} // must null end
};

int test4()
{
	// register a module with c functions for lua, and lua can call it
	const char *file = "test1.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	do
	{
		// register list of c functions for lua
		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		lua_getglobal(L, "_VERSION");
		const char * pv = lua_tostring(L, -1);
		char version[50];
		sprintf(version, "%s", pv);
		lua_pop(L, 1);
		printf("version=%s\n", version);

		const char *libname = "cfunc4";
		// core logic
		register_cmodule(L, libname, cfunc_list);

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

static int get_raw_len(lua_State *L, int index)
{
	// get raw len
	// string size
	// table: #
	// other: 0
#ifndef __LUA_5_2
	int size = lua_objlen(L, index);
#else
	int size = lua_rawlen(L, index);
#endif
	return size;
}

static void print_array(lua_State *L)
{
	if (!lua_istable(L, -1))
	{
		fprintf(stderr, "-1 not a table\n");
		return;
	}

	int size = get_raw_len(L, -1);
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
	// get and set from a table array
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

		const char *tablename = "test5array";
		lua_getglobal(L, tablename);
		if (!lua_istable(L, -1))
		{
			fprintf(stderr, "%s not a table\n", tablename);
			lua_pop(L, 1);
			break;
		}

		// get array table len
		int size = get_raw_len(L, -1);

		printf("size=%d\n", size);
		print_array(L);

		// get string value in array by index
		// use rawget
		// 1. push index
		// 2. rawget(L, table stack index)
		{
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
		}

		// get int value in array by index
		// use rawgeti, no need to push index
		// rawgeti(L, table stack index, index)
		{
			int index = 3;
			lua_rawgeti(L, -1, index);
			if (!lua_isnumber(L, -1))
			{
				fprintf(stderr, "-1 not a number\n");
				lua_pop(L, 1);
				break;
			}
			int value = lua_tonumber(L, -1);
			printf("[%d]value=%d\n", index, value);
			lua_pop(L, 1);
		}

		{
			// set value into array
			// use rawseti()
			// 1. push value
			// 2. rawseti(L, table_stack_index, index)
			int index = get_raw_len(L, -1) + 1;
			lua_pushstring(L, "hello");
			lua_rawseti(L, -2, index);
			printf("top=%d\n", lua_gettop(L));
		}

		{
			// set value into array
			// use rawset()
			// 1. push key
			// 2. push value
			// 3. rawset(L, table_stack_index)
			// stack info:
			// -1 : value
			// -2 : key
			// -3 : table
			int index = get_raw_len(L, -1) + 1;
			lua_pushnumber(L, index);
			lua_pushstring(L, "world");
			lua_rawset(L, -3);
			printf("top=%d\n", lua_gettop(L));
		}

		{
			// change value into array
			// use rawseti()
			// 1. push value
			// 2. rawseti(L, table_stack_index, index)
			int index = 1;
			lua_pushstring(L, "change");
			lua_rawseti(L, -2, index);
			printf("top=%d\n", lua_gettop(L));
		}

		print_array(L);

	} while (0);

	lua_close(L);

	return 0;
} // test5 end

int test6()
{
	// handle string
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

		lua_newtable(L);

		int index = 1;

		// push len string
		const char *v1 = "qwerty123456";
		lua_pushlstring(L, v1+2, 5);
		lua_rawseti(L, -2, index++);

		// push format string
		lua_pushfstring(L, "%s-%d", "masha", time(NULL));
		lua_rawseti(L, -2, index++);

		print_array(L);

		lua_settop(L, 0);

	} while (0);

	lua_close(L);

	return 0;
} // test6 end


static int str_upper(lua_State *L)
{
	size_t l;
	size_t i;
	printf("1 top=%d\n", lua_gettop(L));

	// lua string buffer
	luaL_Buffer b;

	// check and get len string
	const char *s = luaL_checklstring(L, 1, &l);

	// init lua string buffer
	luaL_buffinit(L, &b);
	printf("2 top=%d\n", lua_gettop(L));

	lua_pushstring(L, "good luck");
	// a function to push top to buffer
	luaL_addvalue(&b);
	printf("3 top=%d\n", lua_gettop(L));

	for (i = 0; i < l; i++)
	{
		// add char into buffer
		luaL_addchar(&b, toupper((unsigned char)(s[i])));
	}
	printf("4 top=%d\n", lua_gettop(L));

	// add lstring into buffer
	const char * s2 = " Masha";
	luaL_addlstring(&b, s2, strlen(s2));
	printf("5 top=%d\n", lua_gettop(L));

	// push buffer into stack
	luaL_pushresult(&b);
	printf("6 top=%d\n", lua_gettop(L));

	return 1;
}

int test7()
{
	// test lua string buffer
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

		// push c function to lua, and name it
		lua_pushcfunction(L, str_upper);
		lua_setglobal(L, "c_str_upper");

		lua_getglobal(L, "func_t7");
		if (!lua_isfunction(L, -1))
		{
			fprintf(stderr, "%s is not a function\n", "func_t7");
			lua_pop(L, 1);
			break;
		}

		const char *buffer = "Hello 123";
		lua_pushstring(L, buffer);

		if (lua_pcall(L, 1, 1, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		// function return
		const char *result = lua_tostring(L, -1);
		printf("result=%s\n", result);

	} while (0);

	lua_close(L);

	return 0;
} // test7 end

int test8()
{
	// registry
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

		// use registry to save global data from c to lua

		const char *key;
		const char *value;

		{
			// set string key and value into registry index
			// use registry like a global table
			key = "my_global_variable1";
			value = "helloworld1";
			lua_pushstring(L, value);
			lua_setfield(L, LUA_REGISTRYINDEX, key);

			key = "my_global_variable2";
			value = "helloworld2";
			lua_pushstring(L, value);
			lua_setfield(L, LUA_REGISTRYINDEX, key);

			// get value from registry index
			key = "my_global_variable1";
			lua_getfield(L, LUA_REGISTRYINDEX, key);
			value = lua_tostring(L, -1);
			printf("key=%s value=%s\n", key, value);
			lua_pop(L, 1);

			key = "my_global_variable2";
			lua_getfield(L, LUA_REGISTRYINDEX, key);
			value = lua_tostring(L, -1);
			printf("key=%s value=%s\n", key, value);
			lua_pop(L, 1);

			printf("\n");
		}

		{

			value = "helloworld3";
			lua_pushstring(L, value);
			// luaL_ref
			// save value into registry, and return a unique index key in registry
			int index = luaL_ref(L, LUA_REGISTRYINDEX);
			printf("index=%d\n", index);

			lua_rawgeti(L, LUA_REGISTRYINDEX, index);
			value = lua_tostring(L, -1);
			printf("index=%d value=%s\n", index, value);
			printf("top=%d\n", lua_gettop(L));
			lua_pop(L, 1);

			// release the index key and value from registry
			luaL_unref(L, LUA_REGISTRYINDEX, index);

			lua_rawgeti(L, LUA_REGISTRYINDEX, index);
			if (lua_isnil(L, -1))
			{
				printf("index=%d already release\n",index);
			}
			printf("top=%d\n", lua_gettop(L));
			lua_pop(L, 1);

			printf("\n");
		}


		{
			static char skey = 'k';
			value = "helloworld4";
			// use a pointer address as a unique key
			lua_pushlightuserdata(L, (void *)&skey);
			lua_pushstring(L, value);
			lua_settable(L, LUA_REGISTRYINDEX);

			lua_pushlightuserdata(L, (void *)&skey);
			lua_gettable(L, LUA_REGISTRYINDEX);
			value = lua_tostring(L, -1);
			printf("key=%p value=%s\n", &skey, value);
			lua_pop(L, 1);
		}


	} while (0);

	lua_close(L);

	return 0;
}

#ifndef __LUA_5_2

static int set_ga(lua_State *L)
{
	const char *name = "g_test9val";

	lua_pushinteger(L, 10);
	lua_setglobal(L, name);

	return 0;
}

static const struct luaL_Reg cfunc_list2 [] =
{
	{"set_ga", set_ga} // table key and function
,	{NULL, NULL} // must null end
};

static int create_independent_module(lua_State *L)
{
	printf("top=%d\n", lua_gettop(L));

	// 1. new a table as module environment
	// 2. replace environment
	// 3. register c module

	lua_newtable(L);
	lua_replace(L, LUA_ENVIRONINDEX); // replace environment
	register_cmodule(L, "cfunc9", cfunc_list2);

	return 0;
}
#endif

int test9()
{
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

#ifndef __LUA_5_2

		// set c functions environment
		// but no long work from 5.2
		// not work on 5.1 too...

		// let lua call a c function
		// in 5.3 like:
		// lua_pushcfunction()
		// lua_pushlightuserdata()
		// lua_pcall()
		lua_cpcall(L, create_independent_module, NULL);

		// call lua function
		const char *funcname = "func_t9";
		lua_getglobal(L, funcname);
		if (!lua_isfunction(L, -1))
		{
			fprintf(stderr, "%s is not a function\n", funcname);
			lua_pop(L, 1);
			break;
		}

		if (lua_pcall(L, 0, 0, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		// 
		const char *name = "g_test9val";
		lua_getglobal(L, name);
		if (lua_isnil(L, -1))
		{
			printf("%s is nil\n", name);
		}
		else
		{
			int val = lua_tointeger(L, -1);
			printf("%s = %d\n", name, val);
		}
#endif

	} while (0);

	lua_close(L);

	return 0;
}


// c closure function
static int counter(lua_State *L)
{
	// get upvalue by upvalue index
	int valA = lua_tointeger(L, lua_upvalueindex(1));
	int valB = lua_tointeger(L, lua_upvalueindex(2));
	printf("valA=%d valB=%d\n", valA, valB);

	// update upvalue
	lua_pushinteger(L, valA + 1);
	lua_replace(L, lua_upvalueindex(1));
	lua_pushinteger(L, valB + 1);
	lua_replace(L, lua_upvalueindex(2));

	// push result
	lua_pushinteger(L, valA * valB);

	return 1;
}

static int newCounter(lua_State *L)
{
	// this function is a factory, will create closure and upvalue for that closure

	// push upvalue for the closure, init value
	lua_pushinteger(L, 10);
	lua_pushinteger(L, 2);

	// push c closure, pass closure and upvalue num
	lua_pushcclosure(L, &counter, 2);
	printf("top=%d\n", lua_gettop(L));

	// closure will stay in stack
	return 1;
}

void print_top(lua_State *L, const char *msg)
{
	printf("%s top=%d\n", msg, lua_gettop(L));
}

int test10()
{
	// closure
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

		// push c closure factory for lua
		lua_pushcfunction(L, newCounter);
		lua_setglobal(L, "c_new_counter");

		{
			// get lua function
			const char *funcname = "func_t10";
			lua_getglobal(L, funcname);
			if (!lua_isfunction(L, -1))
			{
				fprintf(stderr, "%s not a function\n", funcname);
				lua_pop(L, 1);
				break;
			}

			// call lua function, will call closure factory, return closure
			if (lua_pcall(L, 0, 1, 0) != 0)
			{
				fprintf(stderr, "%s\n", lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			}

			// closure
			if (!lua_isfunction(L, -1))
			{
				fprintf(stderr, "reuslt not a function\n");
				lua_pop(L, 1);
				break;
			}
			print_top(L, "after create closure");

			// call closure
			for (int i = 0; i < 3; i++)
			{
				// copy closure and push into stack
				// because after call, closure will pop from stack
				lua_pushvalue(L, -1);

				if (lua_pcall(L, 0, 1, 0) != 0)
				{
					fprintf(stderr, "%s\n", lua_tostring(L, -1));
					lua_pop(L, 1);
					break;
				}
				print_top(L, "after call closure");

				if (!lua_isnumber(L, -1))
				{
					fprintf(stderr, "closure result not a integer\n");
					lua_pop(L, 1);
					break;
				}

				int result = lua_tointeger(L, -1);
				printf("result=%d\n", result);

				lua_pop(L, 1);
			}
		}

	} while (0);

	lua_close(L);

	return 0;
}


// c closure function
static int t_tuple(lua_State *L)
{
	// get optional input int, if not exists, return specified value
	int op = luaL_optinteger(L, 1, 0);
	if (op == 0)
	{
		int i;
		// use lua_isnone() check if upvalue is exists
		for (i = 1; !lua_isnone(L, lua_upvalueindex(i)); i++)
		{
			// push upvalue into stack to return
			lua_pushvalue(L, lua_upvalueindex(i));
		}
		return i - 1;
	}
	else
	{
		// check op is positive, else raise a error
		luaL_argcheck(L, 0 < op, 1, "index out of range");

		if (lua_isnone(L, lua_upvalueindex(op)))
		{
			return 0;
		}
		// push upvalue into stack to return
		lua_pushvalue(L, lua_upvalueindex(op));
		return 1;
	}
}

static int t_new(lua_State *L)
{
	// lua will push param when call this function
	// therefore set these param as upvalue

	lua_pushcclosure(L, t_tuple, lua_gettop(L));
	return 1;
}

static const struct luaL_Reg tuplefuncs [] =
{
	{"new", t_new}
,	{NULL, NULL}
};

int test11()
{
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

		register_cmodule(L, "tuple", tuplefuncs);

		lua_getglobal(L, "func_t11");
		if (!lua_isfunction(L, -1))
		{
			fprintf(stderr, "%s is not a function\n", "func_t_11");
			lua_pop(L, 1);
			break;
		}

		if (lua_pcall(L, 0, 0, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

	} while (0);

	lua_close(L);

	return 0;
}

typedef struct 
{
	int a;
} MySt;

static int myst_new(lua_State *L)
{
	// new a userdata, lua will handle this memory life
	// userdata will stay in stack
	MySt *ptr = (MySt *)lua_newuserdata(L, sizeof(MySt));
	ptr->a = 0;

	// get metatable
	luaL_getmetatable(L, "myst.mt");
	print_top(L, "after get metatable");

	// set metatable for userdata
	lua_setmetatable(L, -2);

	// userdata will stay in stack
	return 1;
}

static int myst_set(lua_State *L)
{
	// get userdata
	// MySt *ptr = (MySt *)lua_touserdata(L, 1);

	// check metatable and get userdata
	MySt *ptr = (MySt *)luaL_checkudata(L, 1, "myst.mt");
	
	luaL_argcheck(L, ptr != NULL, 1, "not a userdata");
	int n = lua_tointeger(L, 2);
	ptr->a = n;
	return 0;
}

static int myst_get(lua_State *L)
{
	// get userdata
	// MySt *ptr = (MySt *)lua_touserdata(L, 1);

	// check metatable and get userdata
	MySt *ptr = (MySt *)luaL_checkudata(L, 1, "myst.mt");
	luaL_argcheck(L, ptr != NULL, 1, "not a userdata");

	lua_pushinteger(L, ptr->a);

	return 1;
}


static const struct luaL_Reg myst_funcs [] =
{
	{"new", myst_new}
,	{"set", myst_set}
,	{"get", myst_get}
,	{NULL, NULL}
};

int test12()
{
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

		// normally, should create a metatable for userdata, therefor when using userdata in c function, if userdata not match, will throw a error.
		// luaL_newmetable(L, metatablename)
		// luaL_getmetable(L, metatablename)
		// lua_setmetable(L, stackindex)
		// create a metatable for c userdata
		luaL_newmetatable(L, "myst.mt");
		print_top(L, "after new metatable");

		register_cmodule(L, "myst", myst_funcs);
		print_top(L, "after register c module");

		lua_getglobal(L, "func_t12");
		if (!lua_isfunction(L, -1))
		{
			fprintf(stderr, "%s is not a function\n", "func_t_11");
			lua_pop(L, 1);
			break;
		}

		lua_pushinteger(L, 555);

		if (lua_pcall(L, 1, 0, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

	} while (0);

	lua_close(L);

	return 0;
}

// mylib13 start [

typedef struct mylib13_t
{
	int a;
} mylib13_t;

// mylib13 constructor
int mylib13_new(lua_State *L)
{
	mylib13_t *ptr = (mylib13_t *)lua_newuserdata(L, sizeof(mylib13_t));
	ptr->a = 0;

	// set metatable for obj
	luaL_getmetatable(L, "mylib13");
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg mylib13_constructor_funcs[] =
{
	{"new", mylib13_new}
,	{NULL, NULL}
};

// mylib13 member functions
int mylib13_set_a(lua_State *L)
{
	mylib13_t *ptr = (mylib13_t *)luaL_checkudata(L, 1, "mylib13");
	luaL_argcheck(L, ptr != NULL, 1, "null userdata");

	int n = lua_tointeger(L, 2);
	ptr->a = n;

	return 0;
}

int mylib13_get_a(lua_State *L)
{
	mylib13_t *ptr = (mylib13_t *)luaL_checkudata(L, 1, "mylib13");
	luaL_argcheck(L, ptr != NULL, 1, "null userdata");

	lua_pushinteger(L, ptr->a);

	return 1;
}

static const luaL_Reg mylib13_member_funcs[] =
{
	{"set_a", mylib13_set_a}
,	{"get_a", mylib13_get_a}
,	{NULL, NULL}
};

int luaopen_mylib13(lua_State *L)
{
	// open lib function
	// should leave a lib table in stack

	// create metatable for this lib, and set funcs into metatable
	luaL_newmetatable(L, "mylib13");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, mylib13_member_funcs, 0);
	print_top(L, "after setfunc into metatable");

	// register constructor function into a table, and leave it in stack
	luaL_newlib(L, mylib13_constructor_funcs);
	print_top(L, "after newlib");

	return 1;
}
// mylib13 start ]

int test13()
{
	const char *file = "test2.lua";

	lua_State *L = luaL_newstate();
	// luaL_openlibs(L); 
	// not open all lib, do it by myself

	do
	{
		const luaL_Reg lua_reg_libs[] =
		{
			// lua standard lib
			{"base", luaopen_base}
		,	{"table", luaopen_table}
		,	{LUA_IOLIBNAME, luaopen_io}
		,	{LUA_OSLIBNAME, luaopen_os}
		// ,	{LUA_BITLIBNAME, luaopen_bit32}
		,	{LUA_COLIBNAME, luaopen_coroutine}
		,	{LUA_MATHLIBNAME, luaopen_math}
		,	{LUA_DBLIBNAME, luaopen_debug}
		,	{LUA_LOADLIBNAME, luaopen_package}

			// require my lib
		,	{"mylib13", luaopen_mylib13}

		,	{NULL, NULL}
		};

		// require libs
		const luaL_Reg *libptr = lua_reg_libs;
		for (; libptr->func; ++libptr)
		{
			// call openlib function, set return table name and set into global
			luaL_requiref(L, libptr->name, libptr->func, 1);
			lua_pop(L, 1); // pop the require return
		}
		print_top(L, "after requiref all lib");

		if (luaL_dofile(L, file))
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

		lua_getglobal(L, "func_t13");
		if (!lua_isfunction(L, -1))
		{
			fprintf(stderr, "%s is not a function\n", "func_t_13");
			lua_pop(L, 1);
			break;
		}

		lua_pushinteger(L, 555);

		if (lua_pcall(L, 1, 0, 0) != 0)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}

	} while (0);

	lua_close(L);

	return 0;
}



int test14()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	
	int a = 1;
	lua_pushlightuserdata(L, (void *)&a);
	luaL_newmetatable(L, "t_m_14a");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "g_a");
	print_stack(L);

	int b = 2;
	lua_pushlightuserdata(L, (void *)&b);
	luaL_newmetatable(L, "t_m_14b");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "g_b");
	print_stack(L);

	lua_getglobal(L, "g_a");
	luaL_checkudata(L, 1, "t_m_14a");

	// getchar();

	return 0;
}

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
,	test8
,	test9
,	test10
,	test11
,	test12
,	test13
,	test14
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
				printf("\nstu:RUN testcase[%d]\n", i);
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
