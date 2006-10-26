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
#include "Theora.h"

CTheora::CTheora( COggStream* pOggStream, UINT frame_x, UINT frame_y, UINT fps )
{
	m_pOggStream = pOggStream;

	m_frame_x = frame_x;
	m_frame_y = frame_y;
	m_fps = fps;

	m_frame_x_offset = 0;
	m_frame_y_offset = 0;

	// Theora has a divisible-by-sixteen restriction for the encoded video size
	// scale the frame size up to the nearest /16 and calculate offsets
	//
	m_video_x = ( ( frame_x + 15 ) >> 4 ) << 4;
	m_video_y = ( ( frame_y + 15 ) >> 4 ) << 4;

	// We force the offset to be even.
	// This ensures that the chroma samples align properly with the luma
	// samples.
	//
	m_frame_x_offset = ( ( m_video_x-frame_x ) / 2 ) & ~1;
	m_frame_y_offset = ( ( m_video_y-frame_y ) / 2 ) & ~1;

	m_pImageDataY = new BYTE[m_video_x*m_video_y];
	m_pImageDataU = new BYTE[m_video_x*m_video_y/2];
	m_pImageDataV = new BYTE[m_video_x*m_video_y/2];

	m_nImageDataSize = frame_x * frame_y * 3;

	// clear initial frame as it may be larger than actual video data
	// fill Y plane with 0x10 and UV planes with 0x80, for black data
	//
	memset( m_pImageDataY, 0x10, m_video_x*m_video_y );
	memset( m_pImageDataU, 0x80, m_video_x*m_video_y/4 );
	memset( m_pImageDataV, 0x80, m_video_x*m_video_y/4 );

	theora_info_init( &ti );
	ti.width = m_video_x;
	ti.height = m_video_y;
	ti.frame_width = frame_x;
	ti.frame_height = frame_y;
	ti.offset_x = m_frame_x_offset;
	ti.offset_y = m_frame_y_offset;
	ti.fps_numerator = fps;
	ti.fps_denominator = 1;
	ti.aspect_numerator = m_video_x;
	ti.aspect_denominator = m_video_y;
	ti.colorspace = OC_CS_UNSPECIFIED;
	ti.pixelformat = OC_PF_420;
	ti.target_bitrate = 1;
	ti.quality = 0;

	ti.dropframes_p = 0;
	ti.quick_p = 1;
	ti.keyframe_auto_p = 0;
	ti.keyframe_mindistance = 8;
	ti.keyframe_frequency = 65535;
	ti.keyframe_frequency_force = 65535;
	//ti.keyframe_data_target_bitrate=(UINT)(ti.target_bitrate*1.5f);
	ti.keyframe_data_target_bitrate = ti.target_bitrate;
	ti.keyframe_auto_threshold = 80;
	ti.noise_sensitivity = 1;
	ti.sharpness = 0; // 2 - less sharp, less bandwidth

	theora_encode_init( &td, &ti );
	theora_info_clear( &ti );

	m_pOggPacket = new ogg_packet;

	// header packet
	//
	theora_encode_header( &td, m_pOggPacket );
	m_pOggStream->PacketIn( m_pOggPacket );

	theora_comment_init( &tc );
	theora_encode_comment( &tc, m_pOggPacket );
	m_pOggPacket->granulepos = 0;
	m_pOggStream->PacketIn( m_pOggPacket );
	free( m_pOggPacket->packet ); // needed because theora_encode_comment does not free it's internal buffer

	theora_encode_tables( &td, m_pOggPacket );
	m_pOggPacket->granulepos = 0;
	m_pOggStream->PacketIn( m_pOggPacket );
}

// feeds a frame to the ogg theora encoder stream
//
void CTheora::FeedFrame( BYTE* pData, UINT lBufferSize )
{
	yuv_buffer yuv;

	ConvertImageRGB2YUV420(pData, m_pImageDataY, m_pImageDataU, m_pImageDataV, m_frame_x, m_frame_y);

	yuv.y_width = m_video_x;
	yuv.y_height = m_video_y;
	yuv.y_stride = m_video_x;

	yuv.uv_width = m_video_x/2;
	yuv.uv_height = m_video_y/2;
	yuv.uv_stride = m_video_x/2;

	yuv.y = m_pImageDataY;
	yuv.u = m_pImageDataU;
	yuv.v = m_pImageDataV;

	theora_encode_YUVin( &td,&yuv );

	// Theora is a one-frame-in, one-frame-out system; submit a frame
	// for compression and pull out the packet
	//
	theora_encode_packetout( &td, 0, m_pOggPacket );

	// this is not a header packet so we adjust granulepos if needed
	//
	if( m_pOggPacket->granulepos == 0 )
	{
		m_pOggPacket->granulepos = 1;
	}

	m_pOggStream->PacketIn( m_pOggPacket );
}

CTheora::~CTheora()
{
	SAFE_DELETE( m_pOggPacket );

	theora_clear( &td );

	SAFE_DELETE_ARRAY( m_pImageDataY );
	SAFE_DELETE_ARRAY( m_pImageDataU );
	SAFE_DELETE_ARRAY( m_pImageDataV );
}

void CTheora::ConvertImageRGB2YUV420( BYTE* pData, BYTE* pYData, BYTE* pUData, BYTE* pVData, UINT nWidth, UINT nHeight )
{
	BYTE r;
	BYTE g;
	BYTE b;
	UINT j = 0;
	UINT u = 0;
	UINT i;
	UINT y;
	UINT x;
	BOOL bEvenLine = false;
	BOOL bEvenPixel = false;

	// flipping image vertically
	for( y = nHeight; y > 0; y-- )
	{
		i = y*nWidth*3;

		for( x = 0; x < nWidth; x++, j++, i += 3 )
		{

			r = pData[i];
			g = pData[i+1];
			b = pData[i+2];

			pYData[j] = (BYTE)( 0.299 * r + 0.587 * g + 0.114 * b );

			// horizontal, vertical 2:1 downsampling
			if( bEvenPixel && bEvenLine )
			{
				pVData[u] = (BYTE)( 128 - 0.168736 * r - 0.331264 * g + 0.5 * b );
				pUData[u] = (BYTE)( 128 + 0.5 * r - 0.418688 * g - 0.081312 * b );
				u++;
			}
			bEvenPixel = !bEvenPixel;
		}
		bEvenLine = !bEvenLine;
	}
}
