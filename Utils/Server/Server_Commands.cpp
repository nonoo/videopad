#include "stdafx.h"
#include "Server.h"

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
	}
	else
	{
		pChannel->RemoveClient( pClient );
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
}
