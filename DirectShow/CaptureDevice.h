#ifndef __CAPTUREDEVICE_H
#define __CAPTUREDEVICE_H

// this class represents a directshow capture device
//
class CCaptureDevice
{
public:
	CCaptureDevice();
	~CCaptureDevice();

	// init capture device by given COM object ID
	//
	CCaptureDevice( LPWSTR szID );

	// retrieves device's COM object ID
	//
	CString GetID();

	// device name
	//
	CString GetName();

	CComPtr< IBaseFilter > GetBaseFilter();
	CComPtr< IPin > GetOutPin();



	void SetOutPin( CComPtr< IPin > Pin );
	void SetBaseFilter( CComPtr< IBaseFilter > Filter );
	void SetDeviceName( CString szNewDeviceName );
	void SetDeviceID( CString szNewID );

private:
	CComPtr< IBaseFilter > m_pBaseFilter;
	CComPtr< IPin > m_pOutPin;

	CString m_szDeviceName;
	CString m_szID;
};

#endif
