#ifndef __LUA_CALLBACK_H__
#define __LUA_CALLBACK_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "..\stdafx.h"

namespace ScriptEngine
{
	class LuaCallback
	{
	private:
		std::vector< std::size_t >	m_vnCallbackErrorFunctions;
		std::vector< std::string >	m_vstrCallbackFunctions;
		
	private:
		std::size_t					GetCallbackFunctionIndex( std::string strFunctionName );
	public:
		LuaCallback( void );
		~LuaCallback( void );
		void						RegisterCallbackFunction( std::string strFunctionName );
		void						UnregisterCallbackFunction( std::string strFunctionName );
		void						CallRegisteredFunctions( lua_State *pLua );
	};
}

extern ScriptEngine::LuaCallback *g_pLuaCallback;

#endif