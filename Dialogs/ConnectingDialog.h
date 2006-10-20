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


class CConnectingDialog : public CDialog
{
	DECLARE_DYNAMIC(CConnectingDialog)

public:
	enum { IDD = IDD_DIALOG_CONNECTING };

private:
	CComboBox		m_comboServerName;
	CString			m_szSelectedServer;
	CString			m_szSelectedNick;

public:
	CConnectingDialog( CWnd* pParent = NULL );
	virtual ~CConnectingDialog();

					// shows the connection dialog, and returns selected server name, port and nick
					//
	INT_PTR			DoModalGetServer( CString& szRetServerName, CString& szRetServerPort, CString& szRetNick );

private:
	virtual void	DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg	void	OnShowWindow( BOOL bShow, UINT /*nStatus*/ );
	afx_msg void	OnBnClickedOk();
};
