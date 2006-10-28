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

COggStream::COggStream( UINT nSerial )
{
	m_nSerial = nSerial;

	m_pStreamState = (ogg_stream_state *) malloc( sizeof( ogg_stream_state ) );

	ogg_stream_init( m_pStreamState, nSerial );
	m_pOggPacket = (ogg_packet *) malloc( sizeof( ogg_packet ) );

	m_bInitialized = false;
	m_pDecoder = NULL;
}

COggStream::~COggStream()
{
	ogg_stream_destroy( m_pStreamState );
}

bool COggStream::IsHeaderPacket( ogg_packet* pOggPacket )
{
	return ( pOggPacket->granulepos == 0 ) ? 1 : 0;
}

HRESULT COggStream::FeedPage( ogg_page& OggPage )
{
	ogg_stream_pagein( m_pStreamState, &OggPage );
	memset( m_pOggPacket, 0, sizeof( ogg_packet ) );
	m_pOggPacket->granulepos = 1; // because packetout doesn't fill out this
	ogg_stream_packetout( m_pStreamState, m_pOggPacket );

	if( m_pDecoder == NULL ) // we don't have a decoder associated with this stream yet
	{
		if( !IsHeaderPacket( m_pOggPacket ) )
		{
			// we don't care about non-header packets
			//
			return -1;
		}

		CTheoraDecoder* pTheoraDecoder = new CTheoraDecoder;
		CSpeexDecoder* pSpeexDecoder = new CSpeexDecoder;
		if( SUCCEEDED( pTheoraDecoder->PreProcess( m_pOggPacket ) ) )
		{
			// this is a theora stream
			//
			m_pDecoder = pTheoraDecoder;
			SAFE_DELETE( pSpeexDecoder );
		}

		if( pSpeexDecoder != NULL )
		{
			if( SUCCEEDED( pSpeexDecoder->PreProcess( m_pOggPacket ) ) )
			{
				// this is a speex stream
				//
				m_pDecoder = pSpeexDecoder;
				SAFE_DELETE( pTheoraDecoder );
			}
		}

		if( m_pDecoder == NULL ) // identifying the decoder of this stream has failed
		{
			return -1;
		}
		return 0;
	}

	m_pDecoder->PreProcess( m_pOggPacket );
	return 0;
}

const UINT& COggStream::GetSerial()
{
	return m_nSerial;
}
