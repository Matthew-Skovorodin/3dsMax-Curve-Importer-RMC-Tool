//////////////////////////////////////////////////////////////////////////////
// RMCLink.cpp - CRMCLink Implementation
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005-2016 by Delta Computer Systems, Inc.
// All Rights Reserved
//////////////////////////////////////////////////////////////////////////////

// <<NOTE>> Borland C++ Builder users should replace "stdafx.h" with <windows.h>.
#include <Windows.h>
#include "RMCLink.h"
#include <comdef.h>


// Ensure 4-byte enums if we're using Borland C++ compilers
#ifdef __BORLANDC__
  #pragma option push -b
#endif


////////////////////////////////////////////////////
// Formal RMCLink.dll COM object types
//
namespace RMCLinkLib
{
	//
	// Enumerations
	//

	enum enumDeviceType
	{
		dtUnknown = 0,
		dtRMC100 = 1,
		dtRMC70 = 2,
		dtRMC150 = 3,
		dtRMC200 = 4
	};

	enum enumPingType
	{
		ptDoNotPing = 0,
		ptPing = 1
	};


	//
	// IRMCLink interface
	//

	struct __declspec(uuid("22def1f9-fe5d-4bfa-ab85-93b13d3ad5f3"))
	IRMCLink : IDispatch
	{
		virtual HRESULT __stdcall Connect() = 0;
		virtual HRESULT __stdcall Disconnect() = 0;
		virtual HRESULT __stdcall IsConnected(
						/*[in]*/ enumPingType Ping,
						/*[out,retval]*/ VARIANT_BOOL * retval) = 0;
		virtual HRESULT __stdcall ReadLFile(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Count,
						/*[out,retval]*/ VARIANT * retval ) = 0;
		virtual HRESULT __stdcall ReadLFile_Script(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Count,
						/*[out,retval]*/ VARIANT * retval ) = 0;
		virtual HRESULT __stdcall ReadFFile(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Count,
						/*[out,retval]*/ VARIANT * retval ) = 0;
		virtual HRESULT __stdcall ReadFFile_Script(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Count,
						/*[out,retval]*/ VARIANT * retval ) = 0;
		virtual HRESULT __stdcall ReadBit(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Bit,
						/*[out,retval]*/ VARIANT_BOOL * retval ) = 0;
		virtual HRESULT __stdcall ReadBitField(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long FieldStart,
						/*[in]*/ long FieldLen,
						/*[out,retval]*/ long * retval ) = 0;
		virtual HRESULT __stdcall WriteLFile(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Count,
						/*[in]*/ VARIANT Data ) = 0;
		virtual HRESULT __stdcall WriteFFile(
						/*[in]*/ long File,
						/*[in]*/ long Element,
						/*[in]*/ long Count,
						/*[in]*/ VARIANT Data ) = 0;
		virtual HRESULT __stdcall CancelRequest() = 0;
		virtual HRESULT __stdcall FRegToLReg(
						/*[in]*/ float regValue,
						/*[out,retval]*/ long* retval);
		virtual HRESULT __stdcall LRegToFReg(
						/*[in]*/ long regValue,
						/*[out,retval]*/ float* retval);
		virtual HRESULT __stdcall ReadImage(
						/*[in]*/ long flags,
						/*[out,retval]*/ VARIANT * retval) = 0;
		virtual HRESULT __stdcall ReadImageToFile(
						/*[in]*/ long flags,
						/*[in]*/ BSTR fileName) = 0;
		virtual HRESULT __stdcall WriteImage(
						/*[in]*/ long flags,
						/*[in]*/ VARIANT data) = 0;
		virtual HRESULT __stdcall WriteImageFromFile(
						/*[in]*/ long flags,
						/*[in]*/ BSTR fileName) = 0;
	};


	//
	// IRMCLinkServer interface
	//

	struct __declspec(uuid("335f4782-e4e6-4a19-93c0-3a992f9c145e"))
	IRMCLinkServer : IDispatch
	{
		virtual HRESULT __stdcall CreateEthernetLink(
						/*[in]*/ enumDeviceType DevType,
						/*[in]*/ BSTR HostName,
						/*[out,retval]*/ struct IRMCLink** RMCLink) = 0;
		virtual HRESULT __stdcall CreateSerialLink(
						/*[in]*/ enumDeviceType DevType,
						/*[in]*/ BSTR PortName,
						/*[out,retval]*/ struct IRMCLink** RMCLink) = 0;
		virtual HRESULT __stdcall CreateUSBLink(
						/*[in]*/ enumDeviceType DevType,
						/*[in]*/ BSTR DeviceId,
						/*[out,retval]*/ struct IRMCLink** RMCLink) = 0;
	};


	//
	// RMCLinkServer CoClass
	//

	// This structure forward reference is used only for its UUID to give
	//	us the CLSID of the RMCLink.RMCLinkServer object.
	struct __declspec(uuid("ca7473f7-2df3-45ce-a180-ca7cbf81f190"))
	RMCLinkServer;

} // namespace RMCLinkLib


// restore the borland-specific enum size option
#ifdef __BORLANDC__
  #pragma option pop
#endif



////////////////////////////////////////////////////
//		CRMCLink::CreateSerialLink
//
// Description:
//		Static function that creates a CRMCLink object
//		for communicating with an RMC of the requested type
//		over the requested serial port.
//
// Parameters:
//		eDevType	[i] Which type of RMC to communicate with:
//							CRMCLink::dtRMC70
//							CRMCLink::dtRMC100
//		pszPortName	[i] Name of the serial port (e.g. "COM1")
//						to use.
//		
// Return Value:
//		CRMCLink*	Pointer to a CRMCLink object. This object
//					must be freed by the caller when done.
//

CRMCLink* CRMCLink::CreateSerialLink(enumDEVICETYPE eDevType, LPCTSTR lpszPortName)
{
	return _CreateLink(LT_SER, eDevType, lpszPortName);
}



////////////////////////////////////////////////////
//		CRMCLink::CreateEthernetLink
//
// Description:
//		Static function that creates a CRMCLink object
//		for communicating with an RMC of the requested type
//		over TCP/IP using the requested host name.
//
// Parameters:
//		eDevType	[i] Which type of RMC to communicate with:
//							CRMCLink::dtRMC70
//							CRMCLink::dtRMC100
//							CRMCLink::dtRMC150
//							CRMCLink::dtRMC200
//		pszHostName	[i] Host name of the device. Notice that this
//						may be a IP address ("192.168.0.3") or a
//						actual host name ("rmc.deltamotion.com").
//		
// Return Value:
//		CRMCLink*	Pointer to a CRMCLink object. This object
//					must be freed by the caller when done.
//

CRMCLink* CRMCLink::CreateEthernetLink(enumDEVICETYPE eDevType, LPCTSTR lpszHostName)
{
	return _CreateLink(LT_ENET, eDevType, lpszHostName);
}



////////////////////////////////////////////////////
//		CRMCLink::CreateUSBLink
//
// Description:
//		Static function that creates a CRMCLink object
//		for communicating with an RMC of the requested type
//		over a USB port using the requested device ID.
//
// Parameters:
//		eDevType	[i] Which type of RMC to communicate with:
//							CRMCLink::dtRMC70
//							CRMCLink::dtRMC100
//							CRMCLink::dtRMC150
//							CRMCLink::dtRMC200
//		pszDeviceId	[i] Device ID of the device.
//		
// Return Value:
//		CRMCLink*	Pointer to a CRMCLink object. This object
//					must be freed by the caller when done.
//

CRMCLink* CRMCLink::CreateUSBLink(enumDEVICETYPE eDevType, LPCTSTR lpszDeviceId)
{
	return _CreateLink(LT_USB, eDevType, lpszDeviceId);
}



////////////////////////////////////////////////////
//		CRMCLink::_CreateLink
//
// Description:
//		Worker function called by CreateSerialLink and
//		CreateEthernetLink.  The parameters and return type
//		are described by the public callers.
//

