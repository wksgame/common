#ifndef KISS_LUA_WRAPPER_H
#define KISS_LUA_WRAPPER_H

struct lua_State;

namespace kiss
{
	class LuaWrapper
	{
		lua_State* L;

	public:
		LuaWrapper();
		~LuaWrapper();

		int Call(const char* funcName, int param1, int param2);
	};//class LuaWrapper

}//namespace kiss
#endif//KISS_LUA_WRAPPER_H