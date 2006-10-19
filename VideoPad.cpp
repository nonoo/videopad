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
}

CVideoPadApp::~CVideoPadApp()
{
	SAFE_DELETE( m_pActiveServer );

	// winsock cleanup
	WSACleanup();
}

CVideoPadApp theApp;


BOOL CVideoPadApp::InitInstance()
{
	CWinApp::InitInstance();

	// initializing winsock
	WSAStartup( MAKEWORD(2,2), &m_wsaData );

	SetRegistryKey( _T("Local AppWizard-Generated Applications") );

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

void CVideoPadApp::OnPressConnect()
{
	SetConnected( !m_bConnected );
}

void CVideoPadApp::OnPressChannel()
{
	if ( m_dlgChannel.DoModalJoinChannel() == IDOK )
	{
		//hanyas
	}
}

// called when the user clicks on the quick connect toolbar button
void CVideoPadApp::SetConnected( bool bConnected )
{
	if ( bConnected )
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
			//m_pActiveServer->Connect( szServerHost, szServerPort, szNick );
			m_pActiveServer->Connect( "hullahaz.hu", "62320", "TestNick" );
		}
	}
	else
	{
		// disconnect
		SAFE_DELETE( m_pActiveServer );
	}
}

CServer* CVideoPadApp::GetActiveServer()
{
	return m_pActiveServer;
}

CIniManager* CVideoPadApp::GetIniManager()
{
	return &m_iniManager;
}

// CMainFrame* CVideoPadApp::GetMainFrame()
// {
// 	return m_pMainWnd;
// }
