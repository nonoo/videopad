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

BEGIN_MESSAGE_MAP( CServer, CDialog )
	ON_WM_CLOSE()
	ON_MESSAGE( WU_CONTROLSOCKET_EVENT, OnControlSocketEvent )
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
