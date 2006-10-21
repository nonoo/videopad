#ifndef __AUDIOGRAPH_H
#define __AUDIOGRAPH_H

#include "DirectShowGraph.h"
#include "AudioCaptureDevice.h"

class CAudioGraph : public CDirectShowGraph
{
public:
	CAudioGraph( CAudioCaptureDevice& AudioCaptureDevice, DWORD dwPreferredSampleRate, WORD wPreferredBitsPerSecond, WORD wPreferredChannels );

	__forceinline DWORD CAudioGraph::GetSamplesPerSec();
	__forceinline WORD CAudioGraph::GetBitsPerSample();
	__forceinline WORD CAudioGraph::GetChannels();

private:
	void SetAudioFormat( DWORD dwSamplesPerSec, WORD wBitsPerSample, WORD nChannels );

	CAudioCaptureDevice& AudioCaptureDevice;
};






__forceinline DWORD CAudioGraph::GetSamplesPerSec()
{
	return AudioCaptureDevice.GetSamplesPerSec();
}

__forceinline WORD CAudioGraph::GetBitsPerSample()
{
	return AudioCaptureDevice.GetBitsPerSample();
}

__forceinline WORD CAudioGraph::GetChannels()
{
	return AudioCaptureDevice.GetChannels();
}

#endif