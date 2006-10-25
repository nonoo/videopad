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

#pragma once

#include "CaptureDevice.h"

class CAudioCaptureDevice : public CCaptureDevice
{
public:
	CAudioCaptureDevice();

	// init capture device by given COM object ID
	//
	HRESULT			Create( CString szID );

	const DWORD&	GetPreferredSamplesPerSec();
	const WORD&		GetPreferredBitsPerSample();
	const WORD&		GetPreferredChannels();

	const DWORD&	GetSamplesPerSec();
	const WORD&		GetBitsPerSample();
	const WORD&		GetChannels();

	void			SetPreferredAudioSamplesPerSec( DWORD nSamplesPerSec );
	void			SetPreferredAudioBitsPerSample( WORD wBitsPerSample );
	void			SetPreferredAudioChannels( WORD nChannels );

	// these functions are used by CAudioGraph to set
	// the current audio format information of the graph
	// and the device
	//
	void			SetAudioSamplesPerSec( DWORD nSamplesPerSec );
	void			SetAudioBitsPerSample( WORD wBitsPerSample );
	void			SetAudioChannels( WORD nChannels );

private:
	DWORD			m_nPreferredSamplesPerSec;
	WORD			m_wPreferredBitsPerSample;
	WORD			m_nPreferredChannels;

	DWORD			m_nSamplesPerSec;
	WORD			m_wBitsPerSample;
	WORD			m_nChannels;
};
