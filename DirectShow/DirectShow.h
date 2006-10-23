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

class CDirectShowGraph;
class CVideoCaptureDevice;
class CAudioCaptureDevice;

class CDirectShow
{
public:
	CDirectShow();
	~CDirectShow();

private:
	// returns the autodetected video/audio capture device's COM ID
	//
	void			AutoDetectVideoCaptureDevice( CString& szVideoCaptureDeviceID );
	void			AutoDetectAudioCaptureDevice( CString& szVideoCaptureDeviceID );

	void			InitCaptureDevices();



	// for asking default capture devices
	CDirectShowGraph*		m_pDirectShowGraph;
	CVideoCaptureDevice*	m_pVideoCaptureDevice;
	CAudioCaptureDevice*	m_pAudioCaptureDevice;
};
