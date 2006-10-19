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
#include "../../VideoPad.h"

CServer::CServer()
{
	CDialog::Create( IDD_DIALOG_SERVER );

	m_pTCPControlConnection = NULL;
	memset( m_pRecvBuf, 0, MAXMESSAGELENGTH+1 );
}

CServer::~CServer()
{
	SAFE_DELETE( m_pTCPControlConnection );

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

	// connecting the control socket
	m_pTCPControlConnection = new CTCPConnection( GetSafeHwnd(), WU_CONTROLSOCKET_EVENT );
	try
	{
		m_pTCPControlConnection->Connect( szHost, szPort );
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
	return m_pTCPControlConnection->GetSocket();
}

LRESULT CServer::OnControlSocketEvent( WPARAM /*wParam*/, LPARAM lParam )
{
	WORD wErrCode = WSAGETSELECTERROR( lParam );
	WORD wEvent = WSAGETSELECTEVENT( lParam );

	switch( wEvent )
	{
		case FD_CONNECT:
		{
			if( wErrCode == WSAECONNREFUSED )
			{
				AddText( "failed: connection refused!\r\n" );
				return 0;
			}

			// now we are connected to the server
			//

			theApp.SetConnected( true );

			AddText( " connected\r\n\r\n" );
			break;
		}

		case FD_READ:
		{
			int res = recv( m_pTCPControlConnection->GetSocket(), m_pRecvBuf, MAXMESSAGELENGTH, 0 );
			if( res <= 0 )
			{
				AddText( "Disconnected.\r\n" );
				break;
			}

			// we break up the received string to lines in case we got more than one line
			// in a string
			//
			CString szLine;
			CString szRecvBuf = m_pRecvBuf;
			int nCurrPos = 0;
			while( nCurrPos < szRecvBuf.GetLength() )
			{
				szLine = szRecvBuf.Tokenize( "\n", nCurrPos );
				ProcessServerMessage( szLine );
			}
			
			memset( m_pRecvBuf, 0, MAXMESSAGELENGTH+1 );
			break;
		}
	}

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

	return 0;
}
