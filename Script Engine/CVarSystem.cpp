#include "CVarSystem.h"

std::size_t CVarSystem::GetCVarIndex( std::string strCVarName )
{
	if( strCVarName.empty( ) )
	{
		ENGINE_MSG( "CVarSystem::GetCVarIndex: empty CVar name!" );
		return -1;
	}

	for( std::size_t i = 0; i < this->m_vstrCVarName.size( ); i++ )
		if( !this->m_vstrCVarName.at( i ).compare( strCVarName ) )
			return i;

	return -1;
}

CVarSystem::CVarSystem( void )
{

}

CVarSystem::~CVarSystem( void )
{
	this->Release( );
}

bool CVarSystem::GetBool( std::string strCVarName )
{
	return ( this->GetFloat( strCVarName ) >= 1.f );
}

float CVarSystem::GetFloat( std::string strCVarName )
{
	auto nIndex = this->GetCVarIndex( strCVarName );
	if( nIndex == -1 )
	{
		ENGINE_MSG( "CVarSystem::Get: %s isn't registered!" );
		return 0.f;
	}

	return this->m_vflValue.at( nIndex );
}

int CVarSystem::GetInt( std::string strCVarName )
{
	return ( int )this->GetFloat( strCVarName );
}

void CVarSystem::RegisterCVar( std::string strCVarName, bool bActive )
{
	this->RegisterCVar( strCVarName, bActive ? 1.f : 0.f );
}

void CVarSystem::RegisterCVar( std::string strCVarName, float flValue )
{
	if( strCVarName.empty( ) )
	{
		ENGINE_MSG( "CVarSystem::AddCVar: empty CVar name!" );
		return;
	}

	this->m_vflValue.push_back( flValue );
	this->m_vstrCVarName.push_back( strCVarName );
}

void CVarSystem::RegisterCVar( std::string strCVarName, int iValue )
{
	this->RegisterCVar( strCVarName, ( float )iValue );
}

void CVarSystem::SetValue( std::string strCVarName, bool bValue )
{
	this->SetValue( strCVarName, bValue ? 1.f : 0.f );
}

void CVarSystem::SetValue( std::string strCVarName, float flValue )
{
	auto nIndex = this->GetCVarIndex( strCVarName );
	if( nIndex == -1 )
		return;

	this->m_vflValue.at( nIndex ) = flValue;
}

void CVarSystem::SetValue( std::string strCVarName, int iValue )
{
	this->SetValue( strCVarName, ( float )iValue );
}

void CVarSystem::Release( void )
{
	if( !this->m_vflValue.empty( ) )
		this->m_vflValue.clear( );

	if( !this->m_vstrCVarName.empty( ) )
		this->m_vstrCVarName.clear( );
}

CVarSystem* g_pCVarManager = nullptr;