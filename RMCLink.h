//////////////////////////////////////////////////////////////////////////////
// RMCLink.h - CRMCLink declaration
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005-2016 by Delta Computer Systems, Inc.
// All Rights Reserved
//////////////////////////////////////////////////////////////////////////////

#pragma once

#define RMCLINK_CTL_HRESULT(n) MAKE_HRESULT(SEVERITY_ERROR, FACILITY_CONTROL, (n))

// Forward reference to interface defined in RMCLinkLib.h
namespace RMCLinkLib
{
	struct IRMCLink;
};

class CRMCLink
{
public:
	enum enumDEVICETYPE
	{
		dtRMC100 = 1,
		dtRMC70 = 2,
		dtRMC150 = 3,
		dtRMC200 = 4
	};


	/////////////////////////////////////////////
	// CRMCLink static methods
	/////////////////////////////////////////////
	//
	static CRMCLink* CreateSerialLink(enumDEVICETYPE eDevice, LPCTSTR lpszPortName);
	static CRMCLink* CreateEthernetLink(enumDEVICETYPE eDevice, LPCTSTR lpszHostName);
	static CRMCLink* CreateUSBLink(enumDEVICETYPE eDevice, LPCTSTR lpszDeviceId);

	static long		MakeRMCAddr(UINT uFile, UINT uElem)
		{ return (uFile << 12) | uElem; }

	static long		FRegToLReg(float fValue)
		{ return *(long*)&fValue; }

	static float	LRegToFReg(long lValue)
		{ return *(float*)&lValue; }
	//
	/////////////////////////////////////////////


	/////////////////////////////////////////////
	// CRMCLink instance methods
	/////////////////////////////////////////////
	//
	HRESULT	Connect();
	void	Disconnect();
	bool	IsConnected(bool bPing);
	HRESULT	ReadBit(UINT uFile, UINT uElem, UINT uBit, bool& bValue);
	HRESULT	ReadBitField(UINT uFile, UINT uElem, UINT uFieldStart, UINT uFieldLen, UINT& uValue);
	HRESULT	ReadLFile(UINT uFile, UINT uElem, UINT uCount, long* palData);
	HRESULT	ReadFFile(UINT uFile, UINT uElem, UINT uCount, float* pafData);
	HRESULT	WriteLFile(UINT uFile, UINT uElem, UINT uCount, long const* palData);
	HRESULT	WriteFFile(UINT uFile, UINT uElem, UINT uCount, float const* pafData);
	HRESULT ReadImage(UINT uFlags, long*& palData, UINT& uCount);
	HRESULT WriteImage(UINT uFlags, long const* palData, UINT uCount);
	HRESULT ReadImageToFile(UINT uFlags, LPCTSTR lpszFileName);
	HRESULT WriteImageFromFile(UINT uFlags, LPCTSTR lpszFileName);
	void	CancelRequest();
	//
	/////////////////////////////////////////////


	// RMC100 File Number enumeration - for convenience only
	enum enumFILENUMBER100
	{
		fn100StatusAxis0		= 0,
		fn100StatusAxis1,
		fn100StatusAxis2,
		fn100StatusAxis3,
		fn100StatusAxis4,
		fn100StatusAxis5,
		fn100StatusAxis6,
		fn100StatusAxis7,

		fn100CommandAxis0		= 8,
		fn100CommandAxis1,
		fn100CommandAxis2,
		fn100CommandAxis3,
		fn100CommandAxis4,
		fn100CommandAxis5,
		fn100CommandAxis6,
		fn100CommandAxis7,

		fn100ParameterAxis0		= 16,
		fn100ParameterAxis1,
		fn100ParameterAxis2,
		fn100ParameterAxis3,
		fn100ParameterAxis4,
		fn100ParameterAxis5,
		fn100ParameterAxis6,
		fn100ParameterAxis7,

		fn100StepTable			= 24,
		fn100I2ERisingEdges		= 25,
		fn100I2EFallingEdges	= 26,
		fn100StatusMap			= 27,
		fn100ProfileTable		= 28,
		fn100PlotType			= 29,
		fn100DIORegs			= 30,
		fn100PlotTime			= 31,
		fn100ParamError			= 32,
		fn100FWRev				= 33,
		fn100SplineDA			= 34,

		fn100PlotAxis0Set0		= 208,
		fn100PlotAxis0Set1,
		fn100PlotAxis0Set2,
		fn100PlotAxis0Set3,
		fn100PlotAxis0Set4,
		fn100PlotAxis0Set5,
		fn100PlotAxis1Set0		= 214,
		fn100PlotAxis1Set1,
		fn100PlotAxis1Set2,
		fn100PlotAxis1Set3,
		fn100PlotAxis1Set4,
		fn100PlotAxis1Set5,
		fn100PlotAxis2Set0		= 220,
		fn100PlotAxis2Set1,
		fn100PlotAxis2Set2,
		fn100PlotAxis2Set3,
		fn100PlotAxis2Set4,
		fn100PlotAxis2Set5,
		fn100PlotAxis3Set0		= 226,
		fn100PlotAxis3Set1,
		fn100PlotAxis3Set2,
		fn100PlotAxis3Set3,
		fn100PlotAxis3Set4,
		fn100PlotAxis3Set5,
		fn100PlotAxis4Set0		= 232,
		fn100PlotAxis4Set1,
		fn100PlotAxis4Set2,
		fn100PlotAxis4Set3,
		fn100PlotAxis4Set4,
		fn100PlotAxis4Set5,
		fn100PlotAxis5Set0		= 238,
		fn100PlotAxis5Set1,
		fn100PlotAxis5Set2,
		fn100PlotAxis5Set3,
		fn100PlotAxis5Set4,
		fn100PlotAxis5Set5,
		fn100PlotAxis6Set0		= 244,
		fn100PlotAxis6Set1,
		fn100PlotAxis6Set2,
		fn100PlotAxis6Set3,
		fn100PlotAxis6Set4,
		fn100PlotAxis6Set5,
		fn100PlotAxis7Set0		= 250,
		fn100PlotAxis7Set1,
		fn100PlotAxis7Set2,
		fn100PlotAxis7Set3,
		fn100PlotAxis7Set4,
		fn100PlotAxis7Set5
	};

	// RMC100 Miscellaneous Constants - for convenience only
	enum enumCONSTANTS100
	{
		rmc100StsRegsPerAxis	= 10,
		rmc100CmdRegsPerAxis	= 6,
		rmc100ParamRegsPerAxis	= 16,
		rmc100RegsPerStep		= 8,
		rmc100RegsPerI2EInput	= 8,
		rmc100RegsPerProfile	= 4,
		rmc100RegsPerPlotFile	= 1024,

		rmc100MaxAxes			= 8,

		rmc100PlotFilesPerAxis	= 6,

		rmc100PTPrecision		= 0,
		rmc100PTCommands		= 1,
		rmc100PTEvents			= 2,
		rmc100PTCounts			= 3
	};

	// RMC70 File Number enumeration - for convenience only
	enum enumFILENUMBER70
	{
		fn70ControllerInfo	= 7,
		fn70StatusAxis0		= 8,
		fn70StatusAxis1,
		fn70StatusAxis2,
		fn70StatusAxis3,
		fn70ParameterAxis0	= 12,
		fn70ParameterAxis1,
		fn70ParameterAxis2,
		fn70ParameterAxis3,

		fn70CommandArea		= 16,
		fn70IndDataMap		= 17,
		fn70IndDataValues	= 18,
		fn70AxisAssign		= 19,
		fn70ControllerRegs	= 20,
		fn70CommConfig		= 21,
		fn70DiscreteIO		= 23,
		fn70TaskStatus		= 24,
		fn70LrgCommandArea	= 25,
		fn70AnalogInputs	= 26,
		fn70AxisNames		= 27,

		fn70ImageArea		= 30,

		fn70PlotSchema		= 31,

		fn70Plot0CfgSts		= 32,
		fn70Plot1CfgSts,
		fn70Plot2CfgSts,
		fn70Plot3CfgSts,
		fn70Plot4CfgSts,
		fn70Plot5CfgSts,
		fn70Plot6CfgSts,
		fn70Plot7CfgSts,

		fn70Plot0DynUA		= 40,
		fn70Plot1DynUA,
		fn70Plot2DynUA,
		fn70Plot3DynUA,
		fn70Plot4DynUA,
		fn70Plot5DynUA,
		fn70Plot6DynUA,
		fn70Plot7DynUA,

		fn70Plot0StaticUA	= 48,
		fn70Plot1StaticUA,
		fn70Plot2StaticUA,
		fn70Plot3StaticUA,
		fn70Plot4StaticUA,
		fn70Plot5StaticUA,
		fn70Plot6StaticUA,
		fn70Plot7StaticUA,

