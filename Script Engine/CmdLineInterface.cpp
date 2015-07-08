#include "CmdLineInterface.h"

std::size_t CmdLineInterface::GetCommandIndex( std::string strCommand )
{
	if( strCommand.empty( ) )
	{
		ENGINE_MSG( "CmdLineInterface::GetCommandIndex: empty command!" );
		return -1;
	}

	for( std::size_t i = 0; i < this->m_vCommands.size( ); i++ )
		if( !this->m_vCommands[ i ].m_strCommand.compare( strCommand ) )
			return i;

	return -1;
}

CmdLineInterface::CmdLineInterface( void )
{

}

CmdLineInterface::~CmdLineInterface( void )
{
	this->Release( );
}

bool CmdLineInterface::GetCommand( std::string strCommand, CmdCommand_t *pCommand )
{
	if( strCommand.empty( ) )
	{
		ENGINE_MSG( "CmdLineInterface::GetCommand: empty command!" );
		return false;
	}

	auto nIndex = this->GetCommandIndex( strCommand );
	if( nIndex == -1 )
	{
		ENGINE_MSG( "CmdLineInterface::GetCommand: %s isn't registered!", strCommand.c_str( ) );
		return false;
	}

	if( pCommand )
		*pCommand = this->m_vCommands[ nIndex ];

	return true;
}

bool CmdLineInterface::GetCommandsFromCmdLine( int iArgC, char *pszArg[] )
{
	std::vector< std::string > vstrCommandInput;
	for( int i = 1; i < iArgC; i++ )
		vstrCommandInput.push_back( pszArg[ i ] );

	if( vstrCommandInput.size( ) / 2 != this->m_vCommands.size( ) )
	{
		ENGINE_MSG( "CmdLineInterface: missing arguemts or parameters!" );
		return false;
	}

	for( std::size_t i = 0; i < vstrCommandInput.size( ); i += 2 )
		if( this->is_command( vstrCommandInput[ i ] ) )
			this->SetCommandValue( vstrCommandInput[ i ], vstrCommandInput[ i + 1 ] );

	return true;
}

bool CmdLineInterface::is_command( std::string strCommand )
{
	if( strCommand.empty( ) )
	{
		ENGINE_MSG( "CmdLineInterface::is_command: empty command!" );
		return false;
	}

	return( this->GetCommand( strCommand, nullptr ) );
}

void CmdLineInterface::SetCommandValue( std::string strCommand, std::string strValue )
{
	if( strCommand.empty( ) )
	{
		ENGINE_MSG( "CmdLineInterface::SetCommandValue: empty command!" );
		return;
	}

	auto nIndex = this->GetCommandIndex( strCommand );
	if( nIndex == -1 )
	{
		ENGINE_MSG( "CmdLineInterface::SetCommand: %s isn't registered!", strCommand.c_str( ) );
		return;
	}

	this->m_vCommands[ nIndex ].m_strValue = strValue;
}

void CmdLineInterface::RegisterCommand( std::string strCommand )
{
	if( strCommand.empty( ) )
	{
		ENGINE_MSG( "CmdLineInterface::RegisterCommand: empty command!" );
		return;
	}

	if( this->GetCommandIndex( strCommand ) != -1 )
	{
		ENGINE_MSG( "CmdLineInterface:RegisterCommand: %s is already registered!", strCommand.c_str( ) );
		return;
	}

	this->m_vCommands.push_back( { strCommand, "" } );
}

void CmdLineInterface::Release( void )
{
	this->m_vCommands.clear( );
}

CmdLineInterface *g_pCmdLineInterface = nullptr;