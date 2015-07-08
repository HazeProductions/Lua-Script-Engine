#ifndef __CVARSYSTEM_H__
#define __CVARSYSTEM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "stdafx.h"

class CVarSystem
{
private:
	std::vector< std::string >	m_vstrCVarName;
	std::vector< float >		m_vflValue;

private:
	std::size_t GetCVarIndex( std::string strCVarName );

public:	
	CVarSystem( void );
	~CVarSystem( void );
	bool		GetBool( std::string strCVarName );
	float		GetFloat( std::string strCVarName );
	int			GetInt( std::string strCVarName );
	void		RegisterCVar( std::string strCVarName, bool bActive );
	void		RegisterCVar( std::string strCVarName, float flValue );
	void		RegisterCVar( std::string strCVarName, int iValue );
	void		SetValue( std::string strCVarName, bool bValue );
	void		SetValue( std::string strCVarName, float flValue );
	void		SetValue( std::string strCVarName, int iValue );
	void		Release( void );
};

extern CVarSystem* g_pCVarManager;

#endif