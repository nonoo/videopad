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
#include "TCPConnection.h"

HRESULT CTCPConnection::Connect( CString szHost, CString szPort, HWND hParentWnd )
{
	ADDRINFO hints;
	ADDRINFO* result = NULL;
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	HRESULT hr = GetAddrInfo( "0.0.0.0", szPort, &hints, &result );

	if( FAILED(hr) )
	{
		WSACleanup();
		MessageBoxA( hParentWnd, "Can't bind to 0.0.0.0\n", "Error", MB_ICONERROR | MB_OK );
		return -1;
	}

	m_Socket = INVALID_SOCKET;
	m_Socket = socket( result->ai_family, result->ai_socktype, result->ai_protocol);

	if( m_Socket == INVALID_SOCKET ) {
		MessageBoxA( hParentWnd, "Socket error\n", "Error", MB_ICONERROR | MB_OK );
		FreeAddrInfo( result );
		return -1;
	}

	hr = connect( m_Socket, result->ai_addr, (int)result->ai_addrlen);
	if ( hr == SOCKET_ERROR )
	{
		CString szErr = "Can't connect to " + szHost + ":" + szPort;
		MessageBoxA( hParentWnd, szErr, "Error", MB_ICONERROR | MB_OK );
		FreeAddrInfo( result );
		return -1;
	}

	FreeAddrInfo( result );

	return 0;
}