CRMCLink* CRMCLink::_CreateLink(enumLINKTYPE eLinkType, enumDEVICETYPE eDevType, LPCTSTR lpszAddress)
{
	CRMCLink* pRMCLink = NULL;

	// These variables are used in the CoCreateInstance call below.
	IID const& clsid = __uuidof(RMCLinkLib::RMCLinkServer);
	IID const& riid = __uuidof(RMCLinkLib::IRMCLinkServer);
	RMCLinkLib::IRMCLinkServer* pRMCLinkServer = NULL;

	// Create a COM object called 'RMCLink.RMCLinkServer', from which we'll
	//	request our RMCLink object.
	HRESULT hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, riid, (LPVOID*)&pRMCLinkServer);
	if ( SUCCEEDED(hr) )
	{
		// Convert from our internal enumDEVICETYPE to the actual
		//	RMCLinkLib::enumDeviceType enumeration.
		RMCLinkLib::enumDeviceType dt = RMCLinkLib::dtUnknown;
		if ( eDevType == dtRMC100 )
			dt = RMCLinkLib::dtRMC100;
		else if ( eDevType == dtRMC70 )
			dt = RMCLinkLib::dtRMC70;
		else if ( eDevType == dtRMC150 )
			dt = RMCLinkLib::dtRMC150;
		else if ( eDevType == dtRMC200 )
			dt = RMCLinkLib::dtRMC200;

		// Convert the address string into a BSTR.
#ifdef _UNICODE
		BSTR bstrAddress = ::SysAllocString(lpszAddress);
#else
		size_t tLen = strlen(lpszAddress);
		int nAllocLen = ::MultiByteToWideChar(CP_ACP, 0, lpszAddress, (int)tLen, NULL, NULL);
		BSTR bstrAddress = ::SysAllocStringLen(NULL, nAllocLen);
		if ( bstrAddress != NULL )
		{
			if ( ::MultiByteToWideChar(CP_ACP, 0, lpszAddress, (int)tLen, bstrAddress, nAllocLen) != nAllocLen )
			{
				::SysFreeString(bstrAddress);
				bstrAddress = NULL;
			}
		}

		if ( bstrAddress == NULL )
			hr = E_FAIL;
#endif

		if ( SUCCEEDED(hr) )
		{
			// Request a RMCLink object of the appropriate type (Ethernet or Serial).
			RMCLinkLib::IRMCLink* pIRMCLink = NULL;
			if ( eLinkType == LT_ENET )
				hr = pRMCLinkServer->CreateEthernetLink(dt, bstrAddress, &pIRMCLink);
			else if ( eLinkType == LT_SER )
				hr = pRMCLinkServer->CreateSerialLink(dt, bstrAddress, &pIRMCLink);
			else
				hr = pRMCLinkServer->CreateUSBLink(dt, bstrAddress, &pIRMCLink);

			if ( SUCCEEDED(hr) )
			{
				// Create a C++ CRMCLink class wrapping the object that we
				//	got from the IRMCLinkServer::Create*Link call above.
				pRMCLink = new CRMCLink(pIRMCLink);

				// We drop our reference to this object; CRMCLink can now manage
				//	its own reference to that object.
				pIRMCLink->Release();
			}
		}

		// Free the BSTR we created above.
		if ( bstrAddress != NULL )
			::SysFreeString(bstrAddress);

		// We're now done with the server.
		pRMCLinkServer->Release();
	}
	else
	{
		// <<NOTE>> These two errors are so common, yet they only occur due
		//	to setup errors, either in the program or with registering the
		//	COM object, that I just display a message box so the programmer
		//	corrects the problem.
		if ( hr == CO_E_NOTINITIALIZED )
		{
			::MessageBoxA(NULL,
				"The Component Object Model (COM) has not been initialized for use by this thread.  Since the CRMCLink class requires COM objects in RMCLINK.DLL, make sure that you call CoInitializeEx(0, COINIT_MULTITHREADED) or equivalent before using CRMCLink.",
				"RMCLink COM Object", MB_OK | MB_ICONEXCLAMATION);
		}
		else if ( hr == REGDB_E_CLASSNOTREG )
		{
			::MessageBoxA(NULL,
				"The RMCLink.dll COM server has not been registered. Run \"regsvr32 rmclink.dll\" from the command line to register the COM object.",
				"RMCLink COM Object", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	return pRMCLink;
}



////////////////////////////////////////////////////
//		CRMCLink::CRMCLink
//
// Description:
//		This private constructor is called by the two static
//		functions: CreateSerialLink and CreateEthernetLink.
//		It simply hooks up the reference to the real RMCLink
//		COM object.
//

CRMCLink::CRMCLink(RMCLinkLib::IRMCLink* pIRMCLink)
	:	m_pIRMCLink(pIRMCLink)
{
	m_pIRMCLink->AddRef();
}



////////////////////////////////////////////////////
//		CRMCLink::~CRMCLink
//
// Description:
//		The destructor releases our reference to the
//		actual RMCLink COM object.
//

CRMCLink::~CRMCLink()
{
	// Release our interest in this COM object.
	if ( m_pIRMCLink != NULL )
	{
		m_pIRMCLink->Release();
		m_pIRMCLink = NULL;
	}
}



////////////////////////////////////////////////////
//		CRMCLink::Connect
//
// Description:
//		Attempts to connect to the controller as configured.
//
// Return Value:
//		HRESULT
//			S_OK - Connection was successful!
//

HRESULT CRMCLink::Connect()
{
	return m_pIRMCLink->Connect();
}



////////////////////////////////////////////////////
//		CRMCLink::Disconnect
//
// Description:
//		Disconnects from the controller.
//

void CRMCLink::Disconnect()
{
	// <<NOTE>> We ignore the return code.
	m_pIRMCLink->Disconnect();
}



////////////////////////////////////////////////////
//		CRMCLink::IsConnected
//
// Description:
//		Checks to see if this RMCLink is currently
//		connected, optionally performing a ping to
//		check if the connection is still active.
//
// Parameters:
//		bPing	[i] If true, then if the connection
//					appears to be currently established,
//					then the RMC will be "pinged" to
//					determine if it is still connected.
//					If the "ping" fails, then the connection
//					will be automatically disconnected.
//
//					If false, only the current state
//					will be evaluated.
//
// Return Value:
//		true	The controller is still connected.
//		false	The controller is no longer connected.
//

bool CRMCLink::IsConnected(bool bPing)
{
	RMCLinkLib::enumPingType pt =
		bPing ? RMCLinkLib::ptPing : RMCLinkLib::ptDoNotPing;

	VARIANT_BOOL vb;
	HRESULT hr = m_pIRMCLink->IsConnected(pt, &vb);
	if ( FAILED(hr) )
		return false;

	return ( vb != VARIANT_FALSE );
}



////////////////////////////////////////////////////
//		CRMCLink::ReadLFile
//
// Description:
//		Reads one or more consecutive REALs from the controller
//		starting at the address L<f>:<e>.
//
// Parameters:
//		uFile	[i] File number <f> from address.
//		uElem	[i] Element number <e> from address.
//		uCount	[i] Count of LONG registers to read.
//		palData	[o] Pointer to pre-allocated array of uCount LONGs.
//
// Return Value:
//		HRESULT
//			S_OK - Read was successful.
//

HRESULT CRMCLink::ReadLFile(UINT uFile, UINT uElem, UINT uCount, long* palData)
{
	VARIANT	var;
	::VariantInit(&var);

	HRESULT hr = m_pIRMCLink->ReadLFile((long)uFile, (long)uElem, (long)uCount, &var);
	if ( SUCCEEDED(hr) )
	{
		SAFEARRAY* psa = var.parray;

		if (( var.vt == (VT_ARRAY | VT_I4) )
		&&	( psa->cbElements == sizeof(long) )
		&&	( psa->cDims == 1 )
		&&	( psa->rgsabound[0].cElements >= uCount ))
		{
			memcpy(palData, psa->pvData, sizeof(long)*uCount);
		}
		else
			hr = E_FAIL;
	}

	::VariantClear(&var);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::ReadFFile
//
// Description:
//		Reads one or more consecutive REALs from the controller
//		starting at the address L<f>:<e>.
//
// Parameters:
//		uFile	[i] File number <f> from address.
//		uElem	[i] Element number <e> from address.
//		uCount	[i] Count of FLOAT registers to read.
//		palData	[o] Pointer to pre-allocated array of uCount FLOATs.
//
// Return Value:
//		HRESULT
//			S_OK - Read was successful.
//

HRESULT CRMCLink::ReadFFile(UINT uFile, UINT uElem, UINT uCount, float* pafData)
{
	VARIANT	var;
	::VariantInit(&var);

	HRESULT hr = m_pIRMCLink->ReadFFile((long)uFile, (long)uElem, (long)uCount, &var);
	if ( SUCCEEDED(hr) )
	{
		SAFEARRAY* psa = var.parray;

		if (( var.vt == (VT_ARRAY | VT_R4) )
		&&	( psa->cbElements == sizeof(float) )
		&&	( psa->cDims == 1 )
		&&	( psa->rgsabound[0].cElements >= uCount ))
		{
			memcpy(pafData, psa->pvData, sizeof(float)*uCount);
		}
		else
			hr = E_FAIL;
	}

	::VariantClear(&var);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::ReadBit
//
// Description:
//		Reads a single bit from an L-file register.  In
//		Allen-Bradley format, the address is L<f>:<e>/<b>.
//
// Parameters:
//		uFile	[i] File number <f> from address.
//		uElem	[i] Element number <e> from address.
//		uBit	[i] Bit number <b> (0-31) to read.
//		pbValue	[o] Value of bit (true/false).
//
// Return Value:
//		HRESULT
//			S_OK - Read was successful.
//

HRESULT CRMCLink::ReadBit(UINT uFile, UINT uElem, UINT uBit, bool& bValue)
{
	VARIANT_BOOL vb;
	HRESULT hr = m_pIRMCLink->ReadBit((long)uFile, (long)uElem, (long)uBit, &vb);
	if ( SUCCEEDED(hr) )
		bValue = ( vb != VARIANT_FALSE );

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::ReadBitField
//
// Description:
//		Reads a single- or multi-bit field from an L-file.
//		In Allen-Bradley format, the bit fields runs from
//		an LSB of L<f>:<e>/<b> to L<f>:<e>/(<b>+<l>).
//
// Parameters:
//		uFile		[i] File number <f> from address.
//		uElem		[i] Element number <e> from address.
//		uFieldStart	[i] Bit number <b> (0-31) of field's LSB.
//		uFieldLen	[i] Number of bits <l> in field.
//		puValue		[o] Value of bit field.
//
// Return Value:
//		HRESULT
//			S_OK - Read was successful.
//

HRESULT CRMCLink::ReadBitField(UINT uFile, UINT uElem, UINT uFieldStart, UINT uFieldLen, UINT& uValue)
{
	long	lVal;
	HRESULT hr = m_pIRMCLink->ReadBitField((long)uFile, (long)uElem, (long)uFieldStart, (long)uFieldLen, &lVal);
	if ( SUCCEEDED(hr) )
		uValue = (UINT)lVal;

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::WriteLFile
//
// Description:
//		Writes to one or more consecutive LONGs starting
//		at the address L<f>:<e>.
//
// Parameters:
//		uFile	[i] File number <f> from address.
//		uElem	[i] Element number <e> from address.
//		uCount	[i] Count of LONG registers to write.
//		pafData	[i] Pointer to array of uCount LONGs to write.
//
// Return Value:
//		HRESULT
//			S_OK - Write was successful.
//

HRESULT CRMCLink::WriteLFile(UINT uFile, UINT uElem, UINT uCount, long const* palData)
{
	SAFEARRAYBOUND bounds;
	bounds.lLbound = 0;
	bounds.cElements = uCount;

	SAFEARRAY* psa = ::SafeArrayCreate(VT_I4, 1, &bounds);
	memcpy(psa->pvData, palData, sizeof(long)*uCount);

	VARIANT var;
	var.vt = VT_I4 | VT_ARRAY;
	var.parray = psa;

	HRESULT hr = m_pIRMCLink->WriteLFile((long)uFile, (long)uElem, (long)uCount, var);

	::VariantClear(&var);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::WriteFFile
//
// Description:
//		Writes to one or more consecutive REALs starting
//		at the address L<f>:<e>.
//
// Parameters:
//		uFile	[i] File number <f> from address.
//		uElem	[i] Element number <e> from address.
//		uCount	[i] Count of FLOAT registers to write.
//		pafData	[i] Pointer to array of uCount FLOATs to write.
//
// Return Value:
//		HRESULT
//			S_OK - Write was successful.
//

HRESULT CRMCLink::WriteFFile(UINT uFile, UINT uElem, UINT uCount, float const* pafData)
{
	SAFEARRAYBOUND bounds;
	bounds.lLbound = 0;
	bounds.cElements = uCount;

	SAFEARRAY* psa = ::SafeArrayCreate(VT_R4, 1, &bounds);
	memcpy(psa->pvData, pafData, sizeof(float)*uCount);

	VARIANT var;
	var.vt = VT_R4 | VT_ARRAY;
	var.parray = psa;

	HRESULT hr = m_pIRMCLink->WriteFFile((long)uFile, (long)uElem, (long)uCount, var);

	::VariantClear(&var);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::CancelRequest
//
// Description:
//		This method signals to the RMCLink COM object to cancel
//		the current in-progress request.
//

void CRMCLink::CancelRequest()
{
	// <<NOTE>> We ignore the return code.
	m_pIRMCLink->CancelRequest();
}



////////////////////////////////////////////////////
//		CRMCLink::ReadImage
//
// Description:
//		This method signals to the RMCLink COM object to cancel
//		the current in-progress request.
//

HRESULT CRMCLink::ReadImage(UINT uFlags, long*& palData, UINT& uCount)
{
	VARIANT	var;
	::VariantInit(&var);

	HRESULT hr = m_pIRMCLink->ReadImage((long)uFlags, &var);
	if ( SUCCEEDED(hr) )
	{
		SAFEARRAY* psa = var.parray;

		if (( var.vt == (VT_ARRAY | VT_I4) )
		&&	( psa->cbElements == sizeof(long) )
		&&	( psa->cDims == 1 )
		&&	( psa->rgsabound[0].cElements >= 0 ))
		{
			uCount = (UINT)psa->rgsabound[0].cElements;
			palData = new long[psa->rgsabound[0].cElements];
			memcpy(palData, psa->pvData, sizeof(long)*uCount);
		}
		else
			hr = E_FAIL;
	}

	::VariantClear(&var);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::WriteImage
//
// Description:
//		This method signals to the RMCLink COM object to cancel
//		the current in-progress request.
//

HRESULT CRMCLink::WriteImage(UINT uFlags, long const* palData, UINT uCount)
{
	SAFEARRAYBOUND bounds;
	bounds.lLbound = 0;
	bounds.cElements = uCount;

	SAFEARRAY* psa = ::SafeArrayCreate(VT_I4, 1, &bounds);
	memcpy(psa->pvData, palData, sizeof(long)*uCount);

	VARIANT var;
	var.vt = VT_I4 | VT_ARRAY;
	var.parray = psa;

	HRESULT hr = m_pIRMCLink->WriteImage((long)uFlags, var);

	::VariantClear(&var);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::ReadImageToFile
//
// Description:
//		This method signals to the RMCLink COM object to cancel
//		the current in-progress request.
//

HRESULT CRMCLink::ReadImageToFile(UINT uFlags, LPCTSTR lpszFileName)
{
	HRESULT hr = S_OK;

	// Convert the filename string into a BSTR.
#ifdef _UNICODE
	BSTR bstrFileName = ::SysAllocString(lpszFileName);
#else
	size_t tLen = strlen(lpszFileName);
	int nAllocLen = ::MultiByteToWideChar(CP_ACP, 0, lpszFileName, (int)tLen, NULL, NULL);
	BSTR bstrFileName = ::SysAllocStringLen(NULL, nAllocLen);
	if ( bstrFileName != NULL )
	{
		if ( ::MultiByteToWideChar(CP_ACP, 0, lpszFileName, (int)tLen, bstrFileName, nAllocLen) != nAllocLen )
		{
			::SysFreeString(bstrFileName);
			bstrFileName = NULL;
		}
	}

	if ( bstrFileName == NULL )
		hr = E_FAIL;
#endif

	if ( SUCCEEDED(hr) )
		hr = m_pIRMCLink->ReadImageToFile((long)uFlags, bstrFileName);

	// Free the BSTR we created above.
	if ( bstrFileName != NULL )
		::SysFreeString(bstrFileName);

	return hr;
}



////////////////////////////////////////////////////
//		CRMCLink::WriteImageFromFile
//
// Description:
//		This method signals to the RMCLink COM object to cancel
//		the current in-progress request.
//

HRESULT CRMCLink::WriteImageFromFile(UINT uFlags, LPCTSTR lpszFileName)
{
	HRESULT hr = S_OK;

	// Convert the filename string into a BSTR.
#ifdef _UNICODE
	BSTR bstrFileName = ::SysAllocString(lpszFileName);
#else
	size_t tLen = strlen(lpszFileName);
	int nAllocLen = ::MultiByteToWideChar(CP_ACP, 0, lpszFileName, (int)tLen, NULL, NULL);
	BSTR bstrFileName = ::SysAllocStringLen(NULL, nAllocLen);
	if ( bstrFileName != NULL )
	{
		if ( ::MultiByteToWideChar(CP_ACP, 0, lpszFileName, (int)tLen, bstrFileName, nAllocLen) != nAllocLen )
		{
			::SysFreeString(bstrFileName);
			bstrFileName = NULL;
		}
	}

	if ( bstrFileName == NULL )
		hr = E_FAIL;
#endif

	if ( SUCCEEDED(hr) )
		hr = m_pIRMCLink->WriteImageFromFile((long)uFlags, bstrFileName);

	// Free the BSTR we created above.
	if ( bstrFileName != NULL )
		::SysFreeString(bstrFileName);

	return hr;
}
