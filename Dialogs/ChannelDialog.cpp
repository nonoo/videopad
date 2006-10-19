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
#include "ChannelDialog.h"


IMPLEMENT_DYNAMIC(CChannelDialog, CDialog)
CChannelDialog::CChannelDialog( CWnd* pParent /*=NULL*/)
: CDialog( CChannelDialog::IDD, pParent )
{
	m_pChannels = NULL;
}

CChannelDialog::~CChannelDialog()
{
}

BEGIN_MESSAGE_MAP(CChannelDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST_CHANNEL_CHANNELS, OnLbnSelchangeListChannelChannels)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL_JOIN, OnBnClickedJoin)	
	ON_BN_CLICKED(IDC_CHECK_CHANNEL_AUTOJOIN, OnBnClickedCheckChannelAutojoin)
END_MESSAGE_MAP()

void CChannelDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST_CHANNEL_CHANNELS, m_lbChannels );
	DDX_Control( pDX, IDC_CHECK_CHANNEL_AUTOJOIN, m_cbAutoJoin );
}

void CChannelDialog::OnBnClickedJoin()
{
	if ( m_lbChannels.GetCurSel() != -1 )
	{
		this->EndModalLoop( 1 );
	}
	else
	{
		MessageBox( "Select a Channel", "ACHTUNG!", MB_ICONWARNING | MB_OK );
	}

}

void CChannelDialog::OnShowWindow( BOOL bShow, UINT /*nStatus*/ )
{
	if ( bShow == TRUE )
	{
		m_cbAutoJoin.SetCheck( theApp.GetIniManager()->LoadAutoJoin() );
		
		// adding channel names and pointers to the listbox
		//
		for ( int i = 0; i < m_pChannels->GetCount(); i++ )
		{
			int iPos = m_lbChannels.AddString( (*m_pChannels)[i]->GetChannelName() );
			m_lbChannels.SetItemDataPtr( iPos, (void*)(*m_pChannels)[i] );
		}
	}

	// selecting first item in the listbox
	//
	m_lbChannels.SetSel( 1 );
	
	m_bReSaveAutoJoinChannelName = false;
}

// store the current selected channel's pointer
void CChannelDialog::OnLbnSelchangeListChannelChannels()
{
	int iItemIndex = m_lbChannels.GetCurSel();
	m_pSelectedChannel = (CChannel*)( m_lbChannels.GetItemDataPtr( iItemIndex ) );
}

INT_PTR CChannelDialog::DoModalJoinChannel()
{
	CServer* pActiveServer = theApp.GetActiveServer();
	if ( !pActiveServer )
	{
		return IDCANCEL;
	}
	
	m_pChannels = pActiveServer->GetChannelsArray();

	INT_PTR iRet = this->DoModal();
	if ( iRet == IDOK )
	{
		// trying to join m_pSelectedChannel
		//
		if ( !pActiveServer->JoinChannel( m_pSelectedChannel ) )
		{
			MessageBox( "Failed to join", "Warning", MB_ICONERROR | MB_OK );
			return IDCANCEL;
		}
		
		if ( m_bReSaveAutoJoinChannelName )
		{
			theApp.GetIniManager()->SaveAutoJoinChannelName( m_pSelectedChannel->GetChannelName() );
		}
				
	}
	
	return iRet;
}
void CChannelDialog::OnBnClickedCancel()
{
	m_pSelectedChannel = NULL;
	this->EndModalLoop( 0 ); // abort
}

void CChannelDialog::OnBnClickedCheckChannelAutojoin()
{
	if ( m_cbAutoJoin.GetCheck() )
	{
		theApp.GetIniManager()->SaveAutoJoin( true );
		m_bReSaveAutoJoinChannelName = true;		
	}
	else
	{
		theApp.GetIniManager()->SaveAutoJoin( false );
	}
}
