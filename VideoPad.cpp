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
#include "VideoPad.h"
#include "MainFrm.h"


#ifdef _DEBUG
#include "Utils\vld.h" // memory leak detection, http://www.codeproject.com/tools/visualleakdetector.asp
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CVideoPadApp, CWinApp)
	ON_BN_CLICKED(ID_TOOLBAR_CONNECT, OnPressConnect)
	ON_BN_CLICKED(ID_TOOLBAR_CHANNEL, OnPressChannel)
END_MESSAGE_MAP()


CVideoPadApp::CVideoPadApp()
{
	m_bConnected = false;
	m_pActiveServer = NULL;
	m_pDirectShowGraph = NULL;
	m_pVideoCaptureDevice = NULL;
	m_pAudioCaptureDevice = NULL;
}

CVideoPadApp::~CVideoPadApp()
{
	SAFE_DELETE( m_pActiveServer );

	SAFE_DELETE( m_pDirectShowGraph );
	SAFE_DELETE( m_pVideoCaptureDevice );
	SAFE_DELETE( m_pAudioCaptureDevice );

	// winsock cleanup
	WSACleanup();
}

CVideoPadApp theApp;

BOOL CVideoPadApp::InitInstance()
{
	CWinApp::InitInstance();

	// initializing winsock
	//
	HRESULT hr = WSAStartup( MAKEWORD(2,2), &m_wsaData );
	if( FAILED( hr ) )
	{
		MessageBox( NULL, "Can't initialize WinSock!", "Error", MB_ICONERROR | MB_OK );
		exit( -1 );
	}

	// initializing COM
	//
	hr = CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
	if( FAILED( hr ) )
	{
		MessageBox( NULL, "Can't initialize COM!", "Error", MB_ICONERROR | MB_OK );
		exit( -1 );
	}

	m_SettingsFile.LoadConfig();

	InitCaptureDevices();

	CMainFrame* pFrame = new CMainFrame;
	if ( !pFrame )
	{
		return FALSE;
	}
	
	m_pMainWnd = pFrame;

	pFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );

	pFrame->ShowWindow( SW_SHOW );
	pFrame->UpdateWindow();

	return TRUE;
}

const bool& CVideoPadApp::GetConnected()
{
	return m_bConnected;
}

void CVideoPadApp::SetConnected( bool bStatus )
{
	m_bConnected = bStatus;
}

void CVideoPadApp::OnPressConnect()
{
	if( m_bConnected )
	{
		Disconnect();
	}
	else
	{
		Connect();
	}
}

void CVideoPadApp::OnPressChannel()
{
	if ( m_dlgChannel.DoModalJoinChannel() == IDOK )
	{
		//hanyas
	}
}

void CVideoPadApp::Connect()
{
	// show connecting dialog. if OK then add channel names to Channel Dialog and show it.
	CString szServerHost;
	CString szServerPort;
	CString szNick;
	if ( m_dlgConnecting.DoModalGetServer( szServerHost, szServerPort, szNick ) == IDOK )
	{
		SAFE_DELETE( m_pActiveServer );
		m_pActiveServer = new CServer;

		// trying to connect to the selected host:port
		//
		m_pActiveServer->Connect( szServerHost, szServerPort, szNick );
	}
}

void CVideoPadApp::Disconnect()
{
	SAFE_DELETE( m_pActiveServer );
}

CServer* CVideoPadApp::GetActiveServer()
{
	return m_pActiveServer;
}

// CMainFrame* CVideoPadApp::GetMainFrame()
// {
// 	return m_pMainWnd;
// }

CSettingsFile* CVideoPadApp::GetSettingsFile()
{
	return &m_SettingsFile;
}

void CVideoPadApp::AutoDetectVideoCaptureDevice( CString& szVideoCaptureDeviceID )
{
	map< CString, CString > szmVideoCaptureDevices =
		m_pDirectShowGraph->ListVideoCaptureDevices();
	if( !szmVideoCaptureDevices.empty() )
	{
		// we use the first device from the list
		//
		m_SettingsFile.Set( "VideoCaptureDevice", "Name", szmVideoCaptureDevices.begin()->second );
		m_SettingsFile.Set( "VideoCaptureDevice", "ID", szmVideoCaptureDevices.begin()->first );

		// returning with the device ID
		szVideoCaptureDeviceID = szmVideoCaptureDevices.begin()->first;
	}
}

void CVideoPadApp::AutoDetectAudioCaptureDevice( CString& szAudioCaptureDeviceID )
{
	map< CString, CString > szmAudioCaptureDevices =
		m_pDirectShowGraph->ListAudioCaptureDevices();
	if( !szmAudioCaptureDevices.empty() )
	{
		// we use the first device from the list
		//
		m_SettingsFile.Set( "AudioCaptureDevice", "Name", szmAudioCaptureDevices.begin()->second );
		m_SettingsFile.Set( "AudioCaptureDevice", "ID", szmAudioCaptureDevices.begin()->first );

		// returning with the device ID
		szAudioCaptureDeviceID = szmAudioCaptureDevices.begin()->first;
	}
}

void CVideoPadApp::InitCaptureDevices()
{
	// initializing a directshow filtergraph
	//
	m_pDirectShowGraph = new CDirectShowGraph;
	m_pVideoCaptureDevice = new CVideoCaptureDevice;
	m_pAudioCaptureDevice = new CAudioCaptureDevice;

	// if the settingsfile has a video capture device COM ID, we init our device using it,
	// otherwise we try to init the default video capture device
	//
	CString szVideoCaptureDeviceID = m_SettingsFile.Get( "VideoCaptureDevice", "ID", "" );

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

	// if the settingsfile has an audio capture device COM ID, we init our device using it,
	// otherwise we try to init the default audio capture device
	//
	CString szAudioCaptureDeviceID = m_SettingsFile.Get( "AudioCaptureDevice", "ID", "" );

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
