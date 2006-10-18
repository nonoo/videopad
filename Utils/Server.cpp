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


CServer::CServer()
{
	m_dlgServer.Create( IDD_DIALOG_SERVER );
	m_dlgServer.ShowWindow( SW_SHOW );
	
	m_dlgServer.AddText( "SERVER MESSAGES:\r\n" );
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
