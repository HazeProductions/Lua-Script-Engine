#ifndef __CMDLINEINTERFACE_H__
#define __CMDLINEINTERFACE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "stdafx.h"

typedef struct
{
	std::string	m_strCommand;
	std::string	m_strValue;
}CmdCommand_t;

class CmdLineInterface
{
private:
	std::vector< CmdCommand_t >	m_vCommands;

private:
	std::size_t					GetCommandIndex( std::string strCommand );

public:
	CmdLineInterface( void );
	~CmdLineInterface( void );
	bool						GetCommand( std::string strCommand, CmdCommand_t *pCommand );
	bool						GetCommandsFromCmdLine( int iArgC, char *pszArg[] );
	bool						is_command( std::string strCommand );
	void						SetCommandValue( std::string strCommand, std::string strValue );
	void						RegisterCommand( std::string strCommand );
	void						Release( void );

public:
	
};

extern CmdLineInterface *g_pCmdLineInterface;

#endif