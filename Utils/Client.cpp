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
#include "Client.h"


CClient::CClient()
{
	m_pdlgVideo = NULL;
}

CClient::~CClient()
{
	SAFE_DELETE( m_pdlgVideo );
}

void CClient::Create( CString szNick )
{
	m_szNick = szNick;
	
/*	a videodialogot majd csak akkor nyitjuk meg ha megerkezett az elso kepkocka a dekodertol
	
	if ( bOpenVideoDlg )
	{
		m_pdlgVideo = new CVideoDialog;
		m_pdlgVideo->Create( IDD_DIALOG_VIDEO );
		m_pdlgVideo->SetWindowText( m_szNick );
		m_pdlgVideo->ShowWindow( SW_SHOW );
	}*/
}

void CClient::SetNick( CString szNick )
{
	m_szNick = szNick;
}

const CString& CClient::GetNick()
{
	return m_szNick;
}

void CClient::AddChannel( CChannel* pChannel )
{
	m_apChannels.Add( pChannel );
}

void CClient::RemoveChannel( CChannel* pChannel )
{
	for( int i = 0; i < m_apChannels.GetCount(); i++ )
	{
		if( m_apChannels[i] == pChannel )
		{
			m_apChannels.RemoveAt( i, 1 );
		}
	}
}

INT_PTR CClient::GetChannelNum()
{
	return m_apChannels.GetCount();
}

const CArray< CChannel* >& CClient::GetChannels()
{
	return m_apChannels;
}
