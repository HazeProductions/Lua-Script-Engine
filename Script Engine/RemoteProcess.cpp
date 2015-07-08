#include "RemoteProcess.h"

bool RemoteProcess::DataCompare( const unsigned char *pbData, const unsigned char *pbMask, const char *pszMask )
{
	for( ; *pszMask; ++pszMask, ++pbData, ++pbMask )
	{
		if( *pszMask == 'x' && *pbData != *pbMask )
			return false;
	}

	return ( *pszMask ) == NULL;
}

DWORD_PTR RemoteProcess::FindPrivPattern( DWORD_PTR dwAddress, DWORD_PTR dwSize, PBYTE pbMask, char *pszMask )
{
	for( DWORD_PTR i = 0; i < dwSize; i++ )
	{
		if( this->DataCompare( PBYTE( dwAddress + i ), pbMask, pszMask ) )
			return dwAddress + i;
	}

	return 0;
}

std::size_t RemoteProcess::GetModuleEntryDataIndex( std::string strModuleName )
{
	if( strModuleName.empty( ) )
	{
		ENGINE_MSG( "RemoteProcess::GetModuleEntryDataIndex: empty module name!" );
		return -1;
	}

#ifdef _UNICODE
	std::wstring wstrModuleName( strModuleName.begin( ), strModuleName.end( ) );
#endif

	auto bComparedModuleName = false;
	for( std::size_t i = 0; i < this->m_vModuleEntryData.size( ); i++ )
	{
#ifdef _UNICODE
		bComparedModuleName = ( !wstrModuleName.compare( this->m_vModuleEntryData[ i ].szModule ) );
#else
		bComparedModuleName = ( !strModuleName.compare( this->m_vModuleEntryData[ i ].szModule ) );
#endif

		if( bComparedModuleName )
			return i;
	}

	return -1;
}

RemoteProcess::RemoteProcess( void )
{
	this->m_bAttached = false;
	this->m_dwProcessId = 0;
	this->m_hProcess = NULL;
}

RemoteProcess::~RemoteProcess( void )
{
	this->Release( );
}

bool RemoteProcess::Attach( std::string strProcessName, DWORD dwProcessRights )
{
	if( this->m_bAttached )
		return true;

	if( ( this->m_dwProcessId = GetProcessIdByName( strProcessName ) ) == 0 )
	{
		ENGINE_MSG( "RemoteProcess::Attach: failed to find %s", strProcessName.c_str( ) );
		return false;
	}

	if( ( this->m_hProcess = OpenProcess( dwProcessRights, FALSE, this->m_dwProcessId ) ) == INVALID_HANDLE_VALUE )
	{
		ENGINE_MSG( "RemoteProcess::Attach: failed to open process!" );
		return false;
	}
	
	if( !( this->m_hWindow = GetWindowHandleFromProcessId( this->m_dwProcessId ) ) )
	{
		ENGINE_MSG( "RemoteProcess::Attach: failed to find window handle!\n" );
		return false;
	}

	char pszBuffer[ MAX_PATH ];
	GetModuleFileNameExA( this->m_hProcess, NULL, pszBuffer, MAX_PATH );
	this->m_strExecutablePath = pszBuffer;

	return ( this->m_bAttached = true );
}

DWORD RemoteProcess::GetProcessID( void )
{
	return this->m_dwProcessId;
}

