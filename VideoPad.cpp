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

#ifdef _DEBUG
#include "Utils\vld.h" // memory leak detection, http://www.codeproject.com/tools/visualleakdetector.asp
#endif

#include "VideoPad.h"
#include "MainFrm.h"

BEGIN_MESSAGE_MAP(CVideoPadApp, CWinApp)
	ON_BN_CLICKED(ID_TOOLBAR_CONNECT, OnPressConnect)
	ON_BN_CLICKED(ID_TOOLBAR_CHANNEL, OnPressChannel)
END_MESSAGE_MAP()


CVideoPadApp::CVideoPadApp()
{
	m_bConnected = false;
	m_pActiveServer = NULL;
	m_pDirectShow = NULL;
	m_pVideoStream = NULL;
	m_pAudioStream = NULL;
	m_pTheoraEncoder = NULL;
	m_pSpeexEncoder = NULL;
}

CVideoPadApp::~CVideoPadApp()
{
	SAFE_DELETE( m_pActiveServer );

	SAFE_DELETE( m_pTheoraEncoder );
	SAFE_DELETE( m_pSpeexEncoder );
	SAFE_DELETE( m_pVideoStream );
	SAFE_DELETE( m_pAudioStream );
	SAFE_DELETE( m_pDirectShow );

	// winsock cleanup
	WSACleanup();

	// COM uninitialize
	CoUninitialize();
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

	CMainFrame* pFrame = new CMainFrame;
	if ( !pFrame )
	{
		return FALSE;
	}
	
	m_pMainWnd = pFrame;

	pFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );

	pFrame->ShowWindow( SW_SHOW );
	pFrame->UpdateWindow();

	m_pDirectShow = new CDirectShow;
	m_pDirectShow->Create();

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
		Disconnect();
		m_pActiveServer = new CServer;

		// trying to connect to the selected host:port
		//
		m_pActiveServer->Connect( szServerHost, szServerPort, szNick );
	}
}

void CVideoPadApp::Disconnect()
{
	SetConnected( false );
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

CDirectShow* CVideoPadApp::GetDirectShow()
{
	return m_pDirectShow;
}

COggStream* CVideoPadApp::GetVideoStream()
{
	return m_pVideoStream;
}

COggStream* CVideoPadApp::GetAudioStream()
{
	return m_pAudioStream;
}

CTheoraEncoder* CVideoPadApp::GetTheoraEncoder()
{
	return m_pTheoraEncoder;
}

CSpeexEncoder* CVideoPadApp::GetSpeexEncoder()
{
	return m_pSpeexEncoder;
}

void CVideoPadApp::SetVideoStream( COggStream* pVideoStream )
{
	m_pVideoStream = pVideoStream;
}

void CVideoPadApp::SetAudioStream( COggStream* pAudioStream )
{
	m_pAudioStream = pAudioStream;
}

void CVideoPadApp::SetTheoraEncoder( CTheoraEncoder* pTheoraEncoder )
{
	m_pTheoraEncoder = pTheoraEncoder;
}

void CVideoPadApp::SetSpeexEncoder( CSpeexEncoder* pSpeexEncoder )
{
	m_pSpeexEncoder = pSpeexEncoder;
}
