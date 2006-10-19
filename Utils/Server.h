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

#include "Utils/Sockets/TCPConnection.h"

class CServer : public CDialog
{
protected:
	CString					m_szHost;
	CString					m_szPort;
	CString					m_szNick;
	CArray< CChannel* >		m_apChannels;
	CTCPConnection*			m_pTCPConnection;
	
public:	
	CServer();
	~CServer();

	HRESULT						Create( CString szHost, CString szPort, CString szNick );
	const CString&				GetServerHost() const;
	const CString&				GetServerPort() const;
	const CString&				GetNick() const;
	CArray< CChannel* >* const	GetChannelsArray();

	bool						JoinChannel( CChannel* pChannel );
	bool						JoinChannel( CString szChannelName );


	
	
////////////////////////////////////////////////////////////////////////////////////////////////

	// status dialog
	//
public:
	void			AddText( CString szText );  // add "\r\n" after szText
	
protected:
	CEdit			m_editMessages;
	CEdit			m_editMessage;



	virtual void	DoDataExchange( CDataExchange* pDX );
	virtual BOOL	PreTranslateMessage( MSG* pMsg );
	virtual void	OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void	OnClose();	
};
