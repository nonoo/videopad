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
#include "ChatDialog.h"


BEGIN_MESSAGE_MAP(CChatDialog, CDialog)
	ON_WM_SIZE()
	ON_EN_SETFOCUS(IDC_EDIT_MESSAGES, OnEnSetfocusEditMessages)
	ON_EN_SETFOCUS(IDC_EDIT_CLIENTS, OnEnSetfocusEditClients)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CChatDialog, CDialog)
CChatDialog::CChatDialog( CWnd* pParent /*=NULL*/ )
	: CDialog( CChatDialog::IDD, pParent )
{

}

CChatDialog::~CChatDialog()
{
}

void CChatDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_EDIT_MESSAGES, m_eMessages );
	DDX_Control( pDX, IDC_EDIT_MESSAGE, m_eMessage );
	DDX_Control( pDX, IDC_EDIT_CLIENTS, m_eClients );
}

void CChatDialog::OnSize( UINT /*nType*/, int cx, int cy )
{
	m_eMessages.SetWindowPos( NULL, 2, 2, cx - 100, cy - 22, SWP_DRAWFRAME );
	m_eMessage.SetWindowPos( &wndTop, 2, cy - 17, cx - 2 - 2, 15, SWP_DRAWFRAME );
	m_eClients.SetWindowPos( NULL, cx - 95, 2, 93, cy - 22, SWP_DRAWFRAME );
}

BOOL CChatDialog::PreTranslateMessage( MSG* pMsg )
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			CString sMessage;
			m_eMessage.GetWindowText( sMessage );
			sMessage += "\r\n";
			m_eMessages.SetSel( -1, 0 );
			m_eMessages.ReplaceSel( sMessage );
			m_eMessage.SetSel( 0, -1 );
			m_eMessage.ReplaceSel( "" );

		}
	}
	
	return CDialog::PreTranslateMessage( pMsg );
}

// overriding OnOK because we don't want the dialog to close when pressing enter
void CChatDialog::OnOK()
{
} 

void CChatDialog::OnEnSetfocusEditMessages()
{
	m_eMessage.SetFocus();
	m_eMessages.ReplaceSel( "(21:53:10) (@Nonoo) alig hiszem el hogy kepes valamit csinalni\r\n" );
	m_eMessages.ReplaceSel( "(22:29:03) (@Pletykas) Nonoo: D0nv1ru5.\r\n" );	
}

void CChatDialog::OnEnSetfocusEditClients()
{
	m_eMessage.SetFocus();
}