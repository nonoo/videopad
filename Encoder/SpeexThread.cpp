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
#include "SpeexThread.h"

IMPLEMENT_DYNCREATE(CSpeexThread, CWinThread)

BEGIN_MESSAGE_MAP(CSpeexThread, CWinThread)
	ON_THREAD_MESSAGE(WU_THREAD_STOP, OnTerminate)
	ON_THREAD_MESSAGE(WU_THREAD_PROCESS, Process)
END_MESSAGE_MAP()

BOOL CSpeexThread::InitInstance()
{
	return TRUE;
}

int CSpeexThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

void CSpeexThread::Process( WPARAM wp, LPARAM lp )
{
loop:
	if( !m_bStopThread )
	{
		if( m_pAudioGraph->IsBufferAvailable() )
		{
			m_pSpeex->FeedSample( m_pAudioGraph->GetBuffer(), m_pAudioGraph->GetBufferSize() );
		}
	}
	Sleep(1);
	if( !m_bEndThread )
	{
		goto loop;
	}
	AfxEndThread( 0, true );
}

void CSpeexThread::OnTerminate( WPARAM wp, LPARAM lp )
{
	m_bEndThread = true;
}

void CSpeexThread::Create( CSpeex* pSpeex, CAudioGraph* pAudioGraph )
{
	m_bAutoDelete = false;
	m_pSpeex = pSpeex;
	m_pAudioGraph = pAudioGraph;

	m_bEndThread = false;
	m_bStopThread = true;

	// starting thread
	//
	this->ResumeThread();
	this->PumpMessage();
	this->PostThreadMessage( WU_THREAD_PROCESS, 0, 0 );
}

void CSpeexThread::Start()
{
	m_bStopThread = false;
}

void CSpeexThread::Stop()
{
	m_bStopThread = true;
}
