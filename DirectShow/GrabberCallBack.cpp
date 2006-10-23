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
#include "GrabberCallBack.h"

// Note: this object is a SEMI-COM object, and can only be created statically.
// We use this little semi-com object to handle the sample-grab-callback,
// since the callback must provide a COM interface. We could have had an interface
// where you provided a function-call callback, but that's really messy, so we
// did it this way. You can put anything you want into this C++ object, even
// a pointer to a CDialog. Be aware of multi-thread issues though.
//

// fake out any COM ref counting
//
STDMETHODIMP_(ULONG) CGrabberCallBack::AddRef() { return 2; }
STDMETHODIMP_(ULONG) CGrabberCallBack::Release() { return 1; }

// fake out any COM QI'ing
//
STDMETHODIMP CGrabberCallBack::QueryInterface(REFIID riid, void ** ppv)
{
	if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
	{
		*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
		return NOERROR;
	}    
	return E_NOINTERFACE;
}

// we don't implement this interface
//
STDMETHODIMP CGrabberCallBack::SampleCB( double SampleTime, IMediaSample* pSample )
{
	return 0;
}

// The sample grabber is calling us back on its deliver thread.
// This is NOT the main app thread!
//
//           !!!!! WARNING WARNING WARNING !!!!!
//
// On Windows 9x systems, you are not allowed to call most of the 
// Windows API functions in this callback.  Why not?  Because the
// video renderer might hold the global Win16 lock so that the video
// surface can be locked while you copy its data.  This is not an
// issue on Windows 2000, but is a limitation on Win95,98,98SE, and ME.
// Calling a 16-bit legacy function could lock the system, because 
// it would wait forever for the Win16 lock, which would be forever
// held by the video renderer.
//
// As a workaround, copy the bitmap data during the callback,
// post a message to our app, and write the data later.
//
STDMETHODIMP CGrabberCallBack::BufferCB( double dblSampleTime, BYTE *pBuffer, long lBufferSize )
{
	if (!pBuffer)
	{
			return E_POINTER;
	}

	if( m_lBufferSize < lBufferSize )
	{
		SAFE_DELETE_ARRAY( m_pBuffer );
		m_lBufferSize = 0;
	}

	// Since we can't access Windows API functions in this callback, just
	// copy the grabbed data to a global structure for later reference.
	m_dblSampleTime = dblSampleTime;

	// If we haven't yet allocated the data buffer, do it now.
	// Just allocate what we need to store the grabbed data.
	if (!m_pBuffer)
	{
		m_pBuffer = new BYTE[lBufferSize];
		m_lBufferSize = lBufferSize;
	}

	if( !m_pBuffer )
	{
		m_lBufferSize = 0;
		return E_OUTOFMEMORY;
	}

	// Copy the captured data into our buffer
	memcpy( m_pBuffer, pBuffer, lBufferSize );

	m_bBufferAvailable = true;

	// calling callback function
	//m_lpfnCallBack( pBuffer, lBufferSize );

	m_nFPSMeter++;

	return 0;
}

CGrabberCallBack::CGrabberCallBack()
{
	m_lBufferSize = 0;
	m_pBuffer = NULL;
}

/*void CGrabberCallBack::SetCallBack( void (*lpfnCallBack)( BYTE*, long ) )
{
	m_lpfnCallBack = lpfnCallBack;
}*/

CGrabberCallBack::~CGrabberCallBack()
{
	SAFE_DELETE_ARRAY( m_pBuffer );
}

BYTE* CGrabberCallBack::GetBuffer()
{
	m_bBufferAvailable = false;
	return m_pBuffer;
}

const long& CGrabberCallBack::GetBufferSize()
{
	return m_lBufferSize;
}

const BOOL& CGrabberCallBack::IsBufferAvailable()
{
	return m_bBufferAvailable;
}

const UINT& CGrabberCallBack::GetFPSMeter()
{
	return m_nFPSMeter;
}

void CGrabberCallBack::ResetFPSMeter()
{
	m_nFPSMeter = 0;
}
