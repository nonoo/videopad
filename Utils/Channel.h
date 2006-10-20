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

#include "../Dialogs/ChatDialog.h"

class CClient;

class CChannel
{
private:
	CString							m_szName;
	CArray< CClient* >				m_aClients;
	time_t							m_tCreationTime;
	CChatDialog						m_dlgChat;
	typedef map< CClient*,char >	tFlagMap;
	tFlagMap						m_mpcClientFlags;

public:	
	CChannel();
	~CChannel();

	void						Create( CString szName );

	void						AddClient( CClient* pClient );
	void						RemoveClient( CClient* pClient );
	void						SetClientFlag( CClient* pClient, char cFlag );
	const CString&				GetName();
	INT_PTR						GetClientNum();
	const CArray< CClient* >&	GetClients();
	void						SetCreationTime( time_t nCreationTime );
	const time_t&				GetCreationTime();
};
