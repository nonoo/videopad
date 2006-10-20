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

void CConnectingDialog::OnShowWindow( BOOL bShow, UINT /*nStatus*/ )
{
	if ( bShow == TRUE )
	{
		m_comboServerName.SetItemHeight( theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ), 13 );
		m_comboServerName.DeleteString( 0 );
		for ( int i = 0; i < theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ); i++ )
		{
			CString num;
			num.Format( "%d", i+1 );
			m_comboServerName.AddString( theApp.GetSettingsFile()->Get( "RecentServers", num, "" ) );
		}
		
		m_comboServerName.SetFocus();
	}
}

void CConnectingDialog::OnBnClickedOk()
{
	m_comboServerName.GetWindowText( m_szSelectedServerName );
	
	if ( m_szSelectedServerName != "" )
	{
		this->EndModalLoop( IDOK );	
	}
	else
	{
		MessageBox( "Please enter a server host!", "Error", MB_ICONERROR | MB_OK );
	}
}

void CConnectingDialog::AddIntoRecentServerList( CString szServer )
{
	// TODO: elmentse a legutobb hasznalt portot is ehhez a szerverhez
	// megoldhato ugy is hogy a mentes host:port formaban tortenik, visszaolvasaskor meg szetszedi a hostot
	// es a portot kulon
	for( int i = theApp.GetSettingsFile()->GetInt( "Settings", "RecentServerListLength", 5 ); i > 1; i-- )
	{
		CString szI;
		szI.Format( "%d", i );
		CString szI2;
		szI2.Format( "%d", i-1 );
//		::MessageBox( NULL, szI, "a", MB_OK );
//		::MessageBox( NULL, theApp.GetSettingsFile()->Get( "RecentServers", szI2, "" ), "a", MB_OK );
		theApp.GetSettingsFile()->Set( "RecentServers", szI, theApp.GetSettingsFile()->Get( "RecentServers", szI2, "" ) );
	}
	theApp.GetSettingsFile()->Set( "RecentServers", "1", szServer );
}

INT_PTR CConnectingDialog::DoModalGetServer( CString& szRetServerName, CString& szRetServerPort, CString& szRetNick )
{
	INT_PTR iRet = this->DoModal();
	if ( iRet == IDOK )
	{
		szRetServerName = m_szSelectedServerName;
		szRetServerPort = m_szSelectedServerPort;
		szRetNick = m_szSelectedNick;
		AddIntoRecentServerList( m_szSelectedServerName );
	}
	else
	{
		szRetServerName = "";
		szRetServerPort = "";
		szRetNick = "";
	}

	return iRet;
}
