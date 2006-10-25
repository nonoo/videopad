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
#include "UDPConnection.h"

CUDPConnection::CUDPConnection( HWND hParentWnd, WORD wMessage )
{
	m_sSocket = INVALID_SOCKET;
	m_hParentWnd = hParentWnd;
	m_wMessage = wMessage;
	m_bLock = false;
}

SOCKET CUDPConnection::Connect( CString szHost, CString szPort )
{
	ADDRINFO hints;
	ADDRINFO* result = NULL;
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	HRESULT hr = GetAddrInfo( szHost, szPort, &hints, &result );

	if( ( result == NULL ) || ( FAILED(hr) ) )
	{
		throw( "failed: can't resolve host.\r\n" );
	}

	m_sSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol);

	if( m_sSocket == INVALID_SOCKET ) {
		FreeAddrInfo( result );
		throw( "failed: socket error\r\n" );
	}

	// requesting message-based notification
	WSAAsyncSelect( m_sSocket, m_hParentWnd, m_wMessage, FD_READ );
	if ( hr == SOCKET_ERROR )
	{
		FreeAddrInfo( result );
		throw( "failed: socket error\r\n" );
	}

	connect( m_sSocket, result->ai_addr, (int)result->ai_addrlen);

	FreeAddrInfo( result );

	return m_sSocket;
}

void CUDPConnection::SendData( char* pData, long nDataSize )
{
	while( m_bLock )
	{
		Sleep(1);
	}
	m_bLock = true;
	send( m_sSocket, pData, nDataSize, 0 );
	m_bLock = false;
}
