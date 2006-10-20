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

#include <fstream>
#include <cstdlib>
#include "SettingsFile.h"

void CSettingsFile::Set( CString Section, CString Key, CString Value )
{
    m_Settings[Section][Key] = Value;
	SaveConfig();
}

void CSettingsFile::Set( CString Section, int Key, CString Value )
{
	CString szKey;
	szKey.Format( "%d", Key );
	Set( Section, szKey, Value );
}

CString CSettingsFile::Get( CString Section, CString Key, CString DefaultValue )
{
    if( m_Settings.count( Section ) > 0 )
    {
		if( m_Settings[Section].count( Key ) > 0 )
		{
			return m_Settings[Section][Key];
		}
    }
    return DefaultValue;
}

CString CSettingsFile::Get( CString Section, int Key, CString DefaultValue )
{
	CString szKey;
	szKey.Format( "%d", Key );
	return Get( Section, szKey, DefaultValue );
}

int CSettingsFile::GetInt( CString Section, CString Key, const int& DefaultValue )
{
    if( m_Settings.count( Section ) > 0 )
    {
		if( m_Settings[Section].count( Key ) > 0 )
		{
			char* p;
			int res = strtol( m_Settings[Section][Key], &p, 0 );
			if( *p != 0 ) // the whole CString was not valid
			{
				return DefaultValue;
			}
			return res;
		}
    }
    return DefaultValue;
}

CString CSettingsFile::TrimLeft( CString szString )
{
    CString out="";

    if( szString == "" )
    {
		return out;
    }

    bool bStarted = false;

    for( int i = 0; i < szString.GetLength(); i++ )
    {
		if( ( !bStarted ) && ( szString[i] != ' ' ) )
		{
			bStarted = true;
		}

		if( bStarted )
		{
			out += szString[i];
		}
    }

    return out;
}

CString CSettingsFile::TrimRight( CString szString )
{
    CString out="";

    if( szString == "" )
    {
		return out;
    }

    // searching for the first non-space char from the end of the CString
    //
    int i = szString.GetLength();
    while( ( szString[i] == ' ' ) && ( i > 0 ) )
    {
		i--;
    }

    out = szString.Mid( 0, i );

    return out;
}

void CSettingsFile::LoadConfig()
{
    CString tmp;

	if( m_szConfigFile == "" )
    {
		HRESULT hr = SearchForConfigFile();
		if( FAILED( hr ) )
		{
			return;
		}
    }

    ifstream FileStream( m_szConfigFile );
    if( FileStream.fail() )
    {
		// can't open config file
		return;
    }


    CString szCurrentSection = "";
    m_Settings.clear();


    char buf[500];
    memset( buf, 0, 500 );

    while( !FileStream.eof() )
    {
		FileStream.getline( buf, 499 );
		tmp = buf;

		tmp = TrimLeft( tmp );

		if( tmp[0] == '#' ) // comments
		{
			continue;
		}
		if( tmp == "" ) // empty lines
		{
			continue;
		}

		if( tmp[0] == '[' ) // section
		{
			int loc = tmp.Find( ']' );
			if( loc == -1 )
			{
				tmp = tmp.Mid( 1, tmp.GetLength()-1 );
			}
			else
			{
				tmp = tmp.Mid( 1, loc-1 );
			}

			tmp = TrimRight( TrimLeft( tmp ) );

			if( tmp == "" ) // invalid section
			{
				continue;
			}

			szCurrentSection = tmp;
			continue;
		}

		if( szCurrentSection == "" ) // we're not in a section
		{
			continue;
		}

		// we're in a section, reading key & value pairs
		int loc = tmp.Find( '=' );
		if( loc == -1 ) // no '=' in current line -> invalid line
		{
			continue;
		}

		CString key = tmp.Mid( 0, loc );
		key = TrimRight( key );
		if( key == "" )
		{
			continue;
		}
		CString value = tmp.Mid( loc+1, tmp.GetLength()-loc-1 );
		value = TrimRight( TrimLeft( value ) );
		if( value.GetLength() == 0 )
		{
			continue;
		}

		m_Settings[szCurrentSection][key] = value;
	}

    FileStream.close();
}

HRESULT CSettingsFile::SearchForConfigFile()
{
    // opening config file in current directory
    //
    ifstream FileStream( "VideoPad.ini" );
    if( FileStream.fail() )
    {
		// trying in ../
		//
		FileStream.close();

		FileStream.open( "../VideoPad.ini" );
		if( FileStream.fail() )
		{
			return -1;
	    }
		m_szConfigFile = "../VideoPad.ini";
		return 0;
	}
	m_szConfigFile = "VideoPad.ini";
	return 0;
}

CSettingsFile::CSettingsFile()
{
}

void CSettingsFile::SaveConfig()
{
    ofstream FileStream;
    CString tmp;

	if( m_szConfigFile == "" )
	{
		m_szConfigFile = "VideoPad.ini";
	}

	FileStream.open( m_szConfigFile );
	if( FileStream.fail() )
	{
		MessageBox( NULL, "Can't save config file!", "Error", MB_ICONERROR | MB_OK );
	    FileStream.close();
		return;
	}

    bool fLeadingLine = false;

    for( map< CString, t_mKeys >::iterator it1 = m_Settings.begin(); it1 != m_Settings.end(); it1++ )
    {
		if( fLeadingLine )
		{
			FileStream << endl;
		}
		fLeadingLine = true;

		FileStream << '['  << it1->first << ']' << endl;

		t_mKeys mKeys = it1->second;
		for( t_mKeys::iterator it2 = mKeys.begin(); it2 != mKeys.end(); it2++ )
		{
			FileStream << it2->first << "=" << it2->second << endl;
		}
    }

    FileStream.close();
}
