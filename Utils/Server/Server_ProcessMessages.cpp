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
#include "Server.h"

void CServer::ProcessServerMessage( CString szLine )
{
	// tokenizing szLine
	//
	CArray< CString > szTokens;
	int nCurPos = 0;
	while( nCurPos < szLine.GetLength() )
	{
		szTokens.Add( szLine.Tokenize( " ", nCurPos ) );
	}

	if( szTokens.GetCount() == 0 )
	{
		return;
	}

	CString& szCommand = szTokens[0];

	// szText is the string after ':'
	//
	CString szText;
	int nCommaPos = szLine.Find( ':', 0 );
	if( nCommaPos )
	{
		szText = szLine.Mid( nCommaPos+1, szLine.GetLength()-nCommaPos-1 );
	}




	// for the command list, see videopad-server's README file
	//
	if( szCommand == "001" ) // welcome msg
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "002" ) // server version
	{
		AddText( szText + "\r\n" );
		// sending nick
		m_pTCPControlConnection->SendMessage( "nick " + m_szNick + "\r\n" );
		return;
	}

	if( szCommand == "003" ) // MOTD start
	{
		AddText( "\r\nMessage Of The Day:\r\n" );
		return;
	}

	if( szCommand == "004" ) // MOTD line
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "005" ) // MOTD end
	{
		AddText( "\r\n" );
		return;
	}

	if( szCommand == "006" ) // password needed
	{
		CString szPass;
		// TODO: login ablakot felnyisson modalisan, bekerje a szerver jelszavat a felhasznalotol
		m_pTCPControlConnection->SendMessage( "pass " + szPass );
		return;
	}

	if( szCommand == "007" ) // wrong pass
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "008" ) // our unique video & audio stream serials
	{
		m_nVideoStreamSerial = atoi( szTokens[1] );
		m_nAudioStreamSerial = atoi( szTokens[2] );
		return;
	}

	if( szCommand == "009" ) // login completed
	{
		AddText( szText + "\r\n\r\n" );

		// connecting to the data port
		AddText( "Connecting to server's TCP data port..." );

		// connecting the tcp data socket
		m_pTCPDataConnection = new CTCPConnection( GetSafeHwnd(), WU_DATASOCKET_EVENT );
		try
		{
			CString szPort;
			szPort.Format( "%d", m_nTCPDataPort );
			m_pTCPDataConnection->Connect( m_szHost, szPort );
			//m_pTCPDataConnection->Connect( m_szHost, "62333" );
		}
		catch ( char* pError )
		{
			AddText( pError );
			Disconnect();
			return;
		}

		// requesting channel list
		m_pTCPControlConnection->SendMessage( "chanlist\r\n" );

		return;
	}

	if( szCommand == "010" ) // login timeout
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "011" ) // server's tcp+udp data port numbers
	{
		m_nTCPDataPort = atoi( szTokens[1] );
		m_nUDPDataPort = atoi( szTokens[2] );
		return;
	}

	if( szCommand == "300" ) // somebody joins the channel (or we)
	{
		AddClient( szTokens[2], szTokens[1] );
		return;
	}

	if( szCommand == "301" ) // channel creation time
	{
		time_t tCreationTime = atol(szTokens[2]);
		m_mspChannels[szTokens[1]]->SetCreationTime( tCreationTime );
		return;
	}

	if( szCommand == "304" ) // quit
	{
		PartChannel( m_mspChannels[szTokens[1]], m_mspClients[szTokens[2]] );
		return;
	}

	if( szCommand == "305" ) // part
	{
		PartChannel( m_mspChannels[szTokens[1]], m_mspClients[szTokens[2]] );
		return;
	}

	if( szCommand == "306" ) // nick change
	{
		m_mspClients[szTokens[2]]->SetNick( szTokens[3] );
		return;
	}

	if( szCommand == "350" ) // listing channels
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "351" ) // channel
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "352" ) // end of channel list
	{
		AddText( szText + "\r\n" );
		return;
	}

	if( szCommand == "500" ) // channel message
	{
		return;
	}

	if( szCommand == "502" ) // our nick has changed
	{
		m_szNick = szTokens[1];
		AddText( "Your nick is now " + m_szNick + "\r\n" );
		return;
	}

	if( szCommand == "503" ) // PING
	{
		m_pTCPControlConnection->SendMessage( "pong\r\n" );
		return;
	}

	AddText( szLine + "\r\n" );
}
