#include "stdafx.h"
#include "ServerDialog.h"
#include "../VideoPad.h"

BEGIN_MESSAGE_MAP( CServerDialog, CDialog )
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CServerDialog::CServerDialog()
{
	m_bExist = false;
}
CServerDialog::~CServerDialog()
{
}

BOOL CServerDialog::Create(UINT nIDTemplate, CWnd* pParentWnd )
{
	if ( CDialog::Create( nIDTemplate, pParentWnd ) )
	{
		m_bExist = true;
		return true;
	}
	return false;

}

BOOL CServerDialog::PreTranslateMessage( MSG* pMsg )
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


void CServerDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_EDIT_MESSAGES, m_editMessages );	
	DDX_Control( pDX, IDC_EDIT_MESSAGE, m_editMessage );	

}

void CServerDialog::OnClose()
{
	ShowWindow( false );
	theApp.SetConnected( false );	
}

void CServerDialog::OnOK()
{
} 

void CServerDialog::AddText( CString strText )
{
	if ( !m_bExist )
	{
		return;
	}
	
	if ( m_editMessages.GetLineCount() > 600 )
	{
		m_editMessages.SetSel( 0, -1 );
	}
	
	m_editMessages.ReplaceSel( strText );
}
