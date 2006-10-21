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

class CVideoPadApp : public CWinApp
{
private:
	CSettingsFile		m_SettingsFile;

	CChannelDialog		m_dlgChannel;
	CConnectingDialog	m_dlgConnecting;

	bool				m_bConnected;
	CServer*			m_pActiveServer;
	
	// winsock
	WSADATA				m_wsaData;

						// for asking default capture devices
	CDirectShowGraph*	DirectShowGraph;

public:
	CVideoPadApp();
	~CVideoPadApp();
	virtual BOOL	InitInstance();

	void			Connect();
	void			Disconnect();

	void			SetConnected( bool bConnected );
	const bool&		GetConnected();

	CServer*		GetActiveServer();
	CSettingsFile*	GetSettingsFile();

	//	CMainFrame*		GetMainFrame();
	
private:
	void			OnPressConnect();
	void			OnPressChannel();

	DECLARE_MESSAGE_MAP()
};

extern CVideoPadApp theApp;
