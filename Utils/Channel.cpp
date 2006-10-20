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
}

CChannel::~CChannel()
{
}

void CChannel::Create( CString szName )
{
	m_szName = szName;

	m_dlgChat.Create( IDD_DIALOG_CHAT, theApp.m_pMainWnd );
	m_dlgChat.ShowWindow( SW_SHOW );
}

void CChannel::AddClient( CClient* pClient )
{
	m_aClients.Add( pClient );

	pClient->AddChannel( this );
}

void CChannel::RemoveClient( CClient* pClient )
{
	for ( int i = 0; i < m_aClients.GetCount(); i++ )
	{
		if ( m_aClients[i] == pClient )
		{
			m_aClients.RemoveAt( i, 1 );
		}
	}

	pClient->RemoveChannel( this );
}

void CChannel::SetClientFlag( CClient* pClient, char cFlag )
{
	m_mpcClientFlags[pClient] = cFlag;
}

const CString& CChannel::GetName()
{
	return m_szName;
}

UINT CChannel::GetClientNum()
{
	return m_aClients.GetCount();
}

const CArray< CClient* >& CChannel::GetClients()
{
	return m_aClients;
}

void CChannel::SetCreationTime( time_t tCreationTime )
{
	m_tCreationTime = tCreationTime;
}

const time_t& CChannel::GetCreationTime()
{
	return m_tCreationTime;
}
