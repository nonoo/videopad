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

void CServer::Create( CString szHost, CString szPort, CString szNick )
{
	CDialog::Create( IDD_DIALOG_SERVER );
	
	ShowWindow( SW_SHOW );

	AddText( "SERVER MESSAGES:\r\n" );
		
	m_szHost = szHost;
	m_szPort = szPort;
	m_szNick = szNick;

	m_pTCPConnection = new CTCPConnection;
	m_pTCPConnection->Connect( szHost, szPort, NULL ); // ide a NULL hejere kell majd a jelenlegi status ablak hwndje

	//TODO: add channels to the array this way:
		
	CChannel* pChannel = new CChannel;
	pChannel->Create( "XkakafosX" );
	m_apChannels.Add( pChannel );
	
	pChannel = new CChannel;
	pChannel->Create( "qka" );
	m_apChannels.Add( pChannel );	
	
	pChannel = new CChannel;
	pChannel->Create( "seggszor" );
	m_apChannels.Add( pChannel );		
}

const CString& CServer::GetServerHost() const
{
	return m_szHost;
}

const CString& CServer::GetServerPort() const
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
	
	pChannel->Join(); // here we join to pChannel channel.
	
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




//part of the status dialog:

BEGIN_MESSAGE_MAP( CServer, CDialog )
	ON_WM_CLOSE()
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

void CServer::AddText( CString strText )
{

	if ( m_editMessages.GetLineCount() > 600 )
	{
		m_editMessages.SetSel( 0, -1 );
	}

	m_editMessages.ReplaceSel( strText );
}