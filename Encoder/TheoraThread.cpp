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
#include "TheoraThread.h"

IMPLEMENT_DYNCREATE(CTheoraThread, CWinThread)

BEGIN_MESSAGE_MAP(CTheoraThread, CWinThread)
	ON_THREAD_MESSAGE(WU_THREAD_STOP, OnTerminate)
	ON_THREAD_MESSAGE(WM_TIMER, OnTimer)
END_MESSAGE_MAP()

BOOL CTheoraThread::InitInstance()
{
	m_pTimer = SetTimer( NULL, 0, 1000/m_nEncoderFPS, NULL );

	return TRUE;
}

int CTheoraThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

void CTheoraThread::OnTerminate( WPARAM wp, LPARAM lp )
{
	KillTimer( NULL, m_pTimer );
	AfxEndThread( 0, true );
}

void CTheoraThread::OnTimer( WPARAM wp, LPARAM lp )
{
	if( m_pVideoGraph )
	{
		m_pTheora->FeedFrame( m_pVideoGraph->GetBuffer(), m_pVideoGraph->GetBufferSize() );
	}
}

void CTheoraThread::Create( CTheora* pTheora, CVideoGraph* pVideoGraph, UINT nEncoderFPS )
{
	m_bAutoDelete = false;
	m_nEncoderFPS = nEncoderFPS;
	m_pTheora = pTheora;
	m_pVideoGraph = pVideoGraph;
}
