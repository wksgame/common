#include"LuaWrapper.h"

extern "C"
{
#include"lua.h"
#include"lualib.h"
#include"lauxlib.h"
}

namespace kiss
{
	LuaWrapper::LuaWrapper()
	{
		L = lua_newstate();
		luaopen_base(L);         /* opens the basic library 这些是在引入一些库,就如如果add函数在编译成dll后如果在lua中要使用需要require “动态库名"一样*/
		luaopen_table(L);        /* opens the table library这些库是加在这里只是测试 */
		luaopen_io(L);           /* opens the I/O library */
		luaopen_string(L);       /* opens the string lib. */
		luaopen_math(L);         /* opens the math lib. */
	}

	LuaWrapper::~LuaWrapper()
	{
		if(L)
		{
			lua_close(L);
			L = nullptr;
		}
	}

	int LuaWrapper::Call(const char* funcName, int param1, int param2)
	{
		int status = lua_status(L);
		if(status != 0 && status != 2)
		{
			return 0;
		}

		lua_getglobal(L, funcName);
		if(!lua_iscfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}

		lua_pushinteger(L, param1);
		lua_pushinteger(L, param2);

		int error = lua_pcall(L,2,1,0);

		if (error)
		{
			const char* errMsg = lua_tostring(L,-1);
			lua_pop(L, 1); // clean error message
			return 0;
		}

		// get return value
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}

		int ret = lua_tointeger(L, -1);
		lua_pop(L, 1);                                                /* stack: - */
		return ret;
	}

}