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
#include "VideoCaptureDevice.h"

CVideoCaptureDevice::CVideoCaptureDevice()
{
	m_nVideoWidth = 0;
	m_nVideoHeight = 0;
	m_rtVideoFPS = 0;

	m_nPreferredImageWidth = 0;
	m_nPreferredImageHeight = 0;
	m_rtPreferredFPS = 0;
}

CVideoCaptureDevice::CVideoCaptureDevice( LPWSTR szID ) : CCaptureDevice( szID )
{
	return;
}

void CVideoCaptureDevice::SetVideoWidth( UINT nVideoWidth )
{
	m_nVideoWidth = nVideoWidth;
}

void CVideoCaptureDevice::SetVideoHeight( UINT nVideoHeight )
{
	m_nVideoHeight = nVideoHeight;
}

void CVideoCaptureDevice::SetPreferredVideoWidth( UINT nWidth )
{
	m_nPreferredImageWidth = nWidth;
}

void CVideoCaptureDevice::SetPreferredVideoHeight( UINT nHeight )
{
	m_nPreferredImageHeight = nHeight;
}

void CVideoCaptureDevice::SetPreferredVideoFPS( REFERENCE_TIME rtFPS )
{
	m_rtPreferredFPS = rtFPS;
}

const UINT& CVideoCaptureDevice::GetPreferredVideoWidth()
{
	return m_nPreferredImageWidth;
}

const UINT& CVideoCaptureDevice::GetPreferredVideoHeight()
{
	return m_nPreferredImageHeight;
}

const REFERENCE_TIME& CVideoCaptureDevice::GetPreferredVideoFPS()
{
	return m_rtPreferredFPS;
}

const UINT& CVideoCaptureDevice::GetVideoHeight()
{
	return m_nVideoHeight;
}

const UINT& CVideoCaptureDevice::GetVideoWidth()
{
	return m_nVideoWidth;
}
