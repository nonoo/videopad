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
#include "CaptureDevice.h"

CCaptureDevice::CCaptureDevice( LPWSTR szName )
{
	HRESULT hr;

	// create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

	// enumerate video capture devices
	CComPtr< IEnumMoniker > pEm;
	pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0);

	if ( !pEm )
	{
		MessageBox( NULL, "Error: no video capture devices found!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	pEm->Reset();

	IBindCtx* pBindCtx;
	hr = ::CreateBindCtx (0, &pBindCtx);

	if( hr != S_OK )
	{
		MessageBox( NULL, "Error: the given capture device is not valid!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	ULONG ulFetched;
	CComPtr<IMoniker> pM;

	// getting device moniker from given id (szName)
	//
	hr = MkParseDisplayName( pBindCtx, szName, &ulFetched, &pM );
	SAFE_RELEASE( pBindCtx );

	if( hr != S_OK )
	{
		MessageBox( NULL, "Error: the given capture device is not valid!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	// ask for the actual filter
	hr = pM->BindToObject(0,0,IID_IBaseFilter, (void **)&m_pBaseFilter);
	if( hr != S_OK )
	{
		MessageBox( NULL, "Error: the given capture device is not valid!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	// get the property bag interface from the moniker
	CComPtr< IPropertyBag > pBag;
	hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
	if( hr != S_OK )
	{
		MessageBox( NULL, "Error: the given capture device is not valid!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	// ask for the english-readable name
	CComVariant var;
	var.vt = VT_BSTR;
	pBag->Read( L"FriendlyName", &var, NULL );

	LPOLESTR szDeviceID;
	pM->GetDisplayName( 0, NULL, &szDeviceID );

	m_szID = szName;
	m_szDeviceName = var.bstrVal;
}

CComPtr< IBaseFilter >& CCaptureDevice::GetBaseFilter()
{
	return m_pBaseFilter;
}

CComPtr< IPin >& CCaptureDevice::GetOutPin()
{
	return m_pOutPin;
}

const CString& CCaptureDevice::GetName()
{
	return m_szDeviceName;
}

const CString& CCaptureDevice::GetID()
{
	return m_szID;
}

CCaptureDevice::CCaptureDevice()
{
	return;
}

CCaptureDevice::~CCaptureDevice()
{
	m_pBaseFilter.Release();
	m_pOutPin.Release();
}

void CCaptureDevice::SetOutPin( CComPtr< IPin > Pin )
{
	m_pOutPin = Pin;
}

void CCaptureDevice::SetBaseFilter( CComPtr< IBaseFilter > Filter )
{
	m_pBaseFilter = Filter;
}

void CCaptureDevice::SetDeviceName( CString szNewDeviceName )
{
	m_szDeviceName = szNewDeviceName;
}

void CCaptureDevice::SetDeviceID( CString szNewID )
{
	m_szID = szNewID;
}
