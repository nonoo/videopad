#ifndef __DIRECTSHOWGRAPH_H
#define __DIRECTSHOWGRAPH_H

#include "GrabberCallBack.h"
#include "VideoCaptureDevice.h"
#include "AudioCaptureDevice.h"

// represents a DirectShow filter graph whence you can
// capture video (CVideoGraph) or audio (CAudioGraph)
//
class CDirectShowGraph
{
public:
	CDirectShowGraph();
	~CDirectShowGraph();

	CVideoCaptureDevice* GetDefaultVideoCaptureDevice();
	void ListVideoCaptureDevices();

	CAudioCaptureDevice* GetDefaultAudioCaptureDevice();
	void ListAudioCaptureDevices();

	void Start();
	void Stop();

	// retrieving captured data (from CGrabberCallback)
	//
	__forceinline BYTE* GetBuffer();
	__forceinline long GetBufferSize();
	__forceinline BOOL IsBufferAvailable();

	// retrieving FPS meter
	// this is an integer increasing when a sample is grabbed
	//
	__forceinline UINT GetFPSMeter();
	__forceinline void ResetFPSMeter();

protected:
	// for converting BSTRs
	//
	int WideCharToLocal( LPTSTR pLocal, LPCWSTR pWide, DWORD dwChars );

	// the capture source
	//
	CComPtr< IBaseFilter > m_pSourceFilter;

	// the directshow graph
	// other filters are added to this
	//
	CComPtr< IGraphBuilder > m_pGraph;

	// the grabber, this extracts data from the graph
	//
	CComPtr< ISampleGrabber > m_pGrabber;

	CGrabberCallBack GrabberCallBack;


	IPin* GetInPin( IBaseFilter* pFilter, int nPin );
	IPin* GetOutPin( IBaseFilter* pFilter, int nPin );

#ifdef DEBUG
	DWORD m_dwGraphRegister; // ID for ROT registering

	// Adds/removes a DirectShow filter graph from the Running Object Table,
	// allowing GraphEdit to "spy" on a remote filter graph if enabled.
	HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
	void RemoveGraphFromRot(DWORD pdwRegister);
#endif

private:
	HRESULT GetPin( IBaseFilter* pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin );
};





__forceinline BYTE* CDirectShowGraph::GetBuffer()
{
	return GrabberCallBack.GetBuffer();
}

__forceinline long CDirectShowGraph::GetBufferSize()
{
	return GrabberCallBack.GetBufferSize();
}

__forceinline BOOL CDirectShowGraph::IsBufferAvailable()
{
	return GrabberCallBack.IsBufferAvailable();
}

__forceinline UINT CDirectShowGraph::GetFPSMeter()
{
	return GrabberCallBack.GetFPSMeter();
}

__forceinline void CDirectShowGraph::ResetFPSMeter()
{
	GrabberCallBack.ResetFPSMeter();
}

#endif
