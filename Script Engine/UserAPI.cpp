#include "UserAPI.h"

bool GetBoolFromFile( std::string strAppName, std::string strKeyName, std::string strPath, bool bDefaultValue )
{
	if( strAppName.empty( ) || strKeyName.empty( ) || strPath.empty( ) )
		return bDefaultValue;

	auto pszDefaultValue = ( bDefaultValue ? "true" : "false" );

	char pszBuffer[ 0x400 ];
	if( !GetPrivateProfileStringA( strAppName.c_str( ), strKeyName.c_str( ), pszDefaultValue, pszBuffer, 0x400, strPath.c_str( ) ) )
		return bDefaultValue;

	std::string strRetVal( pszBuffer );
	std::transform( strRetVal.begin( ), strRetVal.end( ), strRetVal.begin( ), ::tolower );

	auto bFalse = ( !strRetVal.compare( "false" ) );
	auto bTrue = ( !strRetVal.compare( "true" ) );

	if( !bTrue && !bFalse )
		return bDefaultValue;

	if( bTrue )
		return true;

	return false;
}

bool GetRemoteModuleEntryData( std::string strModuleName, DWORD dwProcessId, MODULEENTRY32 *pModuleEntry )
{
	if( strModuleName.empty( ) || !dwProcessId )
		return false;

	auto hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId );
	if( hSnapshot == INVALID_HANDLE_VALUE )
		return false;

	auto bFoundModuleEntryData = false;
	MODULEENTRY32 ModuleEntry = { sizeof( MODULEENTRY32 ) };
	if( auto bFoundCurrentModuleEntryData = Module32First( hSnapshot, &ModuleEntry ) )
	{
#ifdef _UNICODE
		std::wstring wstrModuleName( strModuleName.begin( ), strModuleName.end( ) );
#endif

		bool bComparedModuleName = false;
		while( bFoundCurrentModuleEntryData )
		{
#ifdef _UNICODE
			bComparedModuleName = ( !wstrModuleName.compare( ModuleEntry.szModule ) );
#else
			bComparedModuleName = ( !strModuleName.compare( ModuleEntry.szModule ) );
#endif
			if( bComparedModuleName )
			{
				if( pModuleEntry )
					*pModuleEntry = ModuleEntry;

				bFoundModuleEntryData = true;
				break;
			}
			bFoundCurrentModuleEntryData = Module32Next( hSnapshot, &ModuleEntry );
		}
	}

	CloseHandle( hSnapshot );

	return bFoundModuleEntryData;
}

DWORD GetProcessIdByName( std::string strProcessName )
{
	if( strProcessName.empty( ) )
		return false;

	auto hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hSnapshot == INVALID_HANDLE_VALUE )
		return false;

	DWORD dwProcessId = 0;
	PROCESSENTRY32 ProcEntry = { sizeof( PROCESSENTRY32 ) };
	if( auto bFoundCurrentProcess = Process32First( hSnapshot, &ProcEntry ) )
	{
#ifdef _UNICODE
		std::wstring wstrModuleName( strProcessName.begin( ), strProcessName.end( ) );
#endif
		auto bComparedProcName = false;
		while( bFoundCurrentProcess )
		{
#ifdef _UNICODE
			bComparedProcName = ( !wstrModuleName.compare( ProcEntry.szExeFile ) );
#else
			bComparedProcName = ( !strProcessName.compare( ProcEntry.szExeFile ) );
#endif
			if( bComparedProcName )
			{
				dwProcessId = ProcEntry.th32ProcessID;
				break;
			}
			bFoundCurrentProcess = Process32Next( hSnapshot, &ProcEntry );
		}
	}

	CloseHandle( hSnapshot );

	return dwProcessId;
}

