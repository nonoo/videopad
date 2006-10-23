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

#include "CaptureDevice.h"

// this class represents a directshow video capture device
//
// init this class by using SetByID() method, or using
// CDirectShowGraph::GetDefaultVideoCaptureDevice()
//
class CVideoCaptureDevice : public CCaptureDevice
{
public:
	CVideoCaptureDevice();

	// init capture device by given COM object ID
	//
	HRESULT					Create( CString szID );

	const UINT&				GetVideoWidth();
	const UINT&				GetVideoHeight();

	void					SetPreferredVideoWidth( UINT nWidth );
	void					SetPreferredVideoHeight( UINT nHeight );
	void					SetPreferredVideoFPS( REFERENCE_TIME rtFPS );

	const UINT&				GetPreferredVideoWidth();
	const UINT&				GetPreferredVideoHeight();
	const REFERENCE_TIME&	GetPreferredVideoFPS();

	// these functions are used by CVideoGraph to set the
	// current output format information
	//
	void					SetVideoWidth( UINT nVideoWidth );
	void					SetVideoHeight( UINT nVideoHeight );

private:
	UINT					m_nVideoWidth;
	UINT					m_nVideoHeight;
	REFERENCE_TIME			m_rtVideoFPS;

	UINT					m_nPreferredImageWidth;
	UINT					m_nPreferredImageHeight;
	REFERENCE_TIME			m_rtPreferredFPS;
};
