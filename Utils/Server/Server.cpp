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
	m_pTCPDataConnection = NULL;
	m_pUDPDataConnection = NULL;
	memset( m_pRecvBuf, 0, MAXMESSAGELENGTH+1 );

	m_nTCPDataPort = 0;
	m_nUDPDataPort = 0;
}

CServer::~CServer()
{
	Disconnect();

	// deleting m_mspChannels
	for ( tChannelMap::iterator it = m_mspChannels.begin(); it != m_mspChannels.end(); it++ )
	{
		SAFE_DELETE( it->second );
	}

	// deleting m_mspClients
	for ( tClientMap::iterator it = m_mspClients.begin(); it != m_mspClients.end(); it++ )
	{
		SAFE_DELETE( it->second );
	}
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

void CServer::Disconnect()
{
	SAFE_DELETE( m_pTCPControlConnection );
	SAFE_DELETE( m_pTCPDataConnection );
	SAFE_DELETE( m_pUDPDataConnection );
}

const CString& CServer::GetHost()
{
	return m_szHost;
}

const CString& CServer::GetPort()
{
	return m_szPort;
}

const CString& CServer::GetNick()
{
	return m_szNick;
}

CTCPConnection* CServer::GetControlConnection()
{
	return m_pTCPControlConnection;
}

void CServer::AddIntoRecentServerList( CString szServer )
{
	// is server already stored?
	for( int i = theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ); i > 0; i-- )
	{
		if( theApp.GetSettingsFile()->Get( "RecentServers", i, "" ) == szServer )
		{
			return;
		}
	}

	for( int i = theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ); i > 1; i-- )
	{
		theApp.GetSettingsFile()->Set( "RecentServers", i, theApp.GetSettingsFile()->Get( "RecentServers", i-1, "" ) );
	}
	theApp.GetSettingsFile()->Set( "RecentServers", "1", szServer );
}

LRESULT CServer::OnDataSocketEvent( WPARAM /*wParam*/, LPARAM lParam )
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
				Disconnect();
				return 0;
			}

			AddText( " connected\r\n" );

			if( m_pUDPDataConnection == NULL )
			{
				// creating the udp data socket
				AddText( "Creating UDP data socket.\r\n\r\n" );

				m_pUDPDataConnection = new CUDPConnection( GetSafeHwnd(), WU_DATASOCKET_EVENT );
				try
				{
					CString szPort;
					szPort.Format( "%d", m_nUDPDataPort );
					m_pUDPDataConnection->Connect( m_szHost, szPort );
					break;
				}
				catch ( char* pError )
				{
					AddText( pError );
					Disconnect();
					break;
				}
			}

			DoAutoJoin();

			break;
		}

		case FD_CLOSE:
		{
			AddText( "Data socket disconnected.\r\n" );
			Disconnect();
			break;
		}
	}
	
	return 0;
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
				Disconnect();
				return 0;
			}

			// now we are connected to the server
			//

			theApp.SetConnected( true );

			// saving server name to the recent server list
			if( m_szPort != "62320" )
			{
				AddIntoRecentServerList( m_szHost + ":" + m_szPort );
			}
			else
			{
				AddIntoRecentServerList( m_szHost );
			}

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
				szLine = szRecvBuf.Tokenize( "\n", nCurrPos ).TrimRight();
				ProcessServerMessage( szLine );
			}
			
			memset( m_pRecvBuf, 0, MAXMESSAGELENGTH+1 );
			break;
		}

		case FD_CLOSE:
		{
			AddText( "Disconnected.\r\n" );
			Disconnect();
			break;
		}
	}

	return 0;
}
