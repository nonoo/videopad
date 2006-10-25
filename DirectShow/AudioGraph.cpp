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
#include "AudioGraph.h"

CAudioGraph::~CAudioGraph()
{
	Destroy();
};

HRESULT CAudioGraph::Create( CAudioCaptureDevice* pAudioCaptureDevice )
{
	m_pAudioCaptureDevice = pAudioCaptureDevice;

	// creating the sample grabber
	//
	m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );

	if( !m_pGrabber )
	{
		// DirectShow error: Can't create ISampleGrabber
		return -1;
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabberBase( m_pGrabber );

	// add the grabber to the graph
	//
	m_pGraph->AddFilter( pGrabberBase, L"Grabber" );

	// setting sample grabber media type
	//
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Audio;
	mt.subtype = MEDIASUBTYPE_PCM;
	m_pGrabber->SetMediaType(&mt);

	// create the source filter (pSourceFilter)
	//
	m_pSourceFilter = m_pAudioCaptureDevice->GetBaseFilter();
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pCaptureBase( m_pSourceFilter );

	// add audio capture source to the graph
	//
	m_pGraph->AddFilter( pCaptureBase, L"Source" );
	m_pAudioCaptureDevice->SetOutPin( GetOutPin( pCaptureBase, 0 ) );
	pCaptureBase.Release();

	HRESULT hr = SetAudioFormat( m_pAudioCaptureDevice->GetPreferredSamplesPerSec(), m_pAudioCaptureDevice->GetPreferredBitsPerSample(), m_pAudioCaptureDevice->GetPreferredChannels() );
	if( FAILED( hr ) )
	{
		pGrabberBase.Release();
		m_pGrabber.Release();
		return -2;
	}

	// connect the capture device out pin to the grabber
	//
	CComPtr< IPin > pGrabberInPin = GetInPin( pGrabberBase, 0 );
	pGrabberBase.Release();

	hr = m_pGraph->Connect( m_pAudioCaptureDevice->GetOutPin(), pGrabberInPin );
	if( FAILED( hr ) )
	{
		pGrabberInPin.Release();
		m_pGrabber.Release();
		// DirectShow error: Can't connect pins in audio filtergraph!
		return -3;
	}
	pGrabberInPin.Release();

	// buffering samples as they pass through
	//
	m_pGrabber->SetBufferSamples( TRUE );

	// not grabbing just one frame
	//
	m_pGrabber->SetOneShot( FALSE );

	// setting callback
	//
	m_pGrabber->SetCallback( &GrabberCallBack, 1 );

#ifdef DEBUG
	// add our graph to the running object table, which will allow
	// the GraphEdit application to "spy" on our graph
	hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
	if (FAILED(hr))
	{
		m_pGrabber.Release();
		// DirectShow error: Failed to register filtergraph to Running Object Table
		return -4;
	}
#endif
	return 0;
}

void CAudioGraph::Destroy()
{
	m_pGrabber.Release();
}

