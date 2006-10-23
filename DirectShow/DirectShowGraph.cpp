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
#include "DirectShowGraph.h"

HRESULT CDirectShowGraph::GetPin( IBaseFilter* pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin )
{
	CComPtr< IEnumPins > pEnum;
	*ppPin = NULL;

	HRESULT hr = pFilter->EnumPins( &pEnum );
	if( FAILED( hr ) ) 
	{
		return hr;
	}

	ULONG ulFound;
	IPin *pPin;
	hr = E_FAIL;

	while( S_OK == pEnum->Next( 1, &pPin, &ulFound ) )
	{
		PIN_DIRECTION pindir = (PIN_DIRECTION)3;

		pPin->QueryDirection( &pindir );
		if( pindir == dirrequired )
		{
			if( iNum == 0 )
			{
				*ppPin = pPin;  // Return the pin's interface
				hr = S_OK;      // Found requested pin, so clear error
				break;
			}
			iNum--;
		} 

		SAFE_RELEASE( pPin );
	} 

	return hr;
}

IPin* CDirectShowGraph::GetInPin( IBaseFilter * pFilter, int nPin )
{
	CComPtr<IPin> pComPin = 0;
	GetPin(pFilter, PINDIR_INPUT, nPin, &pComPin);
	return pComPin;
}

IPin* CDirectShowGraph::GetOutPin( IBaseFilter * pFilter, int nPin )
{
	CComPtr<IPin> pComPin = 0;
	GetPin(pFilter, PINDIR_OUTPUT, nPin, &pComPin);
	return pComPin;
}

CDirectShowGraph::CDirectShowGraph()
{
	// create the filter graph manager
	//
	m_pGraph.CoCreateInstance( CLSID_FilterGraph );

	if( !m_pGraph )
	{
		MessageBox( NULL, "DirectShow Error: Can't create IGraphBuilder!", "Error", MB_ICONSTOP );
		exit(-1);
	}
}

CDirectShowGraph::~CDirectShowGraph()
{
	// destroy capture graph
	if( m_pGraph )
	{
		// have to wait for the graph to stop first
		//
		Stop();

		// make the window go away before we release graph
		// or we'll leak memory/resources
		// 
		CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pGraph;
		if( pWindow )
		{
			pWindow->put_Visible( OAFALSE );
			pWindow->put_Owner( NULL );
		}

#ifdef DEBUG
		// Remove filter graph from the running object table   
		if (m_dwGraphRegister)
		{
			RemoveGraphFromRot(m_dwGraphRegister);
		}
#endif

		m_pSourceFilter.Release();
		m_pGrabber.Release();
		m_pGraph.Release();
	}
}

#ifdef DEBUG
HRESULT CDirectShowGraph::AddGraphToRot( IUnknown *pUnkGraph, DWORD *pdwRegister )
{
	IMoniker* pMoniker;
	IRunningObjectTable *pROT;
	WCHAR wsz[128];
	HRESULT hr;

	if ( !pUnkGraph || !pdwRegister )
	{
		return E_POINTER;
	}

	if ( FAILED( GetRunningObjectTable( 0, &pROT ) ) )
		return E_FAIL;

	hr = StringCchPrintfW( wsz, NUMELMS(wsz), L"FilterGraph %08x pid %08x\0", (DWORD_PTR)pUnkGraph, 
		GetCurrentProcessId() );

	hr = CreateItemMoniker( L"!", wsz, &pMoniker );
	if ( SUCCEEDED(hr) ) 
	{
		// Use the ROTFLAGS_REGISTRATIONKEEPSALIVE to ensure a strong reference
		// to the object.  Using this flag will cause the object to remain
		// registered until it is explicitly revoked with the Revoke() method.
		//
		// Not using this flag means that if GraphEdit remotely connects
		// to this graph and then GraphEdit exits, this object registration 
		// will be deleted, causing future attempts by GraphEdit to fail until
		// this application is restarted or until the graph is registered again.
		hr = pROT->Register( ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, 
			pMoniker, pdwRegister );
		SAFE_RELEASE( pMoniker );
	}

	SAFE_RELEASE( pROT );
	return hr;
}

void CDirectShowGraph::RemoveGraphFromRot( DWORD pdwRegister )
{
	IRunningObjectTable *pROT;

	if ( SUCCEEDED( GetRunningObjectTable( 0, &pROT ) ) )
	{
		pROT->Revoke(pdwRegister);
		SAFE_RELEASE( pROT );
	}
}
#endif

void CDirectShowGraph::ListVideoCaptureDevices()
{
	CComPtr< IBaseFilter >* ppCap;
	HRESULT hr;

	printf( "listing video capture devices:\n" );
//	FILE* fp;
//	fopen_s( &fp, "devices_videocapture.txt", "w" );

	// create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

	// enumerate video capture devices
	CComPtr< IEnumMoniker > pEm;
	pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0);
	pCreateDevEnum.Release();

	if ( !pEm )
	{
		MessageBox( NULL, "Error: no video capture devices found!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	pEm->Reset();

	UINT i = 0;
	while (true)
	{
		i++;

		ULONG ulFetched = 0;

		CComPtr<IMoniker> pM;
		hr = pEm->Next(1, &pM, &ulFetched);
		if( hr != S_OK )
		{
			// no video capture devices found
			break;
		}

		// ask for the actual filter
		hr = pM->BindToObject(0,0,IID_IBaseFilter, (void **) &ppCap);
		// if we found a device
		if (*ppCap)
		{
			// get the property bag interface from the moniker
			CComPtr< IPropertyBag > pBag;
			hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
			if( hr != S_OK )
			{
				continue;
			}

			// ask for the english-readable name
			CComVariant var;
			var.vt = VT_BSTR;
			pBag->Read( L"FriendlyName", &var, NULL );

			LPOLESTR szDeviceID;
			pM->GetDisplayName( 0, NULL, &szDeviceID );

			// device seems ok
			printf( "device #%d:\n", i );
			wprintf( L"%s\n", var.bstrVal ); // device name
			wprintf( L"%s\n", szDeviceID );

			//fprintf( fp, "device #%d:\n", i );
			//fwprintf( fp, L"%s\n", var.bstrVal ); // device name
			//fwprintf( fp, L"%s\n", szDeviceID );

			SAFE_RELEASE( ppCap );
		}
	}
	printf( "\n" );
//	fclose( fp );
}

CVideoCaptureDevice* CDirectShowGraph::GetDefaultVideoCaptureDevice()
{
	CVideoCaptureDevice* VCD = new CVideoCaptureDevice;
	HRESULT hr;
	CComPtr< IBaseFilter > pSourceFilter;

	// create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

	// enumerate video capture devices
	CComPtr< IEnumMoniker > pEm;
	pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0);
	pCreateDevEnum.Release();

	if ( !pEm )
	{
		MessageBox( NULL, "Error: no video capture devices found!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	pEm->Reset();

	// go through and find first capture device
	while (true)
	{
		ULONG ulFetched = 0;

		CComPtr<IMoniker> pM;
		hr = pEm->Next(1, &pM, &ulFetched);
		if( hr != S_OK )
		{
			MessageBox( NULL, "Error: no suitable video capture device found!", "Error", MB_ICONSTOP );
			exit(-1);
		}

		// ask for the actual filter
		hr = pM->BindToObject(0,0,IID_IBaseFilter, (void **)&pSourceFilter);
		// if we found a device
		if (pSourceFilter)
		{
			// get the property bag interface from the moniker
			CComPtr< IPropertyBag > pBag;
			hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
			if( hr != S_OK )
			{
				continue;
			}

			// ask for the english-readable name
			CComVariant var;
			var.vt = VT_BSTR;
			pBag->Read( L"FriendlyName", &var, NULL );

			LPOLESTR szDeviceID;
			pM->GetDisplayName( 0, NULL, &szDeviceID );

			// device seems ok
			printf( "using video device #1:\n" );
			wprintf( L"%s\n", var.bstrVal ); // device name
			wprintf( L"%s\n", szDeviceID );

			TCHAR* lpszDeviceName = new TCHAR[1000];
			TCHAR* lpszDeviceID = new TCHAR[1000];
			WideCharToLocal( lpszDeviceName, var.bstrVal, 1000 );
			WideCharToLocal( lpszDeviceID, szDeviceID, 1000 );

			VCD->SetDeviceName( lpszDeviceName );
			VCD->SetDeviceID( lpszDeviceID );
			VCD->SetBaseFilter( pSourceFilter );

			pSourceFilter.Release();
			break;
		}
	}
	printf( "\n" );

	return VCD;
}

int CDirectShowGraph::WideCharToLocal( LPTSTR pLocal, LPCWSTR pWide, DWORD dwChars )
{
	if(!pLocal || !pWide)
	{
		return 0;
	}

	*pLocal = 0;

#ifdef UNICODE
	lstrcpyn(pLocal, pWide, dwChars);
#else
	WideCharToMultiByte( CP_ACP, 
		0, 
		pWide, 
		-1, 
		pLocal, 
		dwChars, 
		NULL, 
		NULL);
#endif

	return lstrlen(pLocal);
}

void CDirectShowGraph::Start()
{
	CComQIPtr< IMediaControl, &IID_IMediaControl > pControl( m_pGraph );
	HRESULT hr = pControl->Run();
	pControl.Release();

	if (FAILED(hr))
	{
		MessageBox( NULL, "DirectShow Error: can't start capture graph!", "Error", MB_ICONSTOP );
		exit(-1);
	}
}

void CDirectShowGraph::Stop()
{
	CComQIPtr< IMediaControl, &IID_IMediaControl > pControl( m_pGraph );
	pControl->Stop();
	pControl.Release();
}

CAudioCaptureDevice* CDirectShowGraph::GetDefaultAudioCaptureDevice()
{
	CAudioCaptureDevice* ACD = new CAudioCaptureDevice;
	HRESULT hr;
	CComPtr< IBaseFilter > pSourceFilter;

	// create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

	// enumerate audio capture devices
	CComPtr< IEnumMoniker > pEm;
	pCreateDevEnum->CreateClassEnumerator( CLSID_AudioInputDeviceCategory, &pEm, 0);
	pCreateDevEnum.Release();

	if ( !pEm )
	{
		MessageBox( NULL, "Error: no audio capture devices found!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	pEm->Reset();

	// go through and find first capture device
	while (true)
	{
		ULONG ulFetched = 0;

		CComPtr<IMoniker> pM;
		hr = pEm->Next(1, &pM, &ulFetched);
		if( hr != S_OK )
		{
			MessageBox( NULL, "Error: no suitable audio capture device found!", "Error", MB_ICONSTOP );
			exit(-1);
		}

		// ask for the actual filter
		hr = pM->BindToObject(0,0,IID_IBaseFilter, (void **)&pSourceFilter);
		// if we found a device
		if (pSourceFilter)
		{
			// get the property bag interface from the moniker
			CComPtr< IPropertyBag > pBag;
			hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
			if( hr != S_OK )
			{
				continue;
			}

			// ask for the english-readable name
			CComVariant var;
			var.vt = VT_BSTR;
			pBag->Read( L"FriendlyName", &var, NULL );

			LPOLESTR szDeviceID;
			pM->GetDisplayName( 0, NULL, &szDeviceID );

			// device seems ok
			printf( "using audio device #1:\n" );
			wprintf( L"%s\n", var.bstrVal ); // device name
			wprintf( L"%s\n", szDeviceID );

			TCHAR* lpszDeviceName = new TCHAR[1000];
			TCHAR* lpszDeviceID = new TCHAR[1000];
			WideCharToLocal( lpszDeviceName, var.bstrVal, 1000 );
			WideCharToLocal( lpszDeviceID, szDeviceID, 1000 );

			ACD->SetDeviceName( lpszDeviceName );
			ACD->SetDeviceID( lpszDeviceID );
			ACD->SetBaseFilter( pSourceFilter );

			pSourceFilter.Release();
			break;
		}
	}
	printf( "\n" );

	return ACD;
}

void CDirectShowGraph::ListAudioCaptureDevices()
{
	CComPtr< IBaseFilter >* ppCap;
	HRESULT hr;

	printf( "listing audio capture devices:\n" );
//	FILE* fp;
//	fopen_s( &fp, "devices_audiocapture.txt", "w" );

	// create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

	// enumerate video capture devices
	CComPtr< IEnumMoniker > pEm;
	pCreateDevEnum->CreateClassEnumerator( CLSID_AudioInputDeviceCategory, &pEm, 0);
	pCreateDevEnum.Release();

	if ( !pEm )
	{
		MessageBox( NULL, "Error: no audio capture devices found!", "Error", MB_ICONSTOP );
		exit(-1);
	}

	pEm->Reset();

	UINT i = 0;
	while (true)
	{
		i++;

		ULONG ulFetched = 0;

		CComPtr<IMoniker> pM;
		hr = pEm->Next(1, &pM, &ulFetched);
		if( hr != S_OK )
		{
			// no audio capture devices found
			break;
		}

		// ask for the actual filter
		hr = pM->BindToObject(0,0,IID_IBaseFilter, (void **) &ppCap);
		// if we found a device
		if (*ppCap)
		{
			// get the property bag interface from the moniker
			CComPtr< IPropertyBag > pBag;
			hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
			if( hr != S_OK )
			{
				continue;
			}

			// ask for the english-readable name
			CComVariant var;
			var.vt = VT_BSTR;
			pBag->Read( L"FriendlyName", &var, NULL );

			LPOLESTR szDeviceID;
			pM->GetDisplayName( 0, NULL, &szDeviceID );

			// device seems ok
			printf( "device #%d:\n", i );
			wprintf( L"%s\n", var.bstrVal ); // device name
			wprintf( L"%s\n", szDeviceID );

			//fprintf( fp, "device #%d:\n", i );
			//fwprintf( fp, L"%s\n", var.bstrVal ); // device name
			//fwprintf( fp, L"%s\n", szDeviceID );

			SAFE_RELEASE( ppCap );
		}
	}
	printf( "\n" );
	//fclose( fp );
}

BYTE* CDirectShowGraph::GetBuffer()
{
	return GrabberCallBack.GetBuffer();
}

const long& CDirectShowGraph::GetBufferSize()
{
	return GrabberCallBack.GetBufferSize();
}

const BOOL& CDirectShowGraph::IsBufferAvailable()
{
	return GrabberCallBack.IsBufferAvailable();
}

const UINT& CDirectShowGraph::GetFPSMeter()
{
	return GrabberCallBack.GetFPSMeter();
}

void CDirectShowGraph::ResetFPSMeter()
{
	GrabberCallBack.ResetFPSMeter();
}
