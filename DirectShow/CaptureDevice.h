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

// this class represents a directshow capture device
//
class CCaptureDevice
{
public:
	CCaptureDevice();
	~CCaptureDevice();

								// init capture device by given COM object ID
								//
	HRESULT						Create( CString szID );

								// retrieves device's COM object ID
								//
	const CString&				GetID();

								// device name
								//
	const CString&				GetName();

	CComPtr< IBaseFilter >&		GetBaseFilter();
	CComPtr< IPin >&			GetOutPin();



	void						SetOutPin( CComPtr< IPin > Pin );
	void						SetBaseFilter( CComPtr< IBaseFilter > Filter );
	void						SetDeviceName( CString szNewDeviceName );
	void						SetDeviceID( CString szNewID );

private:
	CComPtr< IBaseFilter >		m_pBaseFilter;
	CComPtr< IPin >				m_pOutPin;

	CString						m_szDeviceName;
	CString						m_szID;
};