HRESULT CAudioGraph::SetAudioFormat( DWORD dwSamplesPerSec, WORD wBitsPerSample, WORD nChannels )
{
	IAMBufferNegotiation *pNeg = NULL;
	WORD wBytesPerSample = wBitsPerSample/8;
	DWORD dwBytesPerSecond = wBytesPerSample * dwSamplesPerSec * nChannels;
	//DWORD dwBufferSize = (DWORD)(0.5*dwBytesPerSecond);
	DWORD dwBufferSize;

	// setting buffer size according to the speex frame size
	// (in narrow-band: 160*2 bytes)
	switch( dwSamplesPerSec )
	{
		case 8000: { dwBufferSize = 320; break; }
		case 11025: { dwBufferSize = 1280; break; } // AUDIO STREAM LAG DEPENDS ON THIS
		case 22050: { dwBufferSize = 1280; break; }
		case 44100: { dwBufferSize = 1600; break; }
		default: dwBufferSize = 320;
	}

	// get the nearest, or exact audio format the user wants
	//
	IEnumMediaTypes *pMedia = NULL;
	AM_MEDIA_TYPE *pmt = NULL;

	HRESULT hr = m_pAudioCaptureDevice->GetOutPin()->EnumMediaTypes( &pMedia );

	if( SUCCEEDED(hr) )
	{
		while( pMedia->Next( 1, &pmt, 0 ) == S_OK )
		{
			if ( ( pmt->formattype == FORMAT_WaveFormatEx) && 
				( pmt->cbFormat == sizeof(WAVEFORMATEX) ) )
			{
				WAVEFORMATEX *wf = (WAVEFORMATEX *)pmt->pbFormat;

				if( ( wf->nSamplesPerSec == dwSamplesPerSec ) &&
					( wf->wBitsPerSample == wBitsPerSample ) &&
					( wf->nChannels == nChannels ) )
				{
					// found correct audio format
					//
					CComPtr<IAMStreamConfig> pConfig;
					hr = m_pAudioCaptureDevice->GetOutPin()->QueryInterface( IID_IAMStreamConfig, (void **) &pConfig );
					if( SUCCEEDED( hr ) )
					{
						// get buffer negotiation interface
						m_pAudioCaptureDevice->GetOutPin()->QueryInterface(IID_IAMBufferNegotiation, (void **)&pNeg);

						// set the buffer size based on selected settings
						ALLOCATOR_PROPERTIES prop={0};
						prop.cbBuffer = dwBufferSize;
						prop.cBuffers = 6; // AUDIO STREAM LAG DEPENDS ON THIS
						prop.cbAlign = wBytesPerSample * nChannels;
						pNeg->SuggestAllocatorProperties(&prop);
						SAFE_RELEASE( pNeg );

						WAVEFORMATEX *wf = (WAVEFORMATEX *)pmt->pbFormat;

						// setting additional audio parameters
						wf->nAvgBytesPerSec = dwBytesPerSecond;
						wf->nBlockAlign = wBytesPerSample * nChannels;
						wf->nChannels = nChannels;
						wf->nSamplesPerSec = dwSamplesPerSec;
						wf->wBitsPerSample = wBitsPerSample;

						pConfig->SetFormat( pmt );

					}
					else
					{
						pConfig.Release();
						SAFE_RELEASE( pMedia );
						DeleteMediaType( pmt );
						// can't set given audio format!
						return -1;
					}

					DeleteMediaType( pmt );

					hr = pConfig->GetFormat( &pmt );
					if( SUCCEEDED( hr ) )
					{
						WAVEFORMATEX *wf = (WAVEFORMATEX *)pmt->pbFormat;

						m_pAudioCaptureDevice->SetAudioSamplesPerSec( wf->nSamplesPerSec );
						m_pAudioCaptureDevice->SetAudioBitsPerSample( wf->wBitsPerSample );
						m_pAudioCaptureDevice->SetAudioChannels( wf->nChannels );

						// audio is now initialized
						DeleteMediaType( pmt );
						pConfig.Release();
						SAFE_RELEASE( pMedia );
						return 0;
					}

					// error initializing audio
					DeleteMediaType( pmt );
					pConfig.Release();
					SAFE_RELEASE( pMedia );
					return -1;
				}
			}
			DeleteMediaType( pmt );
		}
		SAFE_RELEASE( pMedia );
	}
	// couldn't find appropriate audio format
	return -2;
}

const DWORD& CAudioGraph::GetSamplesPerSec()
{
	return m_pAudioCaptureDevice->GetSamplesPerSec();
}

const WORD& CAudioGraph::GetBitsPerSample()
{
	return m_pAudioCaptureDevice->GetBitsPerSample();
}

const WORD& CAudioGraph::GetChannels()
{
	return m_pAudioCaptureDevice->GetChannels();
}
