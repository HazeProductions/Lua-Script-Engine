#include "Callback.h"

using namespace ScriptEngine;

std::size_t LuaCallback::GetCallbackFunctionIndex( std::string strFunctionName )
{
	if( strFunctionName.empty( ) )
	{
		ENGINE_MSG( "GetCallbackFunctionIndex: empty function name!" );
		return -1;
	}

	for( std::size_t i = 0; i < this->m_vstrCallbackFunctions.size( ); i++ )
		if( !this->m_vstrCallbackFunctions[ i ].compare( strFunctionName ) )
			return i;

	return -1;
}

LuaCallback::LuaCallback( void )
{

}

LuaCallback::~LuaCallback( void )
{

}

void LuaCallback::RegisterCallbackFunction( std::string strFunctionName )
{
	if( strFunctionName.empty( ) )
	{
		ENGINE_MSG( "RegisterCallbackFunction: empty function name!" );
		return;
	}

	if( this->GetCallbackFunctionIndex( strFunctionName ) != -1 )
	{
		ENGINE_MSG( "RegisterCallbackFunction: %s is allready registered!", strFunctionName.c_str( ) );
		return;
	}

	ENGINE_MSG( "RegisterCallbackFunction: registered %s!", strFunctionName.c_str( ) );
	this->m_vstrCallbackFunctions.push_back( strFunctionName );
}

void LuaCallback::UnregisterCallbackFunction( std::string strFunctionName )
{
	if( strFunctionName.empty( ) )
	{
		ENGINE_MSG( "UnregisterCallbackFunction: empty function name!" );
		return;
	}
	auto nIndex = this->GetCallbackFunctionIndex( strFunctionName );
	if( nIndex != -1 )
	{
		this->m_vstrCallbackFunctions.erase( this->m_vstrCallbackFunctions.begin( ) + nIndex );
		ENGINE_MSG( "UnregisterCallbackFunction: unregistered function: %s!", strFunctionName.c_str( ) );
	}
}

void LuaCallback::CallRegisteredFunctions( lua_State *pLua )
{
	if( !pLua )
	{
		ENGINE_MSG( "CallRegisteredFunctions: invalid lua interface pointer!" );
		return;
	}

	for( std::size_t i = 0; i < this->m_vstrCallbackFunctions.size( ); i++ )
	{
		// Skip all invalid callback functions
		for( std::size_t j = 0; j < this->m_vnCallbackErrorFunctions.size( ); j++ )
			if( i == this->m_vnCallbackErrorFunctions[ j ] )
				continue;

		auto c = this->m_vstrCallbackFunctions[ i ].c_str( );
		lua_getglobal( pLua, c );
		if( lua_pcall( pLua, 0, 1, 0 ) )
		{
			this->m_vnCallbackErrorFunctions.push_back( i );
			ENGINE_MSG( "CallRegisteredFunctions: %s isn't registered!", c );
		}
	}
}

ScriptEngine::LuaCallback *g_pLuaCallback = nullptr;