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

// theora video encoder class
//
class CTheora
{
public:
	// the encoder outputs encoded data to the given ogg stream
	//
	CTheora( COggOutStream* pOggOutStream, UINT frame_x, UINT frame_y, UINT fps );
	~CTheora();

	// encodes given image and puts into the ogg stream
	//
	void FeedFrame( BYTE* pData, UINT lBufferSize );

private:
	void ConvertImageRGB2YUV420( BYTE* pData, BYTE* pYData, BYTE* pUData, BYTE* pVData, UINT nWidth, UINT nHeight );

	COggOutStream*		m_pOggOutStream;

	ogg_packet*			m_pOggPacket;

	theora_state		td;
	theora_info			ti;
	theora_comment		tc;

	int					m_video_x;	// video size
	int					m_video_y;
	int					m_frame_x;	// input frame size
	int					m_frame_y;
	int					m_frame_x_offset;
	int					m_frame_y_offset;
	int					m_fps;

	BYTE*				m_pImageDataY;
	BYTE*				m_pImageDataU;
	BYTE*				m_pImageDataV;

	UINT				m_nImageDataSize;
};
