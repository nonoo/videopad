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
#include "SpeexEncoder.h"

CSpeexEncoder::CSpeexEncoder( COggStream* pOggStream, CAudioGraph* pAudioGraph )
{
	pSpeex = new CSpeex( pOggStream, pAudioGraph->GetSamplesPerSec(), pAudioGraph->GetBitsPerSample(),
		pAudioGraph->GetChannels() );

	// starting encoder feeder thread
	// this feeds samples from the given graph to the encoder
	//
	pSpeexThread = (CSpeexThread *)AfxBeginThread( RUNTIME_CLASS(CSpeexThread), THREAD_PRIORITY_BELOW_NORMAL, CREATE_SUSPENDED );
	pSpeexThread->Create( pSpeex, pAudioGraph );
}

void CSpeexEncoder::Start()
{
	pSpeexThread->Start();
}

void CSpeexEncoder::Stop()
{
	pSpeexThread->Stop();
}

CSpeexEncoder::~CSpeexEncoder()
{
	// stopping thread
	//
	pSpeexThread->PostThreadMessage( WU_THREAD_STOP, 0, 0 );

	// waiting for the thread to stop
	//
	WaitForSingleObject( pSpeexThread->m_hThread, INFINITE );
	pSpeexThread = NULL;

	SAFE_DELETE( pSpeex );
}
