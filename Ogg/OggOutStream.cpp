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
#include "OggOutStream.h"

static bool	g_bOggLock = false;

COggOutStream::COggOutStream( UINT nSerial ) : COggStream( nSerial )
{
	m_nSerial = nSerial;

	m_pTCPConnection = NULL;
	m_pUDPConnection = NULL;

	m_bTCPOnly = false;

	m_pData = NULL;
	m_lDataSize = 0;
}

COggOutStream::~COggOutStream()
{
	SAFE_DELETE_ARRAY( m_pData );
}

void COggOutStream::PacketIn( ogg_packet* pOggPacket )
{
	ogg_stream_packetin( m_pStreamState, pOggPacket );

	// generating a page for this packet (1 packet/page)
	//
	ogg_stream_flush( m_pStreamState, &m_OggPage );

	if( m_lDataSize < m_OggPage.body_len+m_OggPage.header_len )
	{
		SAFE_DELETE_ARRAY( m_pData );
		m_lDataSize = 0;
	}

	if( !m_pData )
	{
		m_lDataSize = m_OggPage.body_len+m_OggPage.header_len;
		m_pData = new char[m_lDataSize];
	}

	memcpy( m_pData, m_OggPage.header, m_OggPage.header_len );
	memcpy( m_pData+m_OggPage.header_len, m_OggPage.body, m_OggPage.body_len );

	// header packets are sent over TCP, everything else via UDP
	//
	m_bTCPOnly = true;
	if( ( IsHeaderPacket( pOggPacket ) ) || ( m_bTCPOnly == true ) )
	{
		if( m_pTCPConnection != NULL )
		{
			m_pTCPConnection->SendData( m_pData, m_lDataSize );
		}
	}
	else
	{
		if( m_pUDPConnection != NULL )
		{
			m_pUDPConnection->SendData( m_pData, m_lDataSize );
		}
	}
}

void COggOutStream::SetTCPDataConnection( CTCPConnection* pTCPConnection )
{
	m_pTCPConnection = pTCPConnection;
}

void COggOutStream::SetUDPDataConnection( CUDPConnection* pUDPConnection )
{
	m_pUDPConnection = pUDPConnection;
}

void COggOutStream::SetTCPOnly( bool bState )
{
	m_bTCPOnly = bState;
}
