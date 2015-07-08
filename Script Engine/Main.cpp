#include "stdafx.h"
#include "Lua\LuaAPI.h"

int main( int iArgC, char *pszArg[] )
{
	printf( "\t\t\t __               _____         _\n" );
	printf( "\t\t\t|  |   _ _ ___   |   __|___ ___|_|___ ___\n" );
	printf( "\t\t\t|  |__| | | .'|  |   __|   | . | |   | -_|\n" );
	printf( "\t\t\t|_____|___|__,|  |_____|_|_|_  |_|_|_|___|\n" );
	printf( "\t\t\t                           |___|\n\n\n" );
	
	// Setup the command line pointer and register the client commands
	g_pCmdLineInterface = new CmdLineInterface( );
	g_pCmdLineInterface->RegisterCommand( "-process" );
	g_pCmdLineInterface->RegisterCommand( "-file" );

	if( g_pCmdLineInterface->GetCommandsFromCmdLine( iArgC, pszArg ) )
	{
		CmdCommand_t ProcCmd, FileCmd;
		g_pCmdLineInterface->GetCommand( "-process", &ProcCmd );
		g_pCmdLineInterface->GetCommand( "-file", &FileCmd );
		
		g_pCVarManager = new CVarSystem( );
		g_pLuaCallback = new ScriptEngine::LuaCallback( );
		g_pRemoteProcess = new RemoteProcess( );
		if( g_pRemoteProcess->Attach( ProcCmd.m_strValue ) )
		{
			auto pLua = ScriptEngine::lua_attach( );
			ScriptEngine::lua_api_register( pLua );
			ScriptEngine::lua_execute( pLua, FileCmd.m_strValue );

			while( !( GetKeyState( VK_F2 ) & 0x8000 ) )
			{
				g_pLuaCallback->CallRegisteredFunctions( pLua );
				Sleep( 1 );
			}

			ScriptEngine::lua_release( pLua );
		}		

		g_pCVarManager->Release( );
		g_pCmdLineInterface->Release( );
		g_pRemoteProcess->Release( );
	}

	

	return 0;
}