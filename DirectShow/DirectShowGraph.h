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

#include "GrabberCallBack.h"
class CVideoCaptureDevice;
class CAudioCaptureDevice;

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
	BYTE* GetBuffer();
	const long& GetBufferSize();
	const BOOL& IsBufferAvailable();

	// retrieving FPS meter
	// this is an integer increasing when a sample is grabbed
	//
	const UINT& GetFPSMeter();
	void ResetFPSMeter();

protected:
	// for converting unicode BSTRs to ansi strings
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