		fn70VarCurValues	= 56,

		fn70VarInitValues	= 64,

		fn70VarAttributes	= 68,
	};

	// RMC70 Miscellaneous Constants - for convenience only
	enum enumCONSTANTS70
    {
		rmc70CmdRegsPerAxis		= 6,
		rmc70LrgCmdRegsPerAxis	= 10
    };

	// RMC150 File Number enumeration - for convenience only
	enum enumFILENUMBER150
	{
		fn150ControllerInfo		= 7,

		fn150StatusAxis0		= 8,
		fn150StatusAxis1,
		fn150StatusAxis2,
		fn150StatusAxis3,
		fn150StatusAxis4,
		fn150StatusAxis5,
		fn150StatusAxis6,
		fn150StatusAxis7,
		fn150StatusAxis8,
		fn150StatusAxis9,
		fn150StatusAxis10,
		fn150StatusAxis11,
		fn150StatusAxis12,
		fn150StatusAxis13,
		fn150StatusAxis14,
		fn150StatusAxis15,

		fn150ParameterAxis0		= 24,
		fn150ParameterAxis1,
		fn150ParameterAxis2,
		fn150ParameterAxis3,
		fn150ParameterAxis4,
		fn150ParameterAxis5,
		fn150ParameterAxis6,
		fn150ParameterAxis7,
		fn150ParameterAxis8,
		fn150ParameterAxis9,
		fn150ParameterAxis10,
		fn150ParameterAxis11,
		fn150ParameterAxis12,
		fn150ParameterAxis13,
		fn150ParameterAxis14,
		fn150ParameterAxis15,

		fn150CommandArea		= 40,
		fn150IndDataMap			= 41,
		fn150IndDataValues		= 42,
		fn150AxisAssign			= 43,
		fn150ControllerRegs		= 44,
		fn150CommConfig			= 45,

		fn150DiscreteIO			= 47,
		fn150TaskStatus			= 48,
		fn150AxisNames			= 49,

		fn150VarCurValues		= 56,
		fn150VarInitValues		= 72,
		fn150VarAttributes		= 88,

		fn150ImageArea			= 94,

		fn150PlotSchema			= 95,

		fn150Plot0CfgSts		= 96,
		fn150Plot1CfgSts		= 97,
		fn150Plot2CfgSts		= 98,
		fn150Plot3CfgSts		= 99,
		fn150Plot4CfgSts		= 100,
		fn150Plot5CfgSts		= 101,
		fn150Plot6CfgSts		= 102,
		fn150Plot7CfgSts		= 103,

		fn150Plot0DynUA			= 104,
		fn150Plot1DynUA			= 105,
		fn150Plot2DynUA			= 106,
		fn150Plot3DynUA			= 107,
		fn150Plot4DynUA			= 108,
		fn150Plot5DynUA			= 109,
		fn150Plot6DynUA			= 110,
		fn150Plot7DynUA			= 111,

		fn150Plot0StaticUA0		= 112,
		fn150Plot0StaticUA1		= 113,
		fn150Plot0StaticUA2		= 114,
		fn150Plot0StaticUA3		= 115,
		fn150Plot1StaticUA0		= 116,
		fn150Plot1StaticUA1		= 117,
		fn150Plot1StaticUA2		= 118,
		fn150Plot1StaticUA3		= 119,
		fn150Plot2StaticUA0		= 120,
		fn150Plot2StaticUA1		= 121,
		fn150Plot2StaticUA2		= 122,
		fn150Plot2StaticUA3		= 123,
		fn150Plot3StaticUA0		= 124,
		fn150Plot3StaticUA1		= 125,
		fn150Plot3StaticUA2		= 126,
		fn150Plot3StaticUA3		= 127,
		fn150Plot4StaticUA0		= 128,
		fn150Plot4StaticUA1		= 129,
		fn150Plot4StaticUA2		= 130,
		fn150Plot4StaticUA3		= 131,
		fn150Plot5StaticUA0		= 132,
		fn150Plot5StaticUA1		= 133,
		fn150Plot5StaticUA2		= 134,
		fn150Plot5StaticUA3		= 135,
		fn150Plot6StaticUA0		= 136,
		fn150Plot6StaticUA1		= 137,
		fn150Plot6StaticUA2		= 138,
		fn150Plot6StaticUA3		= 139,
		fn150Plot7StaticUA0		= 140,
		fn150Plot7StaticUA1		= 141,
		fn150Plot7StaticUA2		= 142,
		fn150Plot7StaticUA3		= 143,

