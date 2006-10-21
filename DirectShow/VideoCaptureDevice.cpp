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

UINT CVideoCaptureDevice::GetPreferredVideoWidth()
{
	return m_nPreferredImageWidth;
}

UINT CVideoCaptureDevice::GetPreferredVideoHeight()
{
	return m_nPreferredImageHeight;
}

REFERENCE_TIME CVideoCaptureDevice::GetPreferredVideoFPS()
{
	return m_rtPreferredFPS;
}
