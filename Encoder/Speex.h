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

class CSpeex
{
public:
	CSpeex( COggStream* pOggStream, DWORD dwSampleRate, WORD wBitsPerSecond, WORD wChannels );
	~CSpeex();

	void		FeedSample( BYTE* pBuffer, UINT nBufferSize );

private:
				// generates speex header packet
	void		GenerateHeader();
				// generates speex comment packet
	void		GenerateComment();



	DWORD		m_dwSampleRate;
	WORD		m_wBitsPerSecond;
	WORD		m_wChannels;

	COggStream*	m_pOggStream;

				// holds the state of the encoder
	void*		m_pState;

				// holds bits so they can be read and written to by the speex routines
	SpeexBits	m_sBits;

				// frame size for the current speex encoder mode
	UINT		m_nFrameSize;

				// holds data for encoding
	float*		m_pInputBuffer;

				// holds encoded speex data
	char*		m_pEncodedBuffer;

	SpeexPreprocessState*	m_pPreProcState;

	ogg_packet	m_Op;
};