		fn150Slot0Settings		= 144,
		fn150Slot1Settings		= 145,
		fn150Slot2Settings		= 146,
		fn150Slot3Settings		= 147,
		fn150Slot4Settings		= 148,
		fn150Slot5Settings		= 149
	};

	// RMC150 Miscellaneous Constants - for convenience only
	enum enumCONSTANTS150
	{
		rmc150CmdRegsPerAxis = 10
	};
	
	// RMC200 File Number enumeration - for convenience only
	enum enumFILENUMBER200
	{
		fn200ControllerInfo		= 7,
		fn200IndDataValues		= 8,
		fn200IndDataMap			= 12,
		fn200CommandArea		= 16,
		fn200ControllerRegs		= 18,
		fn200DiscreteIO			= 20,
		fn200TaskManager		= 21,
        fn200PlotManager		= 22,
		fn200ImageArea			= 23,
		fn200AxisAssignActive	= 24,
		fn200AxisAssignNew		= 25,
		fn200ModuleDefs			= 26,

		fn200CommConfig			= 112,
		fn200AddressMapDF1		= 113,
		fn200AddressMapMB		= 114,
		fn200AddressMapFINS		= 115,
		fn200AddressMapPN		= 116,

        fn200ModuleBase			= 128,
        fn200ModuleSlot0PS		= 129,
        fn200ModuleSlot1CPU		= 130,
        fn200ModuleSlot2		= 131,
        fn200ModuleSlot3		= 132,
        fn200ModuleSlot4		= 133,
        fn200ModuleSlot5		= 134,
        fn200ModuleSlot6		= 135,
        fn200ModuleSlot7		= 136,
        fn200ModuleSlot8		= 137,
        fn200ModuleSlot9		= 138,
        fn200ModuleSlot10		= 139,
        fn200ModuleSlot11		= 140,
        fn200ModuleSlot12		= 141,
        fn200ModuleSlot13		= 142,
        fn200ModuleSlot14		= 143,

		fn200TaskStatus0		= 192,
		fn200TaskStatus1,
		fn200TaskStatus2,
		fn200TaskStatus3,
		fn200TaskStatus4,
		fn200TaskStatus5,
		fn200TaskStatus6,
		fn200TaskStatus7,
		fn200TaskStatus8,
		fn200TaskStatus9,
		fn200TaskStatus10,
		fn200TaskStatus11,
		fn200TaskStatus12,
		fn200TaskStatus13,
		fn200TaskStatus14,
		fn200TaskStatus15,
		fn200TaskStatus16,
		fn200TaskStatus17,
		fn200TaskStatus18,
		fn200TaskStatus19,
		fn200TaskStatus20,
		fn200TaskStatus21,
		fn200TaskStatus22,
		fn200TaskStatus23,
		fn200TaskStatus24,
		fn200TaskStatus25,
		fn200TaskStatus26,
		fn200TaskStatus27,
		fn200TaskStatus28,
		fn200TaskStatus29,
		fn200TaskStatus30,
		fn200TaskStatus31,

		fn200StatusAxis0		= 256,
		fn200StatusAxis1,
		fn200StatusAxis2,
		fn200StatusAxis3,
		fn200StatusAxis4,
		fn200StatusAxis5,
		fn200StatusAxis6,
		fn200StatusAxis7,
		fn200StatusAxis8,
		fn200StatusAxis9,
		fn200StatusAxis10,
		fn200StatusAxis11,
		fn200StatusAxis12,
		fn200StatusAxis13,
		fn200StatusAxis14,
		fn200StatusAxis15,
		fn200StatusAxis16,
		fn200StatusAxis17,
		fn200StatusAxis18,
		fn200StatusAxis19,
		fn200StatusAxis20,
		fn200StatusAxis21,
		fn200StatusAxis22,
		fn200StatusAxis23,
		fn200StatusAxis24,
		fn200StatusAxis25,
		fn200StatusAxis26,
		fn200StatusAxis27,
		fn200StatusAxis28,
		fn200StatusAxis29,
		fn200StatusAxis30,
		fn200StatusAxis31,

		fn200ParameterAxis0		= 384,
		fn200ParameterAxis1,
		fn200ParameterAxis2,
		fn200ParameterAxis3,
		fn200ParameterAxis4,
		fn200ParameterAxis5,
		fn200ParameterAxis6,
		fn200ParameterAxis7,
		fn200ParameterAxis8,
		fn200ParameterAxis9,
		fn200ParameterAxis10,
		fn200ParameterAxis11,
		fn200ParameterAxis12,
		fn200ParameterAxis13,
		fn200ParameterAxis14,
		fn200ParameterAxis15,
		fn200ParameterAxis16,
		fn200ParameterAxis17,
		fn200ParameterAxis18,
		fn200ParameterAxis19,
		fn200ParameterAxis20,
		fn200ParameterAxis21,
		fn200ParameterAxis22,
		fn200ParameterAxis23,
		fn200ParameterAxis24,
		fn200ParameterAxis25,
		fn200ParameterAxis26,
		fn200ParameterAxis27,
		fn200ParameterAxis28,
		fn200ParameterAxis29,
		fn200ParameterAxis30,
		fn200ParameterAxis31,

        fn200Plot0CfgSts		= 512,
        fn200Plot1CfgSts,
        fn200Plot2CfgSts,
        fn200Plot3CfgSts,
        fn200Plot5CfgSts,
        fn200Plot6CfgSts,
        fn200Plot7CfgSts,
        fn200Plot8CfgSts,
        fn200Plot9CfgSts,
        fn200Plot10CfgSts,
        fn200Plot11CfgSts,
        fn200Plot12CfgSts,
        fn200Plot13CfgSts,
        fn200Plot14CfgSts,
        fn200Plot15CfgSts,
        fn200Plot16CfgSts,
        fn200Plot17CfgSts,
        fn200Plot18CfgSts,
        fn200Plot19CfgSts,
        fn200Plot20CfgSts,
        fn200Plot21CfgSts,
        fn200Plot22CfgSts,
        fn200Plot23CfgSts,
        fn200Plot24CfgSts,
        fn200Plot25CfgSts,
        fn200Plot26CfgSts,
        fn200Plot27CfgSts,
        fn200Plot28CfgSts,
        fn200Plot29CfgSts,
        fn200Plot30CfgSts,
        fn200Plot31CfgSts,

        fn200Plot0DynUA			= 576,
        fn200Plot1DynUA,
        fn200Plot2DynUA,
        fn200Plot3DynUA,
        fn200Plot4DynUA,
        fn200Plot5DynUA,
        fn200Plot6DynUA,
        fn200Plot7DynUA,
        fn200Plot8DynUA,
        fn200Plot9DynUA,
        fn200Plot10DynUA,
        fn200Plot11DynUA,
        fn200Plot12DynUA,
        fn200Plot13DynUA,
        fn200Plot14DynUA,
        fn200Plot15DynUA,
        fn200Plot16DynUA,
        fn200Plot17DynUA,
        fn200Plot18DynUA,
        fn200Plot19DynUA,
        fn200Plot20DynUA,
        fn200Plot21DynUA,
        fn200Plot22DynUA,
        fn200Plot23DynUA,
        fn200Plot24DynUA,
        fn200Plot25DynUA,
        fn200Plot26DynUA,
        fn200Plot27DynUA,
        fn200Plot28DynUA,
        fn200Plot29DynUA,
        fn200Plot30DynUA,
        fn200Plot31DynUA,

        // Default assighment allowing access to first 4096 samples
        // of first 16 data sets of first 4 plots. Static plot upload
        // area may be reconfigured via Static Plot Upload Area Map.
        fn200Plot0StaticUA		= 640,
        fn200Plot1StaticUA		= 656,
        fn200Plot2StaticUA		= 672,
        fn200Plot3StaticUA		= 688,

        fn200VarCurValues		= 1024,
        fn200VarInitValues		= 1280,
        fn200VarAttributes		= 1536,
	};

	// RMC200 Miscellaneous Constants - for convenience only
	enum enumCONSTANTS200
	{
		rmc200CmdRegsPerAxis = 10
	};

	enum enumRMCLINKERRORS
	{
		RMCLINK_E_FIRST =				RMCLINK_CTL_HRESULT(1000),	// 0x800A03E8
		RMCLINK_E_LAST =				RMCLINK_CTL_HRESULT(1999),	// 0x800A07CF

