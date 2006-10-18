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
#include "Client.h"


CClient::CClient()
{
	m_pdlgVideo = NULL;
}

CClient::~CClient()
{
	SAFE_DELETE( m_pdlgVideo );
}


void CClient::Create( CString sName, bool bOpenVideoDlg )
{
	m_sName = sName;
	
	if ( bOpenVideoDlg )
	{
		m_pdlgVideo = new CVideoDialog;
		m_pdlgVideo->Create( IDD_DIALOG_VIDEO );
		m_pdlgVideo->SetWindowText( m_sName );
		m_pdlgVideo->ShowWindow( SW_SHOW );
	}
}

const CString& CClient::GetName() const
{
	return m_sName;
}