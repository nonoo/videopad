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

class CVideoDialog;

class CClient
{
private:
	CString				m_szNick;
	CVideoDialog*		m_pdlgVideo;
	CArray< CChannel* >	m_apChannels;

public:
	CClient();
	~CClient();
	
	void						Create( CString szNick );
	void						SetNick( CString szNick );
	const CString&				GetNick();
	
	void						AddChannel( CChannel* pChannel );
	void						RemoveChannel( CChannel* pChannel );

								// returns the number of channels the client is on
	INT_PTR						GetChannelNum();
	const CArray< CChannel* >&	GetChannels();
};