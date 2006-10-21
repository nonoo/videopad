#ifndef __VIDEOGRAPH_H
#define __VIDEOGRAPH_H

#include "DirectShowGraph.h"
#include "VideoCaptureDevice.h"

class CVideoGraph : public CDirectShowGraph
{
public:
	CVideoGraph( CVideoCaptureDevice& VideoCaptureDevice );
	~CVideoGraph();

	__forceinline UINT GetVideoWidth();
	__forceinline UINT GetVideoHeight();

private:
	// sets the given video format for the graph
	//
	void SetVideoFormat( UINT nPreferredImageWidth, UINT nPreferredImageHeight, REFERENCE_TIME nPreferredFPS );

	CVideoCaptureDevice& VideoCaptureDevice;

	CComQIPtr< IVideoWindow, &IID_IVideoWindow > m_pActiveMovieWindow;
	CComPtr< IBaseFilter > m_pSplitter;
};






__forceinline UINT CVideoGraph::GetVideoHeight()
{
	return VideoCaptureDevice.GetVideoHeight();
}

__forceinline UINT CVideoGraph::GetVideoWidth()
{
	return VideoCaptureDevice.GetVideoWidth();
}

#endif