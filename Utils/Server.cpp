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
#include "Server.h"
#include "../VideoPad.h"

CServer::CServer()
{
	CDialog::Create( IDD_DIALOG_SERVER );

	m_pTCPConnection = NULL;
}

CServer::~CServer()
{
	SAFE_DELETE( m_pTCPConnection );

	for ( int i = 0; i < m_apChannels.GetCount(); i++ )
	{
		SAFE_DELETE( m_apChannels[i] );
	}
	m_apChannels.RemoveAll();
}

void CServer::Connect( CString szHost, CString szPort, CString szNick )
{
	m_szHost = szHost;
	m_szPort = szPort;
	m_szNick = szNick;

	ShowWindow( SW_SHOW );
	UpdateWindow();
	AddText( "Connecting to " + m_szHost + ":" + m_szPort + "..." );

	m_pTCPConnection = new CTCPConnection( GetSafeHwnd() );
	try
	{
		m_sSocket = m_pTCPConnection->Connect( szHost, szPort );
	}
	catch ( char* pError )
	{
		AddText( pError );
	}
}

const CString& CServer::GetHost() const
{
	return m_szHost;
}

const CString& CServer::GetPort() const
{
	return m_szPort;
}

const CString& CServer::GetNick() const
{
	return m_szNick;
}

CArray< CChannel* >* const CServer::GetChannelsArray()
{
	return &m_apChannels;
}

bool CServer::JoinChannel( CChannel* pChannel )
{
	if ( !pChannel )
	{
		return false;
	}
	
	pChannel->Join(); // here we join to pChannel
	
	return true;
}

bool CServer::JoinChannel( CString szChannelName )
{
	for ( int i = 0; i < m_apChannels.GetCount(); i++ )
	{
		if ( m_apChannels[i]->GetChannelName() == szChannelName )
		{
			m_apChannels[i]->Join();
			return true;
		}
	}
	
	return false;
}

const SOCKET& CServer::GetSocket()
{
	return m_sSocket;
}

LRESULT CServer::OnSocketEvent( WPARAM /*wParam*/, LPARAM lParam )
{
	WORD wErrCode = WSAGETSELECTERROR( lParam );
	WORD wEvent = WSAGETSELECTEVENT( lParam );

	if( wEvent == FD_CONNECT )
	{
		if( wErrCode == WSAECONNREFUSED )
		{
			AddText( "failed: connection refused!\r\n" );
			return 0;
		}

		// now we are connected to the server
		//

		AddText( " connected\r\n" );

		//TODO: add channels to the array this way:

		/*		CChannel* pChannel = new CChannel;
		pChannel->Create( "XkakafosX" );
		m_apChannels.Add( pChannel );

		pChannel = new CChannel;
		pChannel->Create( "qka" );
		m_apChannels.Add( pChannel );	

		pChannel = new CChannel;
		pChannel->Create( "seggszor" );
		m_apChannels.Add( pChannel );*/

/*		m_bConnected = true;

		if ( m_iniManager.LoadAutoJoin() )
		{
			m_pActiveServer->JoinChannel( m_iniManager.LoadAutoJoinChannelName() );
		}
		else
		{
			if ( m_dlgChannel.DoModalJoinChannel() == IDOK )  // joining to the selected channel
			{

			}				
		}*/

	}

	return 0;
}

// STATUS DIALOG

BEGIN_MESSAGE_MAP( CServer, CDialog )
	ON_WM_CLOSE()
	ON_MESSAGE( WU_SOCKET_EVENT, OnSocketEvent )
END_MESSAGE_MAP()

BOOL CServer::PreTranslateMessage( MSG* pMsg )
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			CString sMessage;
			m_editMessage.GetWindowText( sMessage );
			sMessage += "\r\n";
			m_editMessages.SetSel( -1, 0 );
			m_editMessages.ReplaceSel( sMessage );
			m_editMessage.SetSel( 0, -1 );
			m_editMessage.ReplaceSel( "" );

		}
	}

	return CDialog::PreTranslateMessage( pMsg );
}


void CServer::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_EDIT_MESSAGES, m_editMessages );	
	DDX_Control( pDX, IDC_EDIT_MESSAGE, m_editMessage );	

}

void CServer::OnClose()
{
	ShowWindow( false );
	theApp.SetConnected( false );	
}

void CServer::OnOK()
{
} 

void CServer::AddText( CString szText )
{

	if ( m_editMessages.GetLineCount() > theApp.GetIniManager()->LoadMaxLineNum() )
	{
		m_editMessages.SetSel( 0, -1 );
	}

	m_editMessages.ReplaceSel( szText );
}