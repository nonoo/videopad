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
#include "SpeexDecoder.h"

IMPLEMENT_DYNCREATE(CSpeexDecoder, CWinThread)

BEGIN_MESSAGE_MAP(CSpeexDecoder, CWinThread)
END_MESSAGE_MAP()

BOOL CSpeexDecoder::InitInstance()
{
	return TRUE;
}

int CSpeexDecoder::ExitInstance()
{
	return CWinThread::ExitInstance();
}

HRESULT CSpeexDecoder::PreProcess( ogg_packet* pOggPacket )
{
	return -1;
}