DWORD_PTR RemoteProcess::FindSignature( std::string strModuleName, std::string strSignature, DWORD_PTR dwExtraOffset )
{
	if( !this->m_bAttached )
		return 0;

	if( strModuleName.empty( ) || strSignature.empty( ) )
	{
		ENGINE_MSG( "RemoteProcess::FindSignature: empty arg data" );
		return 0;
	}

	auto nIndex = this->GetModuleEntryDataIndex( strModuleName );
	if( nIndex == -1 )
	{
		ENGINE_MSG( "RemoteProcess::FindSignature: %s isn't registered!", strModuleName.c_str( ) );
		return 0;
	}

	auto ModuleEntryData = this->m_vModuleEntryData[ nIndex ];
	std::size_t nPos = 0;
	std::string strMask;
	std::vector< BYTE > vBytes;
	
	while( true )
	{
		nPos = strSignature.find_first_of( " \n\0" );
		if( nPos != std::string::npos )
		{
			auto bStringPos = ( nPos != std::string::npos );
			bool bLast = ( !strSignature.empty( ) );
			auto strCurrentByte = strSignature.substr( 0, nPos );
			strSignature.erase( 0, nPos + 1 );

			if( !strCurrentByte.compare( "??" ) )
			{
				vBytes.push_back( 0 );
				strMask.append( "?" );
			}
			else
			{
				strMask.append( "x" );
				vBytes.push_back( ( BYTE )std::strtoul( ( std::string( "0x" ) + strCurrentByte ).c_str( ), NULL, 0 ) );
			}
		}
		else
		{
			if( !strSignature.empty( ) )
			{
				if( !strSignature.compare( "??" ) )
				{
					vBytes.push_back( 0 );
					strMask.append( "?" );
				}
				else
				{
					strMask.append( "x" );
					vBytes.push_back( ( BYTE )std::strtoul( ( std::string( "0x" ) + strSignature ).c_str( ), NULL, 0 ) );
				}
			}

			break;
		}
	}	
	
	auto nNumberOfBytes = vBytes.size( );
	auto pbPattern = new BYTE[ vBytes.size( ) ];
	for( std::size_t i = 0; i < nNumberOfBytes; i++ )
		pbPattern[ i ] = vBytes[ i ];

	auto pszMask = ( char * )strMask.c_str( );
	auto dwOffset = this->FindPattern( ( DWORD_PTR )ModuleEntryData.modBaseAddr, ( DWORD_PTR )ModuleEntryData.modBaseSize, pbPattern, pszMask, dwExtraOffset );
	
	delete pbPattern;

	return dwOffset;

}

DWORD_PTR RemoteProcess::FindPattern( DWORD_PTR dwAddress, DWORD_PTR dwLen, PBYTE pbMask, char *pszMask, DWORD_PTR dwOffset )
{
	auto dwCurrentAddress = dwAddress;
	auto pbPage = new BYTE[ 0x1000 ];
	
	do
	{
		if( ReadProcessMemory( this->m_hProcess, ( LPCVOID )( dwAddress ), pbPage, 0x1000, NULL ) )
		{
			DWORD dwPrivPattern = this->FindPrivPattern( DWORD( pbPage ), 0x1000, pbMask, pszMask );

			if( dwPrivPattern )
			{
				if( dwOffset != 0 )
					dwPrivPattern += dwOffset;
				dwPrivPattern -= DWORD( pbPage );
				dwPrivPattern += dwAddress;

				delete pbPage;

				return dwPrivPattern;
			}

			dwAddress += 0x1000;
		}
	} while( dwAddress < dwCurrentAddress + dwLen );

	delete pbPage;

	return 0;
}

HANDLE RemoteProcess::GetProcessHandle( void )
{
	return this->m_hProcess;
}

HWND RemoteProcess::GetWindowHandle( void )
{
	return this->m_hWindow;
}

bool RemoteProcess::RegisterModule( std::string strModuleName )
{
	if( strModuleName.empty( ) )
	{
		ENGINE_MSG( "RemoteProcess::RegisterModule: empty module name!" );
		return 0;
	}

	MODULEENTRY32 ModuleEntry;
	if( GetRemoteModuleEntryData( strModuleName, this->m_dwProcessId, &ModuleEntry ) )
	{
		this->m_vModuleEntryData.push_back( ModuleEntry );
		return true;
	}

	ENGINE_MSG( "RemoteProcess::RegisterModule: %s isn't registered!\n", strModuleName.c_str( ) );
	return false;
}

MODULEENTRY32 RemoteProcess::GetModuleData( std::string strModuleName )
{
	static MODULEENTRY32 InvalidModule = { NULL };

	auto nIndex = this->GetModuleEntryDataIndex( strModuleName );
	if( nIndex == -1 )
	{
		ENGINE_MSG( "RemoteProcess::GetModuleData: %s isn't registered!", strModuleName.c_str( ) );
		return InvalidModule;
	}

	return this->m_vModuleEntryData[ nIndex ];
}

std::string RemoteProcess::GetExecutablePath( void )
{
	return this->m_strExecutablePath;
}

void RemoteProcess::Release( void )
{
	if( this->m_bAttached )
	{
		CloseHandle( this->m_hProcess );
		this->m_dwProcessId = 0;		
		this->m_hProcess = NULL;
		this->m_hWindow = NULL;
		this->m_strExecutablePath.clear( );
		this->m_vModuleEntryData.clear( );		
	}
}

RemoteProcess *g_pRemoteProcess = nullptr;