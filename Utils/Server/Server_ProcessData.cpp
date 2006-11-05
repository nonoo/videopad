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

void CServer::ProcessIncomingData()
{
	int res = recv( m_pTCPDataConnection->GetSocket(), m_OggDecoder.GetBuffer( MAXDATAREAD ), MAXDATAREAD, 0 );
	if( res <= 0 )
	{
		AddText( "Disconnected.\r\n" );
		Disconnect();
		return;
	}

	int nCurrentPageSerial = m_OggDecoder.Wrote( res );
	if( nCurrentPageSerial >= 0 )
	{
		// we got an ogg page
		//
		COggStream* pOggStream = m_SerialMapper.GetOggStream( nCurrentPageSerial );

		if( pOggStream == NULL )
		{
			// we're not processing this stream yet
			//
			/*CVideoDialog* m_pdlgVideo = new CVideoDialog;
			m_pdlgVideo->Create( IDD_DIALOG_VIDEO );
			m_pdlgVideo->SetWindowText( m_szNick );
			m_pdlgVideo->ShowWindow( SW_SHOW );*/
			pOggStream = new COggStream( nCurrentPageSerial );
			m_SerialMapper.AddSerial( nCurrentPageSerial, NULL, pOggStream );
		}

		HRESULT hr = pOggStream->FeedPage( m_OggDecoder.GetPage() );
		if( FAILED( hr ) )
		{
			m_SerialMapper.DestroyStream( pOggStream );
		}
	}
}
