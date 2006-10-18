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
#include "../VideoPad.h"
#include "Channel.h"


CChannel::CChannel()
{
	m_bActive = false;
}

CChannel::~CChannel()
{
	for ( int i = 0; i < m_aClients.GetCount(); i++ )
	{
		SAFE_DELETE( m_aClients[i] );
	}
	
	m_aClients.RemoveAll();
}

void CChannel::Create( CString sChannelName )
{
	m_sChannelName = sChannelName;	
}

void CChannel::AddClient( CClient* pClient )
{
	m_aClients.Add( pClient );
}

void CChannel::RemoveClient( CString sUserName )
{
	for ( int i = 0; i < m_aClients.GetCount(); i++ )
	{
		if ( m_aClients[i]->GetName() == sUserName )
		{
			SAFE_DELETE( m_aClients[i] );
			m_aClients.RemoveAt( i, 1 );
		}
	}
}

const CString& CChannel::GetChannelName() const
{
	return m_sChannelName;
}

void CChannel::Join()
{
	// add some user to this channel:
	
	CClient* pNewClient = new CClient;
	pNewClient->Create( "BlooD2oo1", TRUE );
	AddClient( pNewClient );
	
	pNewClient = new CClient;
	pNewClient->Create( "Nonoo", TRUE );				
	AddClient( pNewClient );
	
	m_dlgChat.Create( IDD_DIALOG_CHAT, theApp.m_pMainWnd );
	m_dlgChat.ShowWindow( SW_SHOW );
	
	m_bActive = true;
}