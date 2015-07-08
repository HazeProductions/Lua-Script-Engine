#ifndef __REMOTEPROCESS_H__
#define __REMOTEPROCESS_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "stdafx.h"

class RemoteProcess
{
private:
	bool							m_bAttached;
	DWORD							m_dwProcessId;
	HANDLE							m_hProcess;
	HWND							m_hWindow;
	std::string						m_strExecutablePath;
	std::vector< MODULEENTRY32 >	m_vModuleEntryData;

private:
	bool							DataCompare( const unsigned char *pbData, const unsigned char *pbMask, const char* pszMask );
	DWORD_PTR						FindPrivPattern( DWORD_PTR dwAddress, DWORD_PTR dwSize, PBYTE pbMask, char *pszMask );
	std::size_t						GetModuleEntryDataIndex( std::string strModuleName );

public:
	RemoteProcess( void );
	~RemoteProcess( void );
	bool							Attach( std::string strProcessName, DWORD dwProcessRights = PROCESS_ALL_ACCESS );
	bool							RegisterModule( std::string strModuleName );
	DWORD							GetProcessID( void );
	DWORD_PTR						FindSignature( std::string strModuleName, std::string strSiganture, DWORD_PTR dwExtraOffset = 0 );
	DWORD_PTR						FindPattern( DWORD_PTR dwAddress, DWORD_PTR dwLen, PBYTE pbMask, char *pszMask, DWORD_PTR dwOffset = 0 );
	HANDLE							GetProcessHandle( void );
	HWND							GetWindowHandle( void );
	MODULEENTRY32					GetModuleData( std::string strModuleName );
	std::string						GetExecutablePath( void );
	void							Release( void );
public:
	template< class T >
	BOOL Read( DWORD_PTR dwBaseAddress, T lpBuffer )
	{
		return ReadProcessMemory( this->m_hProcess, ( LPCVOID )( dwBaseAddress ), &lpBuffer, sizeof( T ), NULL );
	}
	template< class T >
	T Read( DWORD_PTR dwBaseAddress )
	{
		T lpBuffer;
		ReadProcessMemory( this->m_hProcess, ( LPCVOID )( dwBaseAddress ), &lpBuffer, sizeof( T ), NULL );
		return lpBuffer;
	}
	template< class T >
	BOOL Write( DWORD_PTR dwBaseAddress, const T lpBuffer )
	{
		return WriteProcessMemory( this->m_hProcess, ( LPVOID )( dwBaseAddress ), &lpBuffer, sizeof( T ), NULL );
	}
};

extern RemoteProcess *g_pRemoteProcess;

#endif