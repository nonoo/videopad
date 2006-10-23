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

void CVideoGraph::SetVideoFormat( UINT nPreferredImageWidth, UINT nPreferredImageHeight, REFERENCE_TIME nPreferredFPS )
{
	// get the nearest, or exact video size the user wants
	//
	IEnumMediaTypes *pMedia;
	AM_MEDIA_TYPE *pmt = NULL, *pfnt = NULL;

	HRESULT hr = VideoCaptureDevice.GetOutPin()->EnumMediaTypes( &pMedia );

	if( SUCCEEDED(hr) )
	{
		while( pMedia->Next( 1, &pmt, 0 ) == S_OK )
		{
			if ( ( pmt->formattype == FORMAT_VideoInfo ) && 
				( pmt->cbFormat == sizeof(VIDEOINFOHEADER) ) )
			{
				VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pmt->pbFormat;
				printf( "available video size: %ix%i\n", vih->bmiHeader.biWidth, vih->bmiHeader.biHeight );
				if( (UINT)vih->bmiHeader.biWidth == nPreferredImageWidth && (UINT)vih->bmiHeader.biHeight == nPreferredImageHeight )
				{
					pfnt = pmt;

					printf( "found correct video size: %ix%i\n", vih->bmiHeader.biWidth, vih->bmiHeader.biHeight );
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
	hr = VideoCaptureDevice.GetOutPin()->QueryInterface( IID_IAMStreamConfig, (void **) &pConfig );
	if( SUCCEEDED( hr ) )
	{
		if( pfnt != NULL )
		{
			VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pfnt->pbFormat;
			vih->AvgTimePerFrame = 10000000/nPreferredFPS;
			printf("changing fps to %d\n\n",nPreferredFPS);
			pConfig->SetFormat( pfnt );

			DeleteMediaType( pfnt );
		}
		else
		{
			printf( "can't set video size!\n" );
		}

		hr = pConfig->GetFormat( &pfnt );
		if( SUCCEEDED( hr ) )
		{
			VideoCaptureDevice.SetVideoWidth( ((VIDEOINFOHEADER *)pfnt->pbFormat)->bmiHeader.biWidth );
			VideoCaptureDevice.SetVideoHeight( ((VIDEOINFOHEADER *)pfnt->pbFormat)->bmiHeader.biHeight );

			printf("initialized video: %dx%d\n\n", VideoCaptureDevice.GetVideoWidth(), VideoCaptureDevice.GetVideoHeight() );

			DeleteMediaType( pfnt );
		}
	}
}

CVideoGraph::CVideoGraph( CVideoCaptureDevice& VideoCaptureDevice ) : VideoCaptureDevice( VideoCaptureDevice )
{
	// creating the smart tee splitter
	//
	m_pSplitter.CoCreateInstance( CLSID_SmartTee );

	if( !m_pSplitter )
	{
		MessageBox( NULL, "DirectShow Error: Can't create Smart Tee Splitter!", "Error", MB_ICONSTOP );
		exit(-1);
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
		MessageBox( NULL, "DirectShow Error: Can't create ISampleGrabber!", "Error", MB_ICONSTOP );
		exit(-1);
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabberBase( m_pGrabber );

	// add the grabber to the graph
	//
	m_pGraph->AddFilter( pGrabberBase, L"Grabber" );

	// create the source filter (pSourceFilter)

	m_pSourceFilter = VideoCaptureDevice.GetBaseFilter();
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
	VideoCaptureDevice.SetOutPin( GetOutPin( m_pSourceFilter, 0 ) );
	SetVideoFormat( VideoCaptureDevice.GetPreferredVideoWidth(), VideoCaptureDevice.GetPreferredVideoHeight(), VideoCaptureDevice.GetPreferredVideoFPS() );

	HRESULT hr;
	// connect the webcam out pin to the splitter
	//
	CComPtr< IPin > pSplitterInPin = GetInPin( pSplitterBase, 0 );
	hr = m_pGraph->Connect( VideoCaptureDevice.GetOutPin(), pSplitterInPin );
	if( FAILED( hr ) )
	{
		MessageBox( NULL, "DirectShow Error: Can't connect pins in filtergraph!", "Error", MB_ICONSTOP );
		exit(-1);
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
		MessageBox( NULL, "DirectShow Error: Can't connect pins in filtergraph!", "Error", MB_ICONSTOP );
		exit(-1);
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
		MessageBox( NULL, "DirectShow Error: Can't render image from the device!", "Error", MB_ICONSTOP );
		exit(-1);
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
		MessageBox( NULL, "DirectShow Error: Failed to register filtergraph to Running Object Table!", "Error", MB_ICONSTOP );
		exit(-1);
	}
#endif
}

CVideoGraph::~CVideoGraph()
{
	m_pActiveMovieWindow.Release();
	m_pSplitter.Release();
}

const UINT& CVideoGraph::GetVideoHeight()
{
	return VideoCaptureDevice.GetVideoHeight();
}

const UINT& CVideoGraph::GetVideoWidth()
{
	return VideoCaptureDevice.GetVideoWidth();
}
