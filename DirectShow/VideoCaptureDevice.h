#ifndef __VIDEOCAPTUREDEVICE_H
#define __VIDEOCAPTUREDEVICE_H

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
	CVideoCaptureDevice( LPWSTR szID );

	__forceinline UINT GetVideoWidth();
	__forceinline UINT GetVideoHeight();

	void SetPreferredVideoWidth( UINT nWidth );
	void SetPreferredVideoHeight( UINT nHeight );
	void SetPreferredVideoFPS( REFERENCE_TIME rtFPS );

	UINT GetPreferredVideoWidth();
	UINT GetPreferredVideoHeight();
	REFERENCE_TIME GetPreferredVideoFPS();

	// these functions are used by CVideoGraph to set the
	// current output format information
	//
	void SetVideoWidth( UINT nVideoWidth );
	void SetVideoHeight( UINT nVideoHeight );

private:

	UINT m_nVideoWidth;
	UINT m_nVideoHeight;
	REFERENCE_TIME m_rtVideoFPS;

	UINT m_nPreferredImageWidth;
	UINT m_nPreferredImageHeight;
	REFERENCE_TIME m_rtPreferredFPS;
};






__forceinline UINT CVideoCaptureDevice::GetVideoHeight()
{
	return m_nVideoHeight;
}

__forceinline UINT CVideoCaptureDevice::GetVideoWidth()
{
	return m_nVideoWidth;
}

#endif
