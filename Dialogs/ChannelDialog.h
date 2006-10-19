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

#pragma once

#include "Utils/Channel.h"

class CChannelDialog : public CDialog
{
	DECLARE_DYNAMIC(CChannelDialog)

private:
	CListBox				m_lbChannels;
	CButton					m_cbAutoJoin;
	CChannel*				m_pSelectedChannel;
	
	bool					m_bReSaveAutoJoinChannelName;
		
	CArray< CChannel* >*	m_pChannels;

	
public:
	enum { IDD = IDD_DIALOG_CHANNEL };
	
public:
	CChannelDialog( CWnd* pParent = NULL );
	virtual ~CChannelDialog();

						// returns the selected channel's pointer
						//
	INT_PTR				DoModalJoinChannel();

protected:
	virtual void		DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void		OnBnClickedJoin();
	afx_msg void		OnLbnSelchangeListChannelChannels();
	afx_msg	void		OnShowWindow( BOOL bShow, UINT /*nStatus*/ );
	afx_msg void		OnBnClickedCancel();
	afx_msg void		OnBnClickedCheckChannelAutojoin();
};
