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

class CTCPConnection;
class CUDPConnection;

class CServer : public CDialog
{
private:
	CString						m_szHost;
	CString						m_szPort;
	CString						m_szNick;

	typedef map< CString, CChannel* > tChannelMap;
	tChannelMap					m_mspChannels;
	typedef map< CString, CClient* > tClientMap;
	tClientMap					m_mspClients;

	UINT						m_nVideoStreamSerial;
	UINT						m_nAudioStreamSerial;
	UINT						m_nTCPDataPort;
	UINT						m_nUDPDataPort;

	CTCPConnection*				m_pTCPControlConnection;
	CTCPConnection*				m_pTCPDataConnection;
	CUDPConnection*				m_pUDPDataConnection;
	SOCKET						m_sSocket;
	char						m_pRecvBuf[MAXMESSAGELENGTH+1];

	// in Server.cpp
public:
	CServer();
	~CServer();

	void						Connect( CString szHost, CString szPort, CString szNick );
	const CString&				GetHost();
	const CString&				GetPort();
	const CString&				GetNick();
	const SOCKET&				GetSocket();

private:
								// this is used only for closing TCP/UDP connections
								// to fully disconnect, call theApp.Disconnect()
	void						Disconnect();

								// adds the given server to the top of the recent server list
								//
	void						AddIntoRecentServerList( CString szServer );

	afx_msg	LRESULT				OnDataSocketEvent( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT				OnControlSocketEvent( WPARAM wParam, LPARAM lParam );

	// in Server_ProcessMessages.cpp
								// processes messages received from the server
	void						ProcessServerMessage( CString szLine );

	// in Server_Commands.cpp
	void						DeleteChannel( CChannel* pChannel );
	void						PartChannel( CChannel* pChannel, CClient* pClient );
	void						AddClient( CString szNick, CString szChannel );
	void						DoAutoJoin();

	// in Server_Dialog.cpp
public:
	void			AddText( CString szText );  // add "\r\n" after szText
	
private:
	CEdit			m_editMessages;
	CEdit			m_editMessage;

	virtual void	DoDataExchange( CDataExchange* pDX );
	virtual BOOL	PreTranslateMessage( MSG* pMsg );
	virtual void	OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void	OnClose();	
};
