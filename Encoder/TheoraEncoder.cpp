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
#include "TheoraEncoder.h"

CTheoraEncoder::CTheoraEncoder( COggStream* pOggStream, CVideoGraph* pVideoGraph, UINT nEncoderFPS )
{
	m_pTheora = new CTheora( pOggStream, pVideoGraph->GetVideoWidth(), pVideoGraph->GetVideoHeight(), nEncoderFPS );

	// starting encoder feeder thread
	// this feeds images from the given graph to the encoder
	//
	m_pTheoraThread = (CTheoraThread *)AfxBeginThread( RUNTIME_CLASS(CTheoraThread), THREAD_PRIORITY_BELOW_NORMAL, CREATE_SUSPENDED );
	m_pTheoraThread->Create( m_pTheora, pVideoGraph, nEncoderFPS );
}

void CTheoraEncoder::Stop()
{
	m_pTheoraThread->SuspendThread();
}

void CTheoraEncoder::Start()
{
	m_pTheoraThread->ResumeThread();
}

CTheoraEncoder::~CTheoraEncoder()
{
	// stopping thread
	//
	m_pTheoraThread->PostThreadMessage( WU_THREAD_STOP, 0, 0 );
	
	// waiting for the thread to stop
	//
	WaitForSingleObject( m_pTheoraThread->m_hThread, INFINITE );
	m_pTheoraThread = NULL;

	SAFE_DELETE( m_pTheora );
}
