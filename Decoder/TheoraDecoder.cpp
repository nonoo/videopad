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
#include "TheoraDecoder.h"

CTheoraDecoder::CTheoraDecoder()
{
	theora_info_init( &m_TheoraInfo );
	theora_comment_init( &m_TheoraComment );
}

CTheoraDecoder::~CTheoraDecoder()
{
	theora_comment_clear( &m_TheoraComment );
	theora_info_clear( &m_TheoraInfo );
	if( m_bInitialized )
	{
		theora_clear( &m_TheoraState );
	}
}

HRESULT CTheoraDecoder::PreProcess( ogg_packet* pOggPacket )
{
	if( !m_bInitialized )
	{
		// still initializing (we haven't got all of the needed header packets yet)
		//
		int res = theora_decode_header( &m_TheoraInfo, &m_TheoraComment, pOggPacket );
		if( res == 0 )
		{
			m_nHeaderCount++;
			if( m_nHeaderCount == 3 )
			{
				// theora init has completed after 3 header packets
				//
				m_bInitialized = true;

				theora_decode_init( &m_TheoraState, &m_TheoraInfo );
			}
		}
		return res;
	}

	AddToPacketQueue( pOggPacket );

	return 0;
}
