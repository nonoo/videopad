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

#pragma once

class COggStream
{
public:
						COggStream( UINT nSerial );
						~COggStream();

						// this OggStream implementation is a PacketIn-only one
						// you add packets and the class generates pages for
						// every packet and sends them using the given connections
						//
						void PacketIn( ogg_packet* pOggPacket );

						// header packets always sent over TCP
						// other packets can be transmitted over UDP (faster)
						//
	void				SetTCPDataConnection( CTCPConnection* pTCPConnection );
	void				SetUDPDataConnection( CUDPConnection* pUDPConnection );

						// sets if every packet should be sent over TCP, not just only
						// header packets
						//
	void				SetTCPOnly( bool bState );

private:
	bool				IsHeaderPacket( ogg_packet* pOggPacket );

	ogg_stream_state	m_StreamState;
	ogg_page			m_Page;

	CTCPConnection*		m_pTCPConnection;
	CUDPConnection*		m_pUDPConnection;

	char*				m_pData;
	long				m_lDataSize;

	bool				m_bTCPOnly;
};
