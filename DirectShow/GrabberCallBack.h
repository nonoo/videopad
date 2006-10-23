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

// Note: this object is a SEMI-COM object, and can only be created statically.
// We use this little semi-com object to handle the sample-grab-callback,
// since the callback must provide a COM interface. We could have had an interface
// where you provided a function-call callback, but that's really messy, so we
// did it this way. You can put anything you want into this C++ object, even
// a pointer to a CDialog. Be aware of multi-thread issues though.
//
class CGrabberCallBack : public ISampleGrabberCB
{
public:
	CGrabberCallBack();
	~CGrabberCallBack();

	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample* pSample );
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE* pBuffer, long lBufferSize );

	BYTE* GetBuffer();
	const long& GetBufferSize();
	const BOOL& IsBufferAvailable();

	// sets the function to call when the buffer becomes available
	//
	void SetCallBack( void (*lpfnCallBack)( BYTE*, long ) );

	const UINT& GetFPSMeter();
	void ResetFPSMeter();

private:
	BYTE* m_pBuffer;
	long m_lBufferSize;
	double m_dblSampleTime;

	BOOL m_bBufferAvailable;

	UINT m_nFPSMeter;

	//void (*m_lpfnCallBack)( BYTE*, long );
};
