#ifndef __USERAPI_H__
#define __USERAPI_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "stdafx.h"

extern bool			GetBoolFromFile( std::string strAppName, std::string strKeyName, std::string strPath, bool bDefaultValue = false );
extern bool			GetRemoteModuleEntryData( std::string strModuleName, DWORD dwProcessId, MODULEENTRY32 *pModuleEntry );
extern DWORD		GetProcessIdByName( std::string strProcessName );
extern HWND			GetWindowHandleFromProcessId( DWORD dwProcessId );
extern float		GetFloatFromFile( std::string strAppName, std::string strKeyName, std::string strPath, float flDefaultValue = 0.f );
extern int			GetIntFromFile( std::string strAppName, std::string strKeyName, std::string strPath, int iDefaultValue = 0 );
extern std::string	GetApplicationDirectory( std::string strOptionalFileInDir = "" );
extern void			ENGINE_MSG( const char *pszString, ... );
extern void			WriteBoolToFile( std::string strAppName, std::string strKeyName, std::string strPath, bool bValue );
extern void			WriteFloatToFile( std::string strAppName, std::string strKeyName, std::string strPath, float flValue );
extern void			WriteIntToFile( std::string strAppName, std::string strKeyName, std::string strPath, int iValue );

#endif