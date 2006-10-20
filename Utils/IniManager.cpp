//  This file is part of VideoPad.
//
//  VideoPad is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  VideoPad is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with VideoPad; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include "IniManager.h"

CIniManager::CIniManager()
{
	m_IniReader.setINIFileName( "..\\VideoPad\\VideoPad.ini" );
}

CIniManager::~CIniManager()
{

}

void CIniManager::SaveAutoJoin( bool bValue )
{
	if ( bValue )
	{
		m_IniReader.setKey( "true", "AutoJoin", "OPTIONS" );
	}
	else
	{
		m_IniReader.setKey( "false", "AutoJoin", "OPTIONS" );
	}
	
}
bool CIniManager::LoadAutoJoin()
{
	if ( m_IniReader.getKeyValue( "AutoJoin", "OPTIONS" ) == "true" )
	{
		return true;
	}
	return false;
}

void CIniManager::SaveAutoJoinChannelName( CString sChannelName )
{
	m_IniReader.setKey( sChannelName, "AutoJoinChannelName", "OPTIONS" );
}

CString CIniManager::LoadAutoJoinChannelName()
{
	return m_IniReader.getKeyValue(  "AutoJoinChannelName", "OPTIONS" );
}

void CIniManager::SaveRecentServerName( CString sServerName )
{
	CString sName[RECENTSERVERLISTLENGTH];
	sName[0] = m_IniReader.getKeyValue( "RecentServerName0", "OPTIONS" );

	if ( sName[0] == sServerName )
	{
		return;
	}
	
	sName[1] = m_IniReader.getKeyValue( "RecentServerName1", "OPTIONS" );
	sName[2] = m_IniReader.getKeyValue( "RecentServerName2", "OPTIONS" );
	sName[3] = m_IniReader.getKeyValue( "RecentServerName3", "OPTIONS" );	
	
	CString sKey;
	for ( int i = 1; i < RECENTSERVERLISTLENGTH; i++ )
	{
		if ( sServerName == sName[i] )
		{
			for ( int i2 = i; i2 > 0; i2-- )
			{
				sKey.Format( "RecentServerName%i", i2 ); 
				m_IniReader.setKey( sName[i2-1], sKey, "OPTIONS" );
			}
			sKey.Format( "RecentServerName%i", 0 ); 
			m_IniReader.setKey( sName[i], sKey, "OPTIONS" );
			return;
		}
	}
	
	for ( int i = RECENTSERVERLISTLENGTH - 1; i > 0; i-- )
	{
		sKey.Format( "RecentServerName%i", i ); 
		m_IniReader.setKey( sName[i-1], sKey, "OPTIONS" );
	}
	
	m_IniReader.setKey( sServerName, "RecentServerName0", "OPTIONS" );
	
}

CString CIniManager::LoadRecentServerName( int iInd )
{
	if ( iInd > RECENTSERVERLISTLENGTH-1 || iInd < 0 )
	{
		return "";
	}
	
	CString sKey;
	sKey.Format( "RecentServerName%i", iInd ); 
	return m_IniReader.getKeyValue( sKey, "OPTIONS" );
	
}

void CIniManager::SaveMaxLineNum( int iMaxLineNum )
{
	CString sKey;
	sKey.Format( "%i", iMaxLineNum );
	m_IniReader.setKey( sKey, "Max_Line_Num", "OPTIONS" );
}

int CIniManager::LoadMaxLineNum()
{
	return atoi( m_IniReader.getKeyValue( "Max_Line_Num", "OPTIONS" ) );	
}
