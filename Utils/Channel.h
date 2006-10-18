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

#include "Client.h"
#include "Dialogs/ChatDialog.h"

class CChannel
{
private:
	CString				m_sChannelName;
	//vector< CClient >	m_vClients; faszomat, VA nem hozza elo a tagfugvenyeit.
	CArray< CClient* >	m_aClients;
	bool				m_bActive;
	
	CChatDialog			m_dlgChat;

public:	
	CChannel();
	~CChannel();
	
	void				Create( CString sChannelName );		//create channel, named sChannelName
	void				AddClient( CClient* pClient );
	void				RemoveClient( CString sUserName );
	const CString&		GetChannelName() const;
	void				Join();								// join to this channel, and create clients.

};