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

CComPtr< IBaseFilter > CCaptureDevice::GetBaseFilter()
{
	return m_pBaseFilter;
}

CComPtr< IPin > CCaptureDevice::GetOutPin()
{
	return m_pOutPin;
}

CString CCaptureDevice::GetName()
{
	return m_szDeviceName;
}

CString CCaptureDevice::GetID()
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
