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
#include "Speex.h"

void CSpeex::GenerateHeader()
{
	m_Op.b_o_s = 1;
	m_Op.e_o_s = 0;
	m_Op.granulepos = 0;

	SpeexHeader SpxHdr;
	speex_init_header( &SpxHdr, m_dwSampleRate, m_wChannels, &speex_nb_mode );
	int nPacketSize;
	m_Op.packet = (unsigned char *)speex_header_to_packet( &SpxHdr, &nPacketSize );
	m_Op.bytes = nPacketSize;
	m_Op.granulepos = 0;

	m_pOggStream->PacketIn( &m_Op );
	free( m_Op.packet );
}

void CSpeex::GenerateComment()
{
	char* vendor_string = "VideoPad";
	unsigned __int32 vendor_length = 8;
	unsigned __int32 user_comment_list_length = 0;
	UINT nPacketSize = vendor_length+sizeof(unsigned __int32)+sizeof(unsigned __int32);

	m_Op.packet = (unsigned char *)malloc( nPacketSize );
	memcpy( m_Op.packet, &vendor_length, sizeof(unsigned __int32) );
	memcpy( m_Op.packet+sizeof(unsigned __int32), vendor_string, vendor_length );
	memcpy( m_Op.packet+sizeof(unsigned __int32)+vendor_length, &user_comment_list_length, sizeof(unsigned __int32) );

	m_Op.b_o_s = 0;
	m_Op.e_o_s = 0;
	m_Op.granulepos = 0;

	m_Op.bytes = nPacketSize;

	m_pOggStream->PacketIn( &m_Op );
	free( m_Op.packet );
}

CSpeex::CSpeex( COggStream* pOggStream, DWORD dwSampleRate, WORD wBitsPerSecond, WORD wChannels )
{
	m_pOggStream = pOggStream;
	m_dwSampleRate = dwSampleRate;
	m_wBitsPerSecond = wBitsPerSecond;
	m_wChannels = wChannels;

	// initializing narrow-band mode
	//
	m_pState = speex_encoder_init( &speex_nb_mode );

	// setting encoder options
	int tmp = 10000;
	speex_encoder_ctl( m_pState, SPEEX_SET_ABR, &tmp);
	speex_encoder_ctl( m_pState, SPEEX_SET_BITRATE, &tmp);
	tmp = 1;
	speex_encoder_ctl( m_pState, SPEEX_SET_VBR, &tmp);
	speex_encoder_ctl( m_pState, SPEEX_SET_DTX, &tmp);
	tmp = 10;
	speex_encoder_ctl( m_pState, SPEEX_SET_VBR_QUALITY, &tmp);
	tmp = 10;
	speex_encoder_ctl( m_pState, SPEEX_SET_COMPLEXITY, &tmp);

	speex_encoder_ctl( m_pState, SPEEX_GET_FRAME_SIZE, &m_nFrameSize);

	speex_encoder_ctl( m_pState, SPEEX_SET_SAMPLING_RATE, &dwSampleRate);
	speex_encoder_ctl( m_pState, SPEEX_GET_SAMPLING_RATE, &tmp);
	m_pPreProcState = speex_preprocess_state_init( m_nFrameSize, tmp );

	// setting preprocessor options
	tmp = 1;
	speex_preprocess_ctl( m_pPreProcState, SPEEX_PREPROCESS_SET_DENOISE, &tmp );
	speex_preprocess_ctl( m_pPreProcState, SPEEX_PREPROCESS_SET_AGC, &tmp );
	speex_preprocess_ctl( m_pPreProcState, SPEEX_PREPROCESS_SET_VAD, &tmp );

	// initialization of the structure that holds the bits
	speex_bits_init( &m_sBits );

	m_dwSampleRate = dwSampleRate;
	m_wBitsPerSecond = wBitsPerSecond;
	m_wChannels = wChannels;

	m_pEncodedBuffer = new char[m_nFrameSize*2];

	GenerateHeader();
	GenerateComment();

	m_nGranulePos = 0;
}

void CSpeex::FeedSample( BYTE* pBuffer, UINT nBufferSize )
{
	for( UINT i=0; i < nBufferSize/2; i += m_nFrameSize )
	{
		// preprocess the audio frame
		speex_preprocess( m_pPreProcState, (short *)(pBuffer)+i, NULL);

		// flush all the bits in the struct so we can encode a new frame
		speex_bits_reset( &m_sBits );

		// encode the frame
		speex_encode_int( m_pState, (short *)(pBuffer)+i, &m_sBits );

		UINT nBytesEncoded = speex_bits_write( &m_sBits, m_pEncodedBuffer, m_nFrameSize*2);

		m_Op.packet = (unsigned char*)m_pEncodedBuffer;
		m_Op.bytes = nBytesEncoded;
		m_Op.b_o_s = 0;
		m_Op.e_o_s = 0;
		m_Op.granulepos = ++m_nGranulePos;
		m_pOggStream->PacketIn( &m_Op );
	}
}

CSpeex::~CSpeex()
{
	// destroy the encoder state
	speex_encoder_destroy( m_pState );

	// destroy the bit-packing struct
	speex_bits_destroy( &m_sBits );

	speex_preprocess_state_destroy( m_pPreProcState );

	SAFE_DELETE_ARRAY( m_pEncodedBuffer );
}
