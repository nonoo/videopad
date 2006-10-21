#ifndef __AUDIOCAPTUREDEVICE_H
#define __AUDIOCAPTUREDEVICE_H

#include "CaptureDevice.h"

class CAudioCaptureDevice : public CCaptureDevice
{
public:
	CAudioCaptureDevice();

	// init capture device by given COM object ID
	//
	CAudioCaptureDevice( LPWSTR szID );

	__forceinline DWORD GetSamplesPerSec();
	__forceinline WORD GetBitsPerSample();
	__forceinline WORD GetChannels();

	// these functions are used by CAudioGraph to set
	// the current audio format information of the graph
	// and the device
	//
	void SetAudioSamplesPerSec( DWORD nSamplesPerSec );
	void SetAudioBitsPerSample( WORD wBitsPerSample );
	void SetAudioChannels( WORD nChannels );

private:
	DWORD m_nSamplesPerSec;
	WORD m_wBitsPerSample;
	WORD m_nChannels;
};







__forceinline DWORD CAudioCaptureDevice::GetSamplesPerSec()
{
	return m_nSamplesPerSec;
}

__forceinline WORD CAudioCaptureDevice::GetBitsPerSample()
{
	return m_wBitsPerSample;
}

__forceinline WORD CAudioCaptureDevice::GetChannels()
{
	return m_nChannels;
}

#endif