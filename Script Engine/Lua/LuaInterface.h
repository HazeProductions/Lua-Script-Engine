#ifndef __LUA_LUAINTERFACE_H__
#define __LUA_LUAINTERFACE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "..\stdafx.h"

namespace ScriptEngine
{
	extern bool			lua_is_digit( lua_State *pLua, int iStackPos );
	extern bool			lua_to_bool( lua_State *pLua, int iStackPos );
	extern double		lua_to_number( lua_State *pLua, int iStackPos );
	extern int			lua_to_int( lua_State *pLua, int iStackPos );
	extern int			lua_ret_bool( lua_State *pLua, bool bRetVal = false );
	extern int			lua_ret_int( lua_State *pLua, int iRetVal = 0 );
	extern int			lua_ret_number( lua_State *pLua, double nRetVal = 0 );
	extern int			lua_ret_nil( lua_State *pLua );
	extern int			lua_ret_string( lua_State *pLua, std::string strRetVal );
	extern lua_State	*lua_attach( void );
	extern std::string	lua_to_string( lua_State* pLua, int iStackPos );
	extern void			lua_execute( lua_State *pLua, std::string strLuaFile );
	extern void			lua_release( lua_State *pLua );
	extern void			lua_push_bool( lua_State *pLua, bool bRetVal = false );
	extern void			lua_push_int( lua_State *pLua, int iRetVal = 0 );
	extern void			lua_push_number( lua_State *pLua, double nRetVal = 0 );
	extern void			lua_push_nil( lua_State *pLua );
	extern void			lua_push_string( lua_State *pLua, std::string strRetVal );
	extern void			lua_export_function( lua_State *pLua, std::string strFunctionName, const lua_CFunction fn );
	extern void			lua_export_library( lua_State *pLua, std::string strLibraryName, const luaL_Reg *pLibraryFunctions );
}

#endif