#pragma once

#include <iostream>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <dos.h> //for delay
#include "RMCLink.h"
#include "tinyxml2.h"

#define RMC200REG 4096
#define RMCREG 1024

using namespace tinyxml2;

struct CurveData {
    int curveCount,
        pointCount,
        xInterval,
        err;
};

/*---------------------------*/
/*    Function Definitions   */
/*---------------------------*/
CRMCLink* createLink(const wchar_t* connection, const wchar_t* address, CRMCLink::enumDEVICETYPE deviceType);
CRMCLink::enumDEVICETYPE getControllerType(const wchar_t* controller);
int getController(const wchar_t* controller);
CurveData formatAnalysis(char* file);
HRESULT transferToController(int range, int offset, char* file, CurveData curveInfo, int controllerType, CRMCLink* pRMC);
HRESULT curveTransfer(int controllerType, int range, int offset, char* file, CurveData curveInfo, CRMCLink* pRMC);
HRESULT writeCmd(int controllerType, int offset, float* curveCmd, CRMCLink* pRMC);
HRESULT writeCurve(int controllerType, int range, int offset, CRMCLink* pRMC, CurveData curveInfo, std::vector<float> curvesVector);
HRESULT writeToRMC(std::vector<float> &curveBuff, int transferAmount, int controllerType, int offset, CRMCLink* pRMC);
HRESULT checkController(int controllerType, CRMCLink* pRMC);
HRESULT delayLoop(int controllerType, CRMCLink* pRMC);
