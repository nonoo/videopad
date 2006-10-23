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
#include "VideoGraph.h"

HRESULT CVideoGraph::SetVideoFormat( UINT nPreferredImageWidth, UINT nPreferredImageHeight, REFERENCE_TIME nPreferredFPS )
{
	// get the nearest, or exact video size the user wants
	//
	IEnumMediaTypes *pMedia;
	AM_MEDIA_TYPE *pmt = NULL, *pfnt = NULL;

	HRESULT hr = m_pVideoCaptureDevice->GetOutPin()->EnumMediaTypes( &pMedia );

	if( SUCCEEDED(hr) )
	{
		// going through all available video formats until we find the correct one
		//
		while( pMedia->Next( 1, &pmt, 0 ) == S_OK )
		{
			if ( ( pmt->formattype == FORMAT_VideoInfo ) && 
				( pmt->cbFormat == sizeof(VIDEOINFOHEADER) ) )
			{
				VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pmt->pbFormat;
				if( (UINT)vih->bmiHeader.biWidth == nPreferredImageWidth && (UINT)vih->bmiHeader.biHeight == nPreferredImageHeight )
				{
					pfnt = pmt;

					// found correct video size
					//
					break;
				}
				DeleteMediaType( pmt );
			}
		}
		SAFE_RELEASE( pMedia );
	}

	// set the video size
	//
	CComPtr<IAMStreamConfig> pConfig;
	hr = m_pVideoCaptureDevice->GetOutPin()->QueryInterface( IID_IAMStreamConfig, (void **) &pConfig );
	if( SUCCEEDED( hr ) )
	{
		if( pfnt != NULL )
		{
			VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pfnt->pbFormat;

			// setting fps
			//
			vih->AvgTimePerFrame = 10000000/nPreferredFPS;
			pConfig->SetFormat( pfnt );

			DeleteMediaType( pfnt );
		}
		else
		{
			return -1;
		}

		// getting current video format, it may differ from the format we wanted
		// so that's way we ask for it again
		//
		hr = pConfig->GetFormat( &pfnt );
		if( SUCCEEDED( hr ) )
		{
			m_pVideoCaptureDevice->SetVideoWidth( ((VIDEOINFOHEADER *)pfnt->pbFormat)->bmiHeader.biWidth );
			m_pVideoCaptureDevice->SetVideoHeight( ((VIDEOINFOHEADER *)pfnt->pbFormat)->bmiHeader.biHeight );
			m_pVideoCaptureDevice->SetVideoFPS( 10000000/((VIDEOINFOHEADER *)pfnt->pbFormat)->AvgTimePerFrame );

			DeleteMediaType( pfnt );
		}
	}
	return 0;
}

HRESULT CVideoGraph::Create( CVideoCaptureDevice* pVideoCaptureDevice )
{
	m_pVideoCaptureDevice = pVideoCaptureDevice;

	// creating the smart tee splitter
	//
	m_pSplitter.CoCreateInstance( CLSID_SmartTee );

	if( !m_pSplitter )
	{
		// can't create Smart Tee Splitter
		return -1;
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pSplitterBase( m_pSplitter );

	// add the splitter to the graph
	//
	m_pGraph->AddFilter( pSplitterBase, L"Splitter" );

	// creating the sample grabber
	//
	m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );

	if( !m_pGrabber )
	{
		// can't create ISampleGrabber
		return -2;
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabberBase( m_pGrabber );

	// add the grabber to the graph
	//
	m_pGraph->AddFilter( pGrabberBase, L"Grabber" );

	// create the source filter (pSourceFilter)

	m_pSourceFilter = m_pVideoCaptureDevice->GetBaseFilter();
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pCaptureBase( m_pSourceFilter );

	// add video capture source to the graph
	//
	m_pGraph->AddFilter( pCaptureBase, L"Source" );
	pCaptureBase.Release();

	// tell the grabber to grab 24-bit video
	//
	CMediaType GrabType;
	GrabType.SetType( &MEDIATYPE_Video );
	GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
	m_pGrabber->SetMediaType( &GrabType );

	// setting video size
	//
	m_pVideoCaptureDevice->SetOutPin( GetOutPin( m_pSourceFilter, 0 ) );
	SetVideoFormat( m_pVideoCaptureDevice->GetPreferredVideoWidth(), m_pVideoCaptureDevice->GetPreferredVideoHeight(), m_pVideoCaptureDevice->GetPreferredVideoFPS() );

	HRESULT hr;
	// connect the webcam out pin to the splitter
	//
	CComPtr< IPin > pSplitterInPin = GetInPin( pSplitterBase, 0 );
	hr = m_pGraph->Connect( m_pVideoCaptureDevice->GetOutPin(), pSplitterInPin );
	if( FAILED( hr ) )
	{
		// can't connect pins in filtergraph
		return -3;
	}
	pSplitterInPin.Release();

	// connect the splitter capture pin to the grabber
	//
	CComPtr< IPin > pSplitterOutPin = GetOutPin( pSplitterBase, 0 );
	CComPtr< IPin > pGrabberInPin = GetInPin( pGrabberBase, 0 );
	pGrabberBase.Release();
	hr = m_pGraph->Connect( pSplitterOutPin, pGrabberInPin );
	if( FAILED( hr ) )
	{
		// can't connect pins in filtergraph
		return -3;
	}
	pGrabberInPin.Release();
	pSplitterOutPin.Release();

	// connect the splitter preview pin to the renderer
	//
	CComPtr< IPin > pSplitterPrevOutPin = GetOutPin( pSplitterBase, 1 );
	pSplitterBase.Release();
	hr = m_pGraph->Render( pSplitterPrevOutPin );
	pSplitterPrevOutPin.Release();

	if( FAILED( hr ) )
	{
		// can't render image from the device
		return -4;
	}

	// don't buffer the samples as they pass through
	//
	m_pGrabber->SetBufferSamples( FALSE );

	// not grabbing just one frame
	//
	m_pGrabber->SetOneShot( FALSE );

	// setting callback
	//
	m_pGrabber->SetCallback( &GrabberCallBack, 1 );

	// set render out to the activemovie window
	//
	m_pActiveMovieWindow = m_pGraph;

#ifdef DEBUG
	// add our graph to the running object table, which will allow
	// the GraphEdit application to "spy" on our graph
	hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
	if (FAILED(hr))
	{
		// failed to register filtergraph to Running Object Table
		return -5;
	}
#endif

	return 0;
}

void CVideoGraph::Destroy()
{
	m_pActiveMovieWindow.Release();
	m_pSplitter.Release();
}

const UINT& CVideoGraph::GetVideoHeight()
{
	return m_pVideoCaptureDevice->GetVideoHeight();
}

const UINT& CVideoGraph::GetVideoWidth()
{
	return m_pVideoCaptureDevice->GetVideoWidth();
}

const REFERENCE_TIME& CVideoGraph::GetVideoFPS()
{
	return m_pVideoCaptureDevice->GetVideoFPS();
}
