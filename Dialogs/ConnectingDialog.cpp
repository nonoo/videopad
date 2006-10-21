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
#include "ConnectingDialog.h"



IMPLEMENT_DYNAMIC(CConnectingDialog, CDialog)

BEGIN_MESSAGE_MAP(CConnectingDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

CConnectingDialog::CConnectingDialog(CWnd* pParent /*=NULL*/)
: CDialog(CConnectingDialog::IDD, pParent)
{
}

CConnectingDialog::~CConnectingDialog()
{
}

void CConnectingDialog::DoDataExchange( CDataExchange* pDX )
{
	DDX_Control( pDX, IDC_COMBO_CONNECT_SERVERNAME, m_comboServerName );
	CDialog::DoDataExchange( pDX );
}

BOOL CConnectingDialog::PreTranslateMessage( MSG* pMsg )
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			OnBnClickedOk();
		}
	}

	return CDialog::PreTranslateMessage( pMsg );
}

void CConnectingDialog::OnShowWindow( BOOL bShow, UINT /*nStatus*/ )
{
	if ( bShow == TRUE )
	{
		m_comboServerName.SetItemHeight( theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ), 13 );
		m_comboServerName.DeleteString( 0 );
		for ( int i = 0; i < theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ); i++ )
		{
			m_comboServerName.AddString( theApp.GetSettingsFile()->Get( "RecentServers", i+1, "" ) );
		}
		
		m_comboServerName.SetFocus();
	}
}

void CConnectingDialog::OnBnClickedOk()
{
	m_comboServerName.GetWindowText( m_szSelectedServer );
	
	if ( m_szSelectedServer != "" )
	{
		this->EndModalLoop( IDOK );	
	}
	else
	{
		MessageBox( "Please enter a server host!", "Error", MB_ICONERROR | MB_OK );
	}
}

INT_PTR CConnectingDialog::DoModalGetServer( CString& szRetServerName, CString& szRetServerPort, CString& szRetNick )
{
	INT_PTR iRet = this->DoModal();
	if ( iRet == IDOK )
	{
		// checking for host:port format
		int nCommaPos = m_szSelectedServer.Find( ':' );
		CString szPort = "62320"; // default VideoPad control port
		if( nCommaPos > 0 )
		{
			szPort = m_szSelectedServer.Mid( nCommaPos + 1, m_szSelectedServer.GetLength() - nCommaPos - 1 );
			m_szSelectedServer = m_szSelectedServer.Left( nCommaPos );
		}
		szRetServerName = m_szSelectedServer;
		szRetServerPort = szPort;
		//szRetNick = m_szSelectedNick;
		szRetNick = "TestNick";
	}
	else
	{
		szRetServerName = "";
		szRetServerPort = "";
		szRetNick = "";
	}

	return iRet;
}
