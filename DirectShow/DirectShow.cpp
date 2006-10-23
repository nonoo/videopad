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
#include "DirectShow.h"
#include "../VideoPad.h"

CDirectShow::CDirectShow()
{
	// initializing a directshow filtergraph
	//
	m_pDirectShowGraph = new CDirectShowGraph;

	m_pVideoGraph = new CVideoGraph;

	InitCaptureDevices();
}

CDirectShow::~CDirectShow()
{
	SAFE_DELETE( m_pDirectShowGraph );
	SAFE_DELETE( m_pVideoGraph );
	SAFE_DELETE( m_pVideoCaptureDevice );
	SAFE_DELETE( m_pAudioCaptureDevice );
}

void CDirectShow::AutoDetectVideoCaptureDevice( CString& szVideoCaptureDeviceID )
{
	map< CString, CString > szmVideoCaptureDevices =
		m_pDirectShowGraph->ListVideoCaptureDevices();
	if( !szmVideoCaptureDevices.empty() )
	{
		// we use the first device from the list
		//
		theApp.GetSettingsFile()->Set( "VideoCaptureDevice", "Name", szmVideoCaptureDevices.begin()->second );
		theApp.GetSettingsFile()->Set( "VideoCaptureDevice", "ID", szmVideoCaptureDevices.begin()->first );

		// returning with the device ID
		szVideoCaptureDeviceID = szmVideoCaptureDevices.begin()->first;
	}
}

void CDirectShow::AutoDetectAudioCaptureDevice( CString& szAudioCaptureDeviceID )
{
	map< CString, CString > szmAudioCaptureDevices =
		m_pDirectShowGraph->ListAudioCaptureDevices();
	if( !szmAudioCaptureDevices.empty() )
	{
		// we use the first device from the list
		//
		theApp.GetSettingsFile()->Set( "AudioCaptureDevice", "Name", szmAudioCaptureDevices.begin()->second );
		theApp.GetSettingsFile()->Set( "AudioCaptureDevice", "ID", szmAudioCaptureDevices.begin()->first );

		// returning with the device ID
		szAudioCaptureDeviceID = szmAudioCaptureDevices.begin()->first;
	}
}

void CDirectShow::InitCaptureDevices()
{
	m_pVideoCaptureDevice = new CVideoCaptureDevice;
	m_pAudioCaptureDevice = new CAudioCaptureDevice;

	// if the settingsfile has a video capture device COM ID, we init our device using it,
	// otherwise we try to init the default video capture device
	//
	CString szVideoCaptureDeviceID = theApp.GetSettingsFile()->Get( "VideoCaptureDevice", "ID", "" );

	// trying to init the last used (stored in the settings file) video capture device
	//
	HRESULT hr = m_pVideoCaptureDevice->Create( szVideoCaptureDeviceID );
	if( FAILED( hr ) )
	{
		AutoDetectVideoCaptureDevice( szVideoCaptureDeviceID );
		hr = m_pVideoCaptureDevice->Create( szVideoCaptureDeviceID );
		if( FAILED( hr ) )
		{
			// no video capture device
			SAFE_DELETE( m_pVideoCaptureDevice );
		}
	}
	else
	{
		SetVideoFormat(
			theApp.GetSettingsFile()->GetInt( "VideoCaptureDevice", "VideoWidth", 352 ),
			theApp.GetSettingsFile()->GetInt( "VideoCaptureDevice", "VideoHeight", 288 ),
			theApp.GetSettingsFile()->GetInt( "VideoCaptureDevice", "VideoFPS", 30 )
			);
	}


	// if the settingsfile has an audio capture device COM ID, we init our device using it,
	// otherwise we try to init the default audio capture device
	//
	CString szAudioCaptureDeviceID = theApp.GetSettingsFile()->Get( "AudioCaptureDevice", "ID", "" );

	// trying to init the last used (stored in the settings file) audio capture device
	//
	hr = m_pAudioCaptureDevice->Create( szAudioCaptureDeviceID );
	if( FAILED( hr ) )
	{
		AutoDetectAudioCaptureDevice( szAudioCaptureDeviceID );
		hr = m_pAudioCaptureDevice->Create( szAudioCaptureDeviceID );
		if( FAILED( hr ) )
		{
			// no audio capture device
			SAFE_DELETE( m_pAudioCaptureDevice );
		}
	}
}

void CDirectShow::StartVideo()
{
	m_pVideoGraph->Start();
}

void CDirectShow::StopVideo()
{
	m_pVideoGraph->Stop();
}

void CDirectShow::SetVideoFormat( UINT nPreferredVideoWidth, UINT nPreferredVideoHeight, REFERENCE_TIME rtPreferredVideoFPS )
{
	StopVideo();
	m_pVideoGraph->Destroy();

	m_pVideoCaptureDevice->SetPreferredVideoWidth( nPreferredVideoWidth );
	m_pVideoCaptureDevice->SetPreferredVideoHeight( nPreferredVideoHeight );
	m_pVideoCaptureDevice->SetPreferredVideoFPS( rtPreferredVideoFPS );

	m_pVideoGraph->Create( m_pVideoCaptureDevice );

	theApp.GetSettingsFile()->Set( "VideoCaptureDevice", "VideoWidth", m_pVideoGraph->GetVideoWidth() );
	theApp.GetSettingsFile()->Set( "VideoCaptureDevice", "VideoHeight", m_pVideoGraph->GetVideoHeight() );
	//theApp.GetSettingsFile()->Set( "VideoCaptureDevice", "VideoFPS", m_pVideoGraph->GetVideoFPS() );

	StartVideo();
}
