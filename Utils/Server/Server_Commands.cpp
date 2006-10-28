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
#include "../../VideoPad.h"

void CServer::DeleteChannel( CChannel* pChannel )
{
	m_mspChannels.erase( m_mspChannels.find( pChannel->GetName() ) );
	SAFE_DELETE( pChannel );
}

void CServer::PartChannel( CChannel* pChannel, CClient* pClient )
{
	// if we part the channel
	if( pClient->GetNick() == m_szNick )
	{
		DeleteChannel( pChannel );

		// if we are not on any channels
		//
		if( m_mspChannels.size() == 0 )
		{
			StopBroadcast();
		}
	}
	else
	{
		pChannel->RemoveClient( pClient );

		// if we are the only one on all of our channels
		//
		bool bStopNeeded = true;
		for( tChannelMap::iterator it = m_mspChannels.begin(); it != m_mspChannels.end(); it++ )
		{
			CChannel* pTmpChannel = it->second;
			if( pTmpChannel->GetClientNum() > 1 )
			{
				bStopNeeded = false;
			}
		}
		if( bStopNeeded )
		{
			StopBroadcast();
		}
	}

	// if the client has no channels, we delete it
	if( pClient->GetChannelNum() == 0 )
	{
		m_mspClients.erase( m_mspClients.find( pClient->GetNick() ) );
		SAFE_DELETE( pClient );
	}
}

void CServer::AddClient( CString szNick, CString szChannel )
{
	// checking if we're already on the channel
	if( m_mspChannels.count( szChannel ) == 0 )
	{
		// we're not on the channel, creating it
		CChannel* pChannel = new CChannel;
		pChannel->Create( szChannel );
		m_mspChannels[szChannel] = pChannel;
	}
	else
	{
		// we're already on the channel,
		// that means someone else has just joined
		StartBroadcast();
	}

	// removing flags from nick
	char cFlag = ' ';
	if( ( szNick[0] == '.' ) || ( szNick[0] == '@' ) )
	{
		cFlag = szNick[0];
		szNick = szNick.Mid( 1, szNick.GetLength() - 1 );
	}

	// checking if we already have that client
	if( m_mspClients.count( szNick ) == 0 )
	{
		// we don't have the client, creating it
		CClient* pClient = new CClient;
		pClient->Create( szNick );
		m_mspClients[szNick] = pClient;
	}

	// adding the client to the channel
	m_mspChannels[szChannel]->AddClient( m_mspClients[szNick] );
	m_mspChannels[szChannel]->SetClientFlag( m_mspClients[szNick], cFlag );

	StartBroadcast(); // this is here just for debugging purposes
}

void CServer::DoAutoJoin()
{
	/*		if ( m_iniManager.LoadAutoJoin() )
	{
	m_pActiveServer->JoinChannel( m_iniManager.LoadAutoJoinChannelName() );
	}
	else
	{
	if ( m_dlgChannel.DoModalJoinChannel() == IDOK )  // joining to the selected channel
	{

	}				
	}*/
}

void CServer::StartBroadcast()
{
	CTheoraEncoder* pTheoraEncoder = theApp.GetTheoraEncoder();
	CSpeexEncoder* pSpeexEncoder = theApp.GetSpeexEncoder();
	CDirectShow* pDirectShow = theApp.GetDirectShow();

	if( pTheoraEncoder == NULL )
	{
		// checking if we have video
		if( pDirectShow->GetVideoGraph() != NULL )
		{
			// initializing theora encoder
			//
			COggStream* pVideoStream = new COggStream( m_nVideoStreamSerial );
			pVideoStream->SetTCPDataConnection( m_pTCPDataConnection );
			pVideoStream->SetUDPDataConnection( m_pUDPDataConnection );
			pTheoraEncoder = new CTheoraEncoder(
				pVideoStream,
				pDirectShow->GetVideoGraph(),
				theApp.GetSettingsFile()->GetInt( "VideoEncoder", "FPS", 15 )
				);

			theApp.SetVideoStream( pVideoStream );
			theApp.SetTheoraEncoder( pTheoraEncoder );
		}
	}

	if( pSpeexEncoder == NULL )
	{
		// checking if we have audio
		if( pDirectShow->GetAudioGraph() != NULL )
		{
			// initializing speex encoder
			//
			COggStream* pAudioStream = new COggStream( m_nAudioStreamSerial );
			pAudioStream->SetTCPDataConnection( m_pTCPDataConnection );
			pAudioStream->SetUDPDataConnection( m_pUDPDataConnection );
			pSpeexEncoder = new CSpeexEncoder( pAudioStream, pDirectShow->GetAudioGraph() );

			theApp.SetAudioStream( pAudioStream );
			theApp.SetSpeexEncoder( pSpeexEncoder );
		}
	}

	if( pTheoraEncoder != NULL )
	{
		pTheoraEncoder->Start();
	}

	if( pSpeexEncoder != NULL )
	{
		pSpeexEncoder->Start();
	}
}

void CServer::StopBroadcast()
{
	CTheoraEncoder* pTheoraEncoder = theApp.GetTheoraEncoder();
	CSpeexEncoder* pSpeexEncoder = theApp.GetSpeexEncoder();

	if( pTheoraEncoder != NULL )
	{
		pTheoraEncoder->Stop();
	}

	if( pSpeexEncoder != NULL )
	{
		pSpeexEncoder->Stop();
	}
}
