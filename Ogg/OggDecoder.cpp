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
#include "OggDecoder.h"

COggDecoder::COggDecoder()
{
    m_pSyncState = (ogg_sync_state *)malloc( sizeof(ogg_sync_state) );
    ogg_sync_init( m_pSyncState );
}

COggDecoder::~COggDecoder()
{
    free( m_pSyncState->data );
    ogg_sync_init( m_pSyncState );
    ogg_sync_destroy( m_pSyncState );
}

char* COggDecoder::GetBuffer( const unsigned int& nBufferSize )
{
    return ogg_sync_buffer( m_pSyncState, nBufferSize );
}

int COggDecoder::Wrote( const unsigned int& nBytes )
{
    ogg_sync_wrote( m_pSyncState, nBytes );
    if( ogg_sync_pageout( m_pSyncState, &m_OggPage ) == 1 )
    {
	return ogg_page_serialno( &m_OggPage );
    }
    return -1;
}

ogg_page& COggDecoder::GetPage()
{
    return m_OggPage;
}

int COggDecoder::GetCurrentPageSerial()
{
    return ogg_page_serialno( &m_OggPage );
}
