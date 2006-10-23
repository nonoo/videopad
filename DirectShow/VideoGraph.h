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

class CDirectShowGraph;
class CVideoCaptureDevice;

class CVideoGraph : public CDirectShowGraph
{
public:
	CVideoGraph( CVideoCaptureDevice& VideoCaptureDevice );
	~CVideoGraph();

	const UINT& GetVideoWidth();
	const UINT& GetVideoHeight();

private:
	// sets the given video format for the graph
	//
	void SetVideoFormat( UINT nPreferredImageWidth, UINT nPreferredImageHeight, REFERENCE_TIME nPreferredFPS );

	CVideoCaptureDevice& VideoCaptureDevice;

	CComQIPtr< IVideoWindow, &IID_IVideoWindow > m_pActiveMovieWindow;
	CComPtr< IBaseFilter > m_pSplitter;
};
