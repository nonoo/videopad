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
#include "OggStream.h"

static bool	g_bOggLock = false;

COggStream::COggStream( UINT nSerial )
{
	m_pTCPConnection = NULL;
	m_pUDPConnection = NULL;

	m_bTCPOnly = false;

	m_pData = NULL;
	m_lDataSize = 0;

	m_pStreamState = (ogg_stream_state*) malloc( sizeof( ogg_stream_state ) );

	ogg_stream_init( m_pStreamState, nSerial );
}

COggStream::~COggStream()
{
	ogg_stream_destroy( m_pStreamState );
	SAFE_DELETE_ARRAY( m_pData );
}

void COggStream::PacketIn( ogg_packet* pOggPacket )
{
	ogg_stream_packetin( m_pStreamState, pOggPacket );

	// generating a page for this packet
	//
	ogg_stream_flush( m_pStreamState, &m_Page );

	if( m_lDataSize < m_Page.body_len+m_Page.header_len )
	{
		SAFE_DELETE_ARRAY( m_pData );
		m_lDataSize = 0;
	}

	if( !m_pData )
	{
		m_lDataSize = m_Page.body_len+m_Page.header_len;
		m_pData = new char[m_lDataSize];
	}

	memcpy( m_pData, m_Page.header, m_Page.header_len );
	memcpy( m_pData+m_Page.header_len, m_Page.body, m_Page.body_len );

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

bool COggStream::IsHeaderPacket( ogg_packet* pOggPacket )
{
	return ( pOggPacket->packet[0] & 0x80 ) ? 1 : 0;
}

void COggStream::SetTCPDataConnection( CTCPConnection* pTCPConnection )
{
	m_pTCPConnection = pTCPConnection;
}

void COggStream::SetUDPDataConnection( CUDPConnection* pUDPConnection )
{
	m_pUDPConnection = pUDPConnection;
}

void COggStream::SetTCPOnly( bool bState )
{
	m_bTCPOnly = bState;
}
