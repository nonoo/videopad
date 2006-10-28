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
#include "SerialMapper.h"

void CSerialMapper::AddSerial( unsigned int nSerial, CClient* pClient, COggStream* pOggStream )
{
	m_SerialMap[nSerial].first = pClient;
	m_SerialMap[nSerial].second = pOggStream;
}

void CSerialMapper::DeleteClientSerials( CClient* pClient )
{
    for( tSerialMap::iterator it = m_SerialMap.begin();	it != m_SerialMap.end(); it++ )
    {
		if( it->second.first == pClient )
		{
			m_SerialMap.erase( it );
		}
    }
}

CClient* CSerialMapper::GetClient( const unsigned int& nSerial )
{
    return m_SerialMap[nSerial].first;
}

COggStream* CSerialMapper::GetOggStream( const unsigned int& nSerial )
{
    return m_SerialMap[nSerial].second;
}

void CSerialMapper::DestroyStreams()
{
	for( tSerialMap::iterator it = m_SerialMap.begin();	it != m_SerialMap.end(); it++ )
	{
		SAFE_DELETE( it->second.second );
	}
}

void CSerialMapper::DestroyStream( COggStream*& pOggStream )
{
	m_SerialMap.erase( m_SerialMap.find( pOggStream->GetSerial() ) );
	SAFE_DELETE( pOggStream );
}
