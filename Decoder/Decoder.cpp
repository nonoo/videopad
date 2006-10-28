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
#include "Decoder.h"

CDecoder::CDecoder()
{
	m_bInitialized = false;
	m_nHeaderCount = 0;
}

CDecoder::~CDecoder()
{
	while( !m_qPacketQueue.empty() )
	{
		ogg_packet* pak = m_qPacketQueue.front();
		m_qPacketQueue.pop();
		ogg_packet_clear( pak );
		free( pak );
	}
}

HRESULT CDecoder::PreProcess( ogg_packet* pOggPacket )
{
	return -1;
}

void CDecoder::AddToPacketQueue( ogg_packet* pOggPacket )
{
	// copying given ogg packet into our queue
	//
	ogg_packet* pak = ( ogg_packet* )malloc( sizeof( ogg_packet ) );
	pak->b_o_s = pOggPacket->b_o_s;
	pak->bytes = pOggPacket->bytes;
	pak->e_o_s = pOggPacket->e_o_s;
	pak->granulepos = pOggPacket->granulepos;
	pak->packetno = pOggPacket->packetno;
	pak->packet = ( unsigned char* )malloc( pak->bytes );
	memcpy( pak->packet, pOggPacket->packet, pak->bytes );

	m_qPacketQueue.push( pOggPacket );
}