float GetFloatFromFile( std::string strAppName, std::string strKeyName, std::string strPath, float flDefaultValue )
{
	if( strAppName.empty( ) || strKeyName.empty( ) || strPath.empty( ) )
		return flDefaultValue;

	char pszDefaultValue[ 0x200 ];
	sprintf_s( pszDefaultValue, "%f", flDefaultValue );

	char pszBuffer[ 0x400 ];
	if( !GetPrivateProfileStringA( strAppName.c_str( ), strKeyName.c_str( ), pszDefaultValue, pszBuffer, 0x400, strPath.c_str( ) ) )
		return flDefaultValue;

	if( !isdigit( pszBuffer[ 0 ] ) )
		return flDefaultValue;

	return std::strtof( pszBuffer, NULL );
}

HWND GetWindowHandleFromProcessId( DWORD dwProcessId )
{
	auto hWnd = GetTopWindow( NULL );

	while( hWnd )
	{
		DWORD dwCurrentProcessId = 0;
		GetWindowThreadProcessId( hWnd, &dwCurrentProcessId );

		if( dwCurrentProcessId == dwProcessId )
			break;

		hWnd = GetNextWindow( hWnd, GW_HWNDNEXT );
	}

	return hWnd;
}

int GetIntFromFile( std::string strAppName, std::string strKeyName, std::string strPath, int iDefaultValue )
{
	if( strAppName.empty( ) || strKeyName.empty( ) || strPath.empty( ) )
		return iDefaultValue;

	return ( int )GetPrivateProfileIntA( strAppName.c_str( ), strKeyName.c_str( ), iDefaultValue, strPath.c_str( ) );
}

std::string GetApplicationDirectory( std::string strOptionalFileInDir )
{
	char pszPath[ MAX_PATH ];
	GetCurrentDirectoryA( MAX_PATH, pszPath );

	auto strApplicationDirectory = std::string( pszPath ) + std::string( "\\" );
	if( !strOptionalFileInDir.empty( ) )
		strApplicationDirectory.append( strOptionalFileInDir );

	return strApplicationDirectory;
}

void ENGINE_MSG( const char *pszString, ... )
{
	va_list args;
	va_start( args, pszString );
	char pszBuffer[ 0x400 ];
	vsprintf_s( pszBuffer, pszString, args );
	auto hCon = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hCon, 10 );
	printf( "[" );
	SetConsoleTextAttribute( hCon, 12 );
	printf( "ScriptEngine" );
	SetConsoleTextAttribute( hCon, 10 );
	printf( "] " );
	SetConsoleTextAttribute( hCon, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );
	printf( pszBuffer );
	printf( "\n" );	
	va_end( args );
}

void WriteBoolToFile( std::string strAppName, std::string strKeyName, std::string strPath, bool bValue )
{
	if( strAppName.empty( ) || strKeyName.empty( ) || strPath.empty( ) )
		return;

	char pszBuffer[ 0x20 ];
	sprintf_s( pszBuffer, ( bValue ? "true" : "false" ) );

	WritePrivateProfileStringA( strAppName.c_str( ), strKeyName.c_str( ), pszBuffer, strPath.c_str( ) );
}

void WriteFloatToFile( std::string strAppName, std::string strKeyName, std::string strPath, float flValue )
{
	if( strAppName.empty( ) || strKeyName.empty( ) || strPath.empty( ) )
		return;

	char pszBuffer[ 0x20 ];
	sprintf_s( pszBuffer, "%f", flValue );

	WritePrivateProfileStringA( strAppName.c_str( ), strKeyName.c_str( ), pszBuffer, strPath.c_str( ) );
}

void WriteIntToFile( std::string strAppName, std::string strKeyName, std::string strPath, int iValue )
{
	if( strAppName.empty( ) || strKeyName.empty( ) || strPath.empty( ) )
		return;

	char pszBuffer[ 0x20 ];
	sprintf_s( pszBuffer, "%i", iValue );

	WritePrivateProfileStringA( strAppName.c_str( ), strKeyName.c_str( ), pszBuffer, strPath.c_str( ) );
}