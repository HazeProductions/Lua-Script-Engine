#ifndef __LUA_LUAAPI_H__
#define __LUA_LUAAPI_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "..\stdafx.h"

namespace ScriptEngine
{
	extern void lua_api_register( lua_State *pLua );
}

#endif