#ifndef __GRABBERCALLBACK_H
#define __GRABBERCALLBACK_H

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

	__forceinline BYTE* GetBuffer();
	__forceinline long GetBufferSize();
	__forceinline BOOL IsBufferAvailable();

	// sets the function to call when the buffer becomes available
	//
	void SetCallBack( void (*lpfnCallBack)( BYTE*, long ) );

	UINT GetFPSMeter();
	void ResetFPSMeter();

private:
	BYTE* m_pBuffer;
	long m_lBufferSize;
	double m_dblSampleTime;

	BOOL m_bBufferAvailable;

	UINT m_nFPSMeter;

	//void (*m_lpfnCallBack)( BYTE*, long );
};





__forceinline BYTE* CGrabberCallBack::GetBuffer()
{
	m_bBufferAvailable = false;
	return m_pBuffer;
}

__forceinline long CGrabberCallBack::GetBufferSize()
{
	return m_lBufferSize;
}

__forceinline BOOL CGrabberCallBack::IsBufferAvailable()
{
	return m_bBufferAvailable;
}

__forceinline UINT CGrabberCallBack::GetFPSMeter()
{
	return m_nFPSMeter;
}

__forceinline void CGrabberCallBack::ResetFPSMeter()
{
	m_nFPSMeter = 0;
}

#endif
