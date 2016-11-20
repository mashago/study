#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	char buffer[256];
	int error;

	// new a lua state
	lua_State *L = luaL_newstate();

	// open stdlib
	luaL_openlibs(L);

	while (fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		// lua load from buffer, and protect call it
		error = luaL_loadbuffer(L, buffer, strlen(buffer), "line") || lua_pcall(L, 0, 0, 0);
		if (error)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	// close lua
	lua_close(L);

	return 0;
}
