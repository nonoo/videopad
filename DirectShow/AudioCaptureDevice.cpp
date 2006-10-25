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
#include "AudioCaptureDevice.h"

HRESULT CAudioCaptureDevice::Create( CString szID )
{
	return CCaptureDevice::Create( szID );
}

CAudioCaptureDevice::CAudioCaptureDevice()
{
	return;
}

void CAudioCaptureDevice::SetPreferredAudioSamplesPerSec( DWORD nSamplesPerSec )
{
	m_nPreferredSamplesPerSec = nSamplesPerSec;
}

void CAudioCaptureDevice::SetPreferredAudioBitsPerSample( WORD wBitsPerSample )
{
	m_wPreferredBitsPerSample = wBitsPerSample;
}

void CAudioCaptureDevice::SetPreferredAudioChannels( WORD nChannels )
{
	m_nPreferredChannels = nChannels;
}

void CAudioCaptureDevice::SetAudioSamplesPerSec( DWORD nSamplesPerSec )
{
	m_nSamplesPerSec = nSamplesPerSec;
}

void CAudioCaptureDevice::SetAudioBitsPerSample( WORD wBitsPerSample )
{
	m_wBitsPerSample = wBitsPerSample;
}

void CAudioCaptureDevice::SetAudioChannels( WORD nChannels )
{
	m_nChannels = nChannels;
}

const DWORD& CAudioCaptureDevice::GetPreferredSamplesPerSec()
{
	return m_nPreferredSamplesPerSec;
}

const WORD& CAudioCaptureDevice::GetPreferredBitsPerSample()
{
	return m_wPreferredBitsPerSample;
}

const WORD& CAudioCaptureDevice::GetPreferredChannels()
{
	return m_nPreferredChannels;
}

const DWORD& CAudioCaptureDevice::GetSamplesPerSec()
{
	return m_nSamplesPerSec;
}

const WORD& CAudioCaptureDevice::GetBitsPerSample()
{
	return m_wBitsPerSample;
}

const WORD& CAudioCaptureDevice::GetChannels()
{
	return m_nChannels;
}
