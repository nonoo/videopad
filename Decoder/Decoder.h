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

// interface class
//
class CDecoder : public CWinThread
{
public:
	CDecoder();
	virtual ~CDecoder();

								// PreProcess() returns 0 when pOggPacket is a valid packet to decode
								// and initializes stream with the packet if it hasn't been initialized
								//
								// after init is completed, PreProcess() just puts the packet into the
								// packet queue for further processing by VideoDecoderThread/AudioDecoderThread
								//
	virtual HRESULT				PreProcess( ogg_packet* pOggPacket );

protected:
	void						AddToPacketQueue( ogg_packet* pOggPacket );



	int							m_nHeaderCount;
	bool						m_bInitialized;

	queue<ogg_packet*>			m_qPacketQueue;
};