		RMCLINK_E_ALREADYLOCKED =		RMCLINK_CTL_HRESULT(1001),	// 0x800A03E9
		RMCLINK_E_DISCONNECTING =		RMCLINK_CTL_HRESULT(1002),	// 0x800A03EA
		RMCLINK_E_BADTYPE =				RMCLINK_CTL_HRESULT(1003),	// 0x800A03EB
		RMCLINK_E_BADLEN =				RMCLINK_CTL_HRESULT(1004),	// 0x800A03EC
		RMCLINK_E_DISCONNECTED =		RMCLINK_CTL_HRESULT(1005),	// 0x800A03ED
		RMCLINK_E_ALREADY_CONN =		RMCLINK_CTL_HRESULT(1006),	// 0x800A03EE
		RMCLINK_E_ALREADY_DISC =		RMCLINK_CTL_HRESULT(1007),	// 0x800A03EF
		RMCLINK_E_SOCK_INTERNAL =		RMCLINK_CTL_HRESULT(1008),	// 0x800A03F0
		RMCLINK_E_SOCK_CONN_FAIL =		RMCLINK_CTL_HRESULT(1009),	// 0x800A03F1
		RMCLINK_E_CANCELLED =			RMCLINK_CTL_HRESULT(1010),	// 0x800A03F2
		RMCLINK_E_TIMEOUT =				RMCLINK_CTL_HRESULT(1011),	// 0x800A03F3
		RMCLINK_E_SOCK_HOSTNAME_NOTIP =	RMCLINK_CTL_HRESULT(1012),	// 0x800A03F4
		RMCLINK_E_SOCK_HOSTNAME_FAIL =	RMCLINK_CTL_HRESULT(1013),	// 0x800A03F5
		RMCLINK_E_SOCK_GRACEFUL_CLOSE =	RMCLINK_CTL_HRESULT(1014),	// 0x800A03F6
		RMCLINK_E_SOCK_ERROR_RD =		RMCLINK_CTL_HRESULT(1015),	// 0x800A03F7
		RMCLINK_E_SOCK_INV_RD_LEN =		RMCLINK_CTL_HRESULT(1016),	// 0x800A03F8
		RMCLINK_E_SOCK_ERROR_WR =		RMCLINK_CTL_HRESULT(1017),	// 0x800A03F9
		RMCLINK_E_SOCK_INV_WR_LEN =		RMCLINK_CTL_HRESULT(1018),	// 0x800A03FA
		RMCLINK_E_PORT_ACCESS_DENIED =	RMCLINK_CTL_HRESULT(1019),	// 0x800A03FB
		RMCLINK_E_PORT_NOT_FOUND =		RMCLINK_CTL_HRESULT(1020),	// 0x800A03FC
		RMCLINK_E_CE_IOE =				RMCLINK_CTL_HRESULT(1021),	// 0x800A03FD
		RMCLINK_E_CE_FRAME =			RMCLINK_CTL_HRESULT(1022),	// 0x800A03FE
		RMCLINK_E_CE_BREAK =			RMCLINK_CTL_HRESULT(1023),	// 0x800A03FF
		RMCLINK_E_CE_OVERRUN =			RMCLINK_CTL_HRESULT(1024),	// 0x800A0400
		RMCLINK_E_CE_RXOVER =			RMCLINK_CTL_HRESULT(1025),	// 0x800A0401
		RMCLINK_E_CE_RXPARITY =			RMCLINK_CTL_HRESULT(1026),	// 0x800A0402
		RMCLINK_E_CE_TXFULL =			RMCLINK_CTL_HRESULT(1027),	// 0x800A0403
		RMCLINK_E_CONN_BROKEN =			RMCLINK_CTL_HRESULT(1028),	// 0x800A0404
		RMCLINK_E_PARTIAL_WRITE =		RMCLINK_CTL_HRESULT(1029),	// 0x800A0405
		RMCLINK_E_READ_ONLY =			RMCLINK_CTL_HRESULT(1030),	// 0x800A0406
		RMCLINK_E_PRIVATE_RANGE =		RMCLINK_CTL_HRESULT(1031),	// 0x800A0407
		RMCLINK_E_BAD_ADDRESS =			RMCLINK_CTL_HRESULT(1032),	// 0x800A0408
		RMCLINK_E_SOCK_HOSTNAME_INV =	RMCLINK_CTL_HRESULT(1033),	// 0x800A0409
		RMCLINK_E_WRONG_CONTROLLER =	RMCLINK_CTL_HRESULT(1034),	// 0x800A040A
		RMCLINK_E_NOT_SUPPORTED =		RMCLINK_CTL_HRESULT(1035),	// 0x800A040B
		RMCLINK_E_BLDIMG_TIMEOUT =		RMCLINK_CTL_HRESULT(1036),	// 0x800A040C
		RMCLINK_E_BLDIMG_ERROR =		RMCLINK_CTL_HRESULT(1037),	// 0x800A040D
		RMCLINK_E_BLDIMG_STATE =		RMCLINK_CTL_HRESULT(1038),	// 0x800A040E
		RMCLINK_E_UL_FAILED =			RMCLINK_CTL_HRESULT(1039),	// 0x800A040F
		RMCLINK_E_DL_CANT_RESET =		RMCLINK_CTL_HRESULT(1040),	// 0x800A0410
		RMCLINK_E_DL_FAILED =			RMCLINK_CTL_HRESULT(1041),	// 0x800A0411
		RMCLINK_E_BAD_IMAGE =			RMCLINK_CTL_HRESULT(1042),	// 0x800A0412
		RMCLINK_E_APPLY_TIMEOUT =		RMCLINK_CTL_HRESULT(1043),	// 0x800A0413
		RMCLINK_E_APPLY_STATE =			RMCLINK_CTL_HRESULT(1044),	// 0x800A0414
		RMCLINK_E_APPLY_RSTREQ =		RMCLINK_CTL_HRESULT(1045),	// 0x800A0415
		RMCLINK_E_APPLY_FLASH =			RMCLINK_CTL_HRESULT(1046),	// 0x800A0416
		RMCLINK_E_APPLY_NOTINRUN =		RMCLINK_CTL_HRESULT(1047),	// 0x800A0417
		RMCLINK_E_APPLY_WRONGHW =		RMCLINK_CTL_HRESULT(1048),	// 0x800A0418
		RMCLINK_E_USB_NO_DRIVERS =		RMCLINK_CTL_HRESULT(1049),	// 0x800A0419
		RMCLINK_E_USB_DEV_NOT_FOUND =	RMCLINK_CTL_HRESULT(1050),	// 0x800A041A
		RMCLINK_E_USB_SETTIMEOUT =		RMCLINK_CTL_HRESULT(1051),	// 0x800A041B
		RMCLINK_E_USB_TXRXCLEAR =		RMCLINK_CTL_HRESULT(1052),	// 0x800A041C
		RMCLINK_E_USB_OP_ABORT =		RMCLINK_CTL_HRESULT(1053),	// 0x800A041D
		RMCLINK_E_USB_DISCONNECT =		RMCLINK_CTL_HRESULT(1054),	// 0x800A041E
		RMCLINK_E_USB_UNK_FAILURE =		RMCLINK_CTL_HRESULT(1055),	// 0x800A042F
		RMCLINK_E_USB_IN_USE =			RMCLINK_CTL_HRESULT(1056),	// 0x800A0420
		RMCLINK_E_USB_NO_SETUPAPI =		RMCLINK_CTL_HRESULT(1057),	// 0x800A0421
		RMCLINK_E_FILE_NOT_FOUND =		RMCLINK_CTL_HRESULT(1058),	// 0x800A0422
		RMCLINK_E_PATH_NOT_FOUND =		RMCLINK_CTL_HRESULT(1059),	// 0x800A0423
		RMCLINK_E_ACCESS_DENIED =		RMCLINK_CTL_HRESULT(1060),	// 0x800A0424
		RMCLINK_E_DRIVE_NOT_READY =		RMCLINK_CTL_HRESULT(1061),	// 0x800A0425
		RMCLINK_E_BLDIMG_NOT_ALLOWED =	RMCLINK_CTL_HRESULT(1062)	// 0x800A0426
	};

	~CRMCLink();

private:
	// Notice that with the private constructor, a CRMCLink object can only
	//	be created using one of the CreateSerialLink and CreateEthernetLink
	//	static methods.
	CRMCLink(RMCLinkLib::IRMCLink* pIRMCLink);

	// This interface pointer points to the real RMCLink COM object.
	RMCLinkLib::IRMCLink* m_pIRMCLink;

	// Worker function.
	enum enumLINKTYPE { LT_ENET, LT_SER, LT_USB };
	static CRMCLink* _CreateLink(enumLINKTYPE eLinkType, enumDEVICETYPE eDevType, LPCTSTR lpszAddress);
};
