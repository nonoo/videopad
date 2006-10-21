#include "stdafx.h"
#include "AudioCaptureDevice.h"

CAudioCaptureDevice::CAudioCaptureDevice( LPWSTR szID ) : CCaptureDevice( szID )
{
	return;
}

CAudioCaptureDevice::CAudioCaptureDevice()
{
	return;
}

void CAudioCaptureDevice::SetAudioSamplesPerSec( DWORD nSamplesPerSec )
{
	m_nSamplesPerSec = nSamplesPerSec;
}

void CAudioCaptureDevice::SetAudioBitsPerSample( WORD wBitsPerSample )
{
	m_wBitsPerSample = wBitsPerSample;
}

void CAudioCaptureDevice::SetAudioChannels( WORD nChannels )
{
	m_nChannels = nChannels;
}
