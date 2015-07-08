#include "LuaAPI.h"
#include "LuaInterface.h"

namespace ScriptEngine
{
	static bool lua_apicheck_args( lua_State *pLua, int iArgMin, int iArgMax, std::string strFunctionName )
	{
		auto iNumberOfArgs = lua_gettop( pLua );
		if( iNumberOfArgs > iArgMax || iNumberOfArgs < iArgMin )
		{
			ENGINE_MSG( "%s: %s arguments(%d/%d)!\n", strFunctionName.c_str( ), ( iNumberOfArgs > 2 ) ? "to many" : "missing", iNumberOfArgs, iArgMax );
			return false;
		}

		return true;
	}
	static int lua_api_bitband( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "BitBand" ) )
			return lua_ret_int( pLua, 0 );

		if( !lua_isinteger( pLua, 1 ) || !lua_isinteger( pLua, 2 ) )
		{
			ENGINE_MSG( "BitBand: one or more argument isn't an integer!" );
			return lua_ret_int( pLua, 0 );
		}

		return lua_ret_int( pLua, lua_to_int( pLua, 1 ) & lua_to_int( pLua, 2 ) );
	}
	static int lua_api_getkeystate( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "GetKeyState" ) )
			return lua_ret_int( pLua, 0 );

		if( !lua_is_digit( pLua, 1 ) )
		{
			ENGINE_MSG( "GetKeyState: argument isn't digit!" );
			return lua_ret_int( pLua, 0 );
		}

		return lua_ret_int( pLua, GetKeyState( lua_to_int( pLua, 1 ) ) );
	}
	static int lua_api_include( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "Include" ) )
			return 0;

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "Include: argument isn't a string!" );
			return 0;
		}

		if( luaL_loadfile( pLua, GetApplicationDirectory( lua_to_string( pLua, 1 ) ).c_str( ) ) || lua_pcall( pLua, 0, 0, 0 ) )
			ENGINE_MSG( "Include: error executing: %s!", luaL_checkstring( pLua, -1 ) );

		return 0;	
	}
	static int lua_api_sleep( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "Sleep" ) )
			return 0;

		if( !lua_is_digit( pLua, 1 ) )
		{
			ENGINE_MSG( "Sleep: argument isn't digit!" );
			return 0;
		}

		Sleep( ( DWORD )lua_to_int( pLua, 1 ) );
		return 0;
	}
	static int lua_api_callback_registercallbackfunction( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "LuaCallback::RegisterCallbackFunction" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "LuaCallback::RegisterCallbackFunction: argument isn't a string!" );
			return lua_ret_bool( pLua );
		}

		g_pLuaCallback->RegisterCallbackFunction( lua_to_string( pLua, 1 ) );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_cvar_registercvar( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "CVarSystem::RegisterCVar" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "CVarSystem::RegisterCVar: argument 1 isn't a string!" );
			return lua_ret_bool( pLua );
		}
		
		auto bDigit = lua_is_digit( pLua, 2 );
		auto bBoolean = ( lua_isboolean( pLua, 2 ) == 1 );

		if( !bDigit && !bBoolean )
		{
			ENGINE_MSG( "CVarSystem::RegisterCVar: argument 2 isn't a boolean or digit number!" );
			return lua_ret_bool( pLua );
		}

		auto flValue = 0.f;
		if( bDigit )
			flValue = ( float )lua_to_number( pLua, 2 );
		else flValue = ( lua_to_bool( pLua, 2 ) ? 1.f : 0.f );

		g_pCVarManager->RegisterCVar( lua_to_string( pLua, 1 ), flValue );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_cvar_getbool( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "CVarSystem::GetBool" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "CVarSystem::GetBool: argument isn't a string!" );
			return lua_ret_bool( pLua );
		}

		return lua_ret_bool( pLua, g_pCVarManager->GetBool( lua_to_string( pLua, 1 ) ) );
	}
	static int lua_api_cvar_getfloat( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "CVarSystem::GetFloat" ) )
			return lua_ret_number( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "CVarSystem::GetFloat: argument isn't a string!" );
			return lua_ret_number( pLua );
		}

		return lua_ret_number( pLua, ( double )g_pCVarManager->GetFloat( lua_to_string( pLua, 1 ) ) );
	}
	static int lua_api_cvar_getint( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "CVarSystem::GetInt" ) )
			return lua_ret_int( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "CVarSystem::GetInt: argument isn't a string!" );
			return lua_ret_int( pLua );
		}

		return lua_ret_int( pLua, g_pCVarManager->GetInt( lua_to_string( pLua, 1 ) ) );
	}
	static int lua_api_cvar_setvalue( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "CVarSystem::SetValue" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "CVarSystem::RegisterCVar: argument 1 isn't a string!" );
			return lua_ret_bool( pLua );
		}

		auto bDigit = lua_is_digit( pLua, 2 );
		auto bBoolean = ( lua_isboolean( pLua, 2 ) == 1 );

		if( !bDigit && !bBoolean )
		{
			ENGINE_MSG( "CVarSystem::RegisterCVar: argument 2 isn't a boolean or digit number!" );
			return lua_ret_bool( pLua );
		}

		auto flValue = 0.f;
		if( bDigit )
			flValue = ( float )lua_to_number( pLua, 2 );
		else flValue = ( lua_to_bool( pLua, 2 ) ? 1.f : 0.f );

		g_pCVarManager->SetValue( lua_to_string( pLua, 1 ), flValue );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_file_readbool( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 3, 4, "GetBoolFromFile" ) )
			return lua_ret_bool( pLua );

		auto bDefaultValue = false;
		if( lua_gettop( pLua ) == 4 && lua_is_digit( pLua, 4 ) || lua_isboolean( pLua, 4 ) )
			bDefaultValue = lua_to_bool( pLua, 4 );

		auto bSucceeded = true;
		for( int i = 1; i <= 3; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "GetBoolFromFile: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !bSucceeded )
			return lua_ret_bool( pLua, bDefaultValue );

		return lua_ret_bool( pLua, GetBoolFromFile( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), GetApplicationDirectory( lua_to_string( pLua, 3 ) ), bDefaultValue ) );
	}
	static int lua_api_file_readfloat( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 3, 4, "GetFloatFromFile" ) )
			return lua_ret_number( pLua );

		auto DefaultValue = 0.0;
		if( lua_gettop( pLua ) == 4 && lua_is_digit( pLua, 4 ) )
			DefaultValue = lua_to_number( pLua, 4 );

		auto bSucceeded = true;
		for( int i = 1; i <= 3; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "GetFloatFromFile: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !bSucceeded )
			return lua_ret_number( pLua, DefaultValue );

		return lua_ret_number( pLua, ( double )GetFloatFromFile( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), GetApplicationDirectory( lua_to_string( pLua, 3 ) ), ( float )DefaultValue ) );
	}
	static int lua_api_file_readint( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 3, 4, "GetIntFromFile" ) )
			return lua_ret_int( pLua );

		auto iDefaultValue = 0;
		if( lua_gettop( pLua ) == 4 && lua_is_digit( pLua, 4 ) )
			iDefaultValue = lua_to_int( pLua, 4 );

		auto bSucceeded = true;
		for( int i = 1; i <= 3; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "GetIntFromFile: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !bSucceeded )
			return lua_ret_int( pLua, iDefaultValue );

		return lua_ret_int( pLua, GetIntFromFile( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), GetApplicationDirectory( lua_to_string( pLua, 3 ) ), iDefaultValue ) );
	}
	static int lua_api_file_writebool( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 4, 4, "WriteBoolToFile" ) )
			return lua_ret_bool( pLua );

		auto bSucceeded = true;
		for( int i = 1; i <= 3; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "WriteBoolToFile: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !lua_is_digit( pLua, 4 ) && !lua_isboolean( pLua, 4 ) )
		{
			ENGINE_MSG( "WriteBoolToFile: argument 4 isn't a boolean!" );
			return lua_ret_bool( pLua );
		}

		WriteBoolToFile( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), GetApplicationDirectory( lua_to_string( pLua, 3 ) ), lua_to_bool( pLua, 4 ) );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_file_writefloat( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 4, 4, "WriteFloatToFile" ) )
			return lua_ret_bool( pLua );

		auto bSucceeded = true;
		for( int i = 1; i <= 3; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "WriteFloatToFile: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !lua_is_digit( pLua, 4 ) && !lua_isboolean( pLua, 4 ) )
		{
			ENGINE_MSG( "WriteFloatToFile: argument 4 isn't a number!" );
			return lua_ret_bool( pLua );
		}

		WriteFloatToFile( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), GetApplicationDirectory( lua_to_string( pLua, 3 ) ), ( float )lua_to_number( pLua, 4 ) );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_file_writeint( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 4, 4, "WriteIntToFile" ) )
			return lua_ret_bool( pLua );

		auto bSucceeded = true;
		for( int i = 1; i <= 3; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "WriteIntToFile: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !lua_is_digit( pLua, 4 ) && !lua_isboolean( pLua, 4 ) )
		{
			ENGINE_MSG( "WriteIntToFile: argument 4 isn't a number!" );
			return lua_ret_bool( pLua );
		}

		WriteIntToFile( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), GetApplicationDirectory( lua_to_string( pLua, 3 ) ), lua_to_int( pLua, 4 ) );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_remoteprocess_registermodule( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "RemoteProcess::RegisterModule" ) )
			return lua_ret_bool( pLua, false );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::RegisterModule: argument isn't a string!" );
			return lua_ret_bool( pLua, false );
		}

		return lua_ret_bool( pLua, g_pRemoteProcess->RegisterModule( lua_to_string( pLua, 1 ) ) );
	}
	static int lua_api_remoteprocess_findsignature( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 3, "RemoteProcess::FindSignature" ) )
			return lua_ret_int( pLua );

		auto bSucceeded = true;
		for( int i = 1; i <= 2; i++ )
		{
			if( !lua_isstring( pLua, i ) )
			{
				ENGINE_MSG( "RemoteProcess::FindSignature: argument %d isn't a string!", i );
				bSucceeded = false;
			}
		}

		if( !bSucceeded )
			return lua_ret_int( pLua );

		DWORD_PTR dwExtraOffset = 0;
		if( lua_gettop( pLua ) == 3 )
		{
			if( !lua_isinteger( pLua, 3 ) )
			{
				ENGINE_MSG( "RemoteProcess::FindSignature: argument 3 isn't an integer!" );
				lua_ret_int( pLua );
			}

			dwExtraOffset = ( DWORD_PTR )lua_to_int( pLua, 3 );
		}

		
		return lua_ret_int( pLua, ( int )g_pRemoteProcess->FindSignature( lua_to_string( pLua, 1 ), lua_to_string( pLua, 2 ), dwExtraOffset ) );
	}
	static int lua_api_remoteprocess_getmoduledata( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "RemoteProcess::GetModuleData" ) )
			return lua_ret_int( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::RegisterModule: argument isn't a string!" );
			return lua_ret_bool( pLua, false );
		}
		
		auto ModuleData = g_pRemoteProcess->GetModuleData( lua_to_string( pLua, 1 ) );
		lua_push_int( pLua, ( int )ModuleData.modBaseAddr );
		lua_push_int( pLua, ( int )ModuleData.modBaseSize );
		return 2;
	}
	static int lua_api_remoteprocess_sendmessage( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 3, 3, "RemoteProcess::SendMessage" ) )
			lua_ret_bool( pLua );

		auto bSucceeded = true;
		for( int i = 0; i < 3; i++ )
		{			
			if( !lua_isinteger( pLua, i ) )
			{
				ENGINE_MSG( "RemoteProcess::SendMeesage: argument %s isn't an integer!", i );
				bSucceeded = false;
			}
		}
		if( !bSucceeded )
			return lua_ret_bool( pLua );

		SendMessageA( g_pRemoteProcess->GetWindowHandle( ), ( UINT )lua_to_int( pLua, 1 ), ( WPARAM )lua_to_int( pLua, 2 ), ( LPARAM )lua_to_int( pLua, 3 ) );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_remoteprocess_readbool( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "RemoteProcess::ReadBool" ) )
			return lua_ret_bool( pLua, false );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::ReadBool: argument isn't an integer!" );
			return lua_ret_bool( pLua, false );
		}

		return lua_ret_bool( pLua, g_pRemoteProcess->Read< bool >( ( DWORD_PTR )lua_to_int( pLua, 1 ) ) );
	}
	static int lua_api_remoteprocess_readbyte( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "RemoteProcess::ReadByte" ) )
			return lua_ret_int( pLua, 0 );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::ReadByte: argument isn't an integer!" );
			return lua_ret_int( pLua, 0 );
		}

		return lua_ret_int( pLua, ( int )g_pRemoteProcess->Read< BYTE >( ( DWORD_PTR )lua_to_int( pLua, 1 ) ) );
	}
	static int lua_api_remoteprocess_readfloat( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "RemoteProcess::ReadFloat" ) )
			return lua_ret_number( pLua, 0.0 );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::ReadFloat: argument isn't an integer!" );
			return lua_ret_number( pLua, 0 );
		}

		return lua_ret_number( pLua, ( double )g_pRemoteProcess->Read< float >( ( DWORD_PTR )lua_to_int( pLua, 1 ) ) );
	}
	static int lua_api_remoteprocess_readinteger( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "RemoteProcess::ReadInteger" ) )
			return lua_ret_int( pLua, 0 );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::ReadInteger: argument isn't an integer!" );
			return lua_ret_int( pLua, 0 );
		}

		return lua_ret_int( pLua, g_pRemoteProcess->Read< int >( ( DWORD_PTR )lua_to_int( pLua, 1 ) ) );
	}
	static int lua_api_remoteprocess_readstring( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "RemoteProcess::ReadString" ) )
			return lua_ret_number( pLua, 0.0 );

		if( !lua_isinteger( pLua, 1 ) || !lua_isinteger( pLua, 2 ) )
		{
			ENGINE_MSG( "RemoteProcess::ReadString: one or more arguments aren't an integer!" );
			return lua_ret_number( pLua, 0 );
		}

		auto pszStringBuffer = new char[ lua_to_int( pLua, 2 ) ];
		ReadProcessMemory( g_pRemoteProcess->GetProcessHandle( ), ( LPCVOID )( lua_to_int( pLua, 1 ) ), &pszStringBuffer, sizeof( pszStringBuffer ), NULL );
		
		lua_push_string( pLua, pszStringBuffer );
		delete[] pszStringBuffer;

		return 1;
	}
	static int lua_api_remoteprocess_writebool( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "RemoteProcess::WriteBool" ) )
			return lua_ret_bool( pLua );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteBool: argument 1 isn't an integer!" );
			return lua_ret_bool( pLua );
		}
		if( !lua_isboolean( pLua, 2 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteBool: argument 2 isn't a boolean!" );
			return lua_ret_bool( pLua );
		}

		auto bRetVal = g_pRemoteProcess->Write< bool >( ( DWORD_PTR )lua_to_int( pLua, 1 ), lua_to_bool( pLua, 2 ) );
		return lua_ret_bool( pLua, ( bRetVal == TRUE ? true : false ) );
	}
	static int lua_api_remoteprocess_writebyte( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "RemoteProcess::WriteByte" ) )
			return lua_ret_bool( pLua );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteByte: argument 1 isn't an integer!" );
			return lua_ret_bool( pLua );
		}
		if( !lua_isinteger( pLua, 2 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteByte: argument 2 isn't a byte!" );
			return lua_ret_bool( pLua );
		}

		auto bRetVal = g_pRemoteProcess->Write< BYTE >( ( DWORD_PTR )lua_to_int( pLua, 1 ), lua_to_bool( pLua, 2 ) );
		return lua_ret_bool( pLua, ( bRetVal == TRUE ? true : false ) );
	}
	static int lua_api_remoteprocess_writefloat( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "RemoteProcess::WriteFloat" ) )
			return lua_ret_bool( pLua );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteFloat: argument 1 isn't an integer!" );
			return lua_ret_bool( pLua );
		}
		if( !lua_is_digit( pLua, 2 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteFloat: argument 2 isn't a number!" );
			return lua_ret_bool( pLua );
		}

		auto bRetVal = g_pRemoteProcess->Write< float >( ( DWORD_PTR )lua_to_int( pLua, 1 ), ( float )lua_to_number( pLua, 2 ) );
		return lua_ret_bool( pLua, ( bRetVal == TRUE ? true : false ) );
	}
	static int lua_api_remoteprocess_writeinteger( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "RemoteProcess::WriteInteger" ) )
			return lua_ret_bool( pLua );

		if( !lua_isinteger( pLua, 1 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteInteger: argument 1 isn't an integer!" );
			return lua_ret_bool( pLua );
		}
		if( !lua_is_digit( pLua, 2 ) )
		{
			ENGINE_MSG( "RemoteProcess::WriteInteger: argument 2 isn't a number!" );
			return lua_ret_bool( pLua );
		}

		auto bRetVal = g_pRemoteProcess->Write< int >( ( DWORD_PTR )lua_to_int( pLua, 1 ), lua_to_int( pLua, 2 ) );
		return lua_ret_bool( pLua, ( bRetVal == TRUE ? true : false ) );
	}
	
	void lua_api_register( lua_State *pLua )
	{
		static luaL_Reg CVarSystemLibrary[] =
		{
			{ "RegisterCVar", lua_api_cvar_registercvar },
			{ "GetBool", lua_api_cvar_getbool },
			{ "GetFloat", lua_api_cvar_getfloat },
			{ "GetInt", lua_api_cvar_getint },
			{ "SetValue", lua_api_cvar_setvalue },
			{ NULL, NULL }
		};

		static luaL_Reg FileSystemLibrary[] =
		{
			{ "GetBool", lua_api_file_readbool },
			{ "GetFloat", lua_api_file_readfloat },
			{ "GetInt", lua_api_file_readint },
			{ "WriteBool", lua_api_file_writebool },
			{ "WriteFloat", lua_api_file_writefloat },
			{ "WriteInt", lua_api_file_writeint },
			{ NULL, NULL }
		};

		static luaL_Reg RemoteProcessLibrary[] =
		{
			{ "RegisterModule", lua_api_remoteprocess_registermodule },
			{ "FindSignature", lua_api_remoteprocess_findsignature },
			{ "GetModuleData", lua_api_remoteprocess_getmoduledata },
			{ "SendMessage", lua_api_remoteprocess_sendmessage },
			{ "ReadBool", lua_api_remoteprocess_readbool },
			{ "ReadByte", lua_api_remoteprocess_readbyte },
			{ "ReadFloat", lua_api_remoteprocess_readfloat },
			{ "ReadInteger", lua_api_remoteprocess_readinteger },
			{ "ReadString", lua_api_remoteprocess_readstring },
			{ "WriteBool", lua_api_remoteprocess_writebool },
			{ "WriteByte", lua_api_remoteprocess_writebyte },
			{ "WriteFloat", lua_api_remoteprocess_writefloat },
			{ "WriteInteger", lua_api_remoteprocess_writeinteger },
			{ NULL, NULL }
		};

		lua_export_function( pLua, "RegisterCallbackFunction", lua_api_callback_registercallbackfunction );
		lua_export_function( pLua, "BitBand", lua_api_bitband );
		lua_export_function( pLua, "GetKeyState", lua_api_getkeystate );
		lua_export_function( pLua, "Include", lua_api_include );
		lua_export_function( pLua, "Sleep", lua_api_sleep );
		lua_export_library( pLua, "CVar", CVarSystemLibrary );
		lua_export_library( pLua, "File", FileSystemLibrary );
		lua_export_library( pLua, "Process", RemoteProcessLibrary );
	}
}