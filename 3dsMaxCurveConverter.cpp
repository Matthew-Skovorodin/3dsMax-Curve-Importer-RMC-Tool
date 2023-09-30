/* 3dsMax XML to Controller Curve Input Program
 * written by Matthew Skovorodin
 * ---------------------------------------------
 * version 1.0
 */

#include "XMLtoRMC.h"

 /* Function: main
  * ------------------
  * Description:
  *     Recieves parameters from script, converts them to proper
  *	    format and calls all essential functions.
  */

int main(int argc, char** argv)
{
    std::wstring controller;
    std::wstring address; 
    std::wstring connection; 
    char* file = NULL; // pathname to 3dsCurves.xml
    int offset = 0; // offset within variable table
    int range = 0; // workable range of variable table  
    char input; // ENTER key program drop out
    CRMCLink::enumDEVICETYPE deviceType;
    for ( int i = 1; i < 12; i++ )
    {
        if ( strncmp(argv[i], "-f", 2) == 0 ) 
        {
            char* szFile = argv[++i];
            file = szFile;
        }
        else if ( strncmp(argv[i], "-m", 2) == 0 ) 
        {
            char* szController = argv[++i];
            controller = std::wstring(szController, szController + strlen(szController));
            deviceType = getControllerType(controller.c_str());
        }
        else if ( strncmp(argv[i], "-i", 2) == 0 ) 
        {
            char* szAddress = argv[++i];
            address = std::wstring(szAddress, szAddress + strlen(szAddress));
        }
        else if ( strncmp(argv[i], "-c", 2) == 0 ) 
        {
            char* szConnection = argv[++i];
            connection = std::wstring(szConnection, szConnection + strlen(szConnection));
        }
        else if ( strncmp(argv[i], "-o", 2) == 0 ) 
        {
            offset = std::atoi(argv[++i]);
        }
        else if ( strncmp(argv[i], "-r", 2) == 0 )
        {
            range = std::atoi(argv[++i]);
            if ( range < 24 )
            {
                printf("Range is too small\n");
                std::cout << "Press Enter key to exit";
                input = getchar();
                return EXIT_FAILURE;
            }
        }
        else 
        {
            printf("ERROR: Invalid Parameters. The arguments must have the following tags:\n");
            printf("-f (filepath) -m (controller model) -i (address) -c (conncetion type) -o (var.offset)\n");
            return EXIT_FAILURE;
        }
    }

    /* Create link */
    CRMCLink* pRMC = createLink(connection.c_str(), address.c_str(), deviceType);
    if ( pRMC == NULL ) 
    {
        printf("Link creation failed\n"); 
        std::cout << "Press Enter key to exit";
        input = getchar();
        return EXIT_FAILURE;
    }

    /* Open XML file for reading */
    tinyxml2::XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(file);
    tinyxml2::XMLNode* pRoot = xmlDoc.FirstChildElement("Root");
    if ( pRoot == nullptr ) 
    {
        printf("Error Reading file\n");
        std::cout << "Press Enter key to exit";
        input = getchar();
        return EXIT_FAILURE;
    }

    /* Grab essential curve data */
    CurveData curveInfo = formatAnalysis(file);
    if ( curveInfo.err == 1 ) 
    {
        printf("Error recieving curve data\n");
        std::cout << "Press Enter key to exit";
        input = getchar();
        return EXIT_FAILURE;
    }

    int controllerType = getController(controller.c_str());
    if ( (transferToController(range, offset, file, curveInfo, controllerType, pRMC)) == S_OK )
        printf("Succesfuly Transimtted all data\n");
    else
    {
        printf("An error occured while transimtting data\n");
        std::cout << "Press Enter key to exit";
        input = getchar();
        return EXIT_FAILURE;
    }

    /* End program */
    std::cout << "Press Enter key to exit";
    input = getchar();
    return EXIT_SUCCESS;
}



/* Function: createLink
 * ------------------
 * Description:
 *     Creates a digital link over the user specified interface.
 *
 * Parameter(s):
 *     connection - USB, Ethernet, and Serial specifiers
 *     address -
 *         USB:
 *            RMC75E: The controller’s serial number "7573xxyyzz"
 *            RMC150: The Ethernet Address "00-50-A0-xx-yy-zz"
 *            RMC200: The CPU module's serial number "22Cyynnnn"
 *         Serial:
 *            serial port such as "COM1" and "COM2"
 *         Ethernet:
 *            IP address or host name
 *     deviceType - RMC75E, RMC150, or RMC200
 *
 * Return Value:
 *     pRMC - Controller link parameter
 */

CRMCLink* createLink(const wchar_t* connection, const wchar_t* address, CRMCLink::enumDEVICETYPE deviceType)
{
    ::CoInitialize(0);

    CRMCLink* pRMC = NULL;
    if  ( (wcsncmp(connection, L"Ethernet", 8) == 0) )
        pRMC = CRMCLink::CreateEthernetLink(deviceType, address);
    else if ( (wcsncmp(connection, L"USB", 3) == 0) )
        pRMC = CRMCLink::CreateUSBLink(deviceType, address);
    else if ( (wcsncmp(connection, L"Serial", 6) == 0) )
        pRMC = CRMCLink::CreateUSBLink(deviceType, address);

    return pRMC;
}



/* Function: getControllerType
 * ------------------
 * Description:
 *     Translates controller into CRMCLink readable format.
 *
 * Parameter(s):
 *     controller - RMC75E, RMC150, or RMC200
 *
 * Return Value:
 *     deviceType - enumDEVICETYPE formatted parameter
 */

CRMCLink::enumDEVICETYPE getControllerType(const wchar_t* controller)
{
    if ( (wcsncmp(controller, L"dtRMC70", 7) == 0) )
        return (CRMCLink::dtRMC70);
    if ( (wcsncmp(controller, L"dtRMC150", 8) == 0) )
        return (CRMCLink::dtRMC150);
    if ( (wcsncmp(controller, L"dtRMC200", 8) == 0) )
        return (CRMCLink::dtRMC200);
}



/* Function: getController
 * ------------------
 * Description:
 *     Converts controller type into a integer.
 *
 * Parameter(s):
 *     controller - RMC75E, RMC150, or RMC200
 *
 * Return Value:
 *     controllerType - simplified integer formatted specifier
 *     1 = dtRMC70, 2 = dtRMC150, 3 = dtRMC200
 */

int getController(const wchar_t* controller)
{
    if ( (wcsncmp(controller, L"dtRMC70", 7) == 0) )
        return 1;
    if ( (wcsncmp(controller, L"dtRMC150", 8) == 0) )
        return 2;
    if ( (wcsncmp(controller, L"dtRMC200", 8) == 0) )
        return 3;
    
    return E_FAIL;
}



/* Function: formatAnalysis
 * ------------------
 * Description:
 *     Fills in curve data structure including formatAnalysis,
 *     pointCount and xInterval.
 *
 * Parameter(s):
 *     file - pathname to 3dsCurves.xml
 *
 * Return Value:
 *     curveInfo - contains essential curve formatting
 *     curveInfo.err = 1 if error is encountered
 */

CurveData formatAnalysis(char* file)
{
    XMLError eResult;
    CurveData curveInfo;

    tinyxml2::XMLDocument xmlDoc;
    eResult = xmlDoc.LoadFile(file);
    tinyxml2::XMLNode* pRoot = xmlDoc.FirstChildElement("Root");
    if ( pRoot == nullptr )
    {
        printf("Error Reading file\n");
    }

    XMLElement* pElement = pRoot->FirstChildElement("curveCount");
    if ( pElement == nullptr )
        printf("Error parsing element (curveCount)\n");
    else
    {
        eResult = pElement->QueryIntText(&curveInfo.curveCount);
        if ( eResult != XML_SUCCESS )
        {
            printf("Could not transfer curve count\n");
            curveInfo.err = 1;
        }
    }


    pElement = pRoot->FirstChildElement("pointCount");
    if ( pElement == nullptr )
        printf("Error parsing element (pointCount)\n");
    else
    {
        eResult = pElement->QueryIntText(&curveInfo.pointCount);
        if ( eResult != XML_SUCCESS )
        {
            printf("Could not transfer point count\n");
            curveInfo.err = 1;
        }
    }


    pElement = pRoot->FirstChildElement("xInterval");
    if ( pElement == nullptr )
        printf("Error parsing element (xInterval)\n");
    else
    {
        eResult = pElement->QueryIntText(&curveInfo.xInterval);
        if ( eResult != XML_SUCCESS )
        {
            printf("Could not transfer xInterval\n");
            curveInfo.err = 1;
        }
    }

    return curveInfo;
}



/* Function: connectController
 * ------------------
 * Description:
 *     Creates a connection with the controller and calls plot transfer
 *     functions.
 *
 * Parameter(s):
 *     range - user specified range in Variable Table
 *     offset - user specified offset in Variable Table
 *     file - pathname to 3dsCurves.xml
 *     curveInfo - contains essential curve formatting
 *     controllerType - RMC75E = 1, RMC150 = 2, or RMC200 = 3
 *     pRMC - Controller link parameter
 *
 * Return Value:
 *     returns E_FAIL upon failure
 *     returns S_OK upon success
 */

HRESULT transferToController(int range, int offset, char* file, CurveData curveInfo, int controllerType, CRMCLink* pRMC)
{
    HRESULT hr = pRMC->Connect();
    if ( SUCCEEDED(hr) )
        curveTransfer(controllerType, range, offset, file, curveInfo, pRMC);
    else
        return E_FAIL;

    pRMC->Disconnect();
    delete pRMC;
    ::CoUninitialize();

    return S_OK;
}



/* Function: curveTransfer
 * ------------------
 * Description:
 *     Grabs all the data points from the xml file and
 *     builds a vector.
 *
 * Parameter(s):
 *     controllerType - Controller specifier
 *     range - user specified range in Variable Table
 *     offset - user specified offset in Variable Table
 *     file - pathname to 3dsCurves.xml
 *     curveInfo - contains essential curve formatting
 *     pRMC - Controller link parameter
 *
 * Return Value:
 *     returns E_FAIL upon failure
 *     returns S_OK upon success
 */

HRESULT curveTransfer(int controllerType, int range, int offset, char* file, CurveData curveInfo, CRMCLink* pRMC)
{
    XMLError eResult;

    tinyxml2::XMLDocument xmlDoc;
    eResult = xmlDoc.LoadFile(file);
    tinyxml2::XMLNode* pRoot = xmlDoc.FirstChildElement("Root");
    if ( pRoot == nullptr )
    {
        printf("Error Reading file\n");
    }

    printf("File was read succesfuly\n");

    int currentCurve = 0;
    int currentPoint = 0;
    float pointValue;

    std::vector<float> curvesVector;

    XMLElement* pCurveElement = pRoot->FirstChildElement("Curve");
    XMLElement* pPointElement = pCurveElement->FirstChildElement("Point");
    while ( currentCurve < curveInfo.curveCount )
    {
        if ( pCurveElement == nullptr ) break;
        while ( currentPoint < curveInfo.pointCount )
        {
            if ( pPointElement == nullptr ) break;
            eResult = pPointElement->QueryFloatText(&pointValue);
            if ( eResult != XML_SUCCESS )
                printf("Could not transfer point value\n");
            curvesVector.push_back(pointValue); //append points to the vector
            pPointElement = pPointElement->NextSiblingElement("Point");
            currentPoint++;
        }
        pCurveElement = pCurveElement->NextSiblingElement("Curve");
        if ( pCurveElement == nullptr )
            break;
        pPointElement = pCurveElement->FirstChildElement("Point");
        currentPoint = 0;
        currentCurve++;
    }
    /* write the values to the RMC */
    if ( ! SUCCEEDED(writeCurve(controllerType, range, offset, pRMC, curveInfo, curvesVector)) ) {
        printf("Variable table write failed");
        return E_FAIL;
    }

    return S_OK;
}



/* Function: writeCmd
 * ------------------
 * Description:
 *     Writes Curve Add (82) cmd to specified controller.
 *
 * Parameter(s):
 *     controllerType - controller specifier
 *     offset - user specified offset in Variable Table
 *     curveCmd - an array containing Curve Add cmd
 *     pRMC - link parameter
 *
 * Return Value:
 *     returns E_FAIL upon failiure
 *     returns S_OK upon success
 */

HRESULT writeCmd(int controllerType, int offset, float* curveCmd, CRMCLink* pRMC)
{
    HRESULT hr = S_OK;

    if ( controllerType == 1 ) //RMC70
    { 
        curveCmd[2] = CRMCLink::fn70VarCurValues * 4096 + offset;
        hr = pRMC->WriteFFile(CRMCLink::fn70CommandArea, 0,
            CRMCLink::rmc70CmdRegsPerAxis, curveCmd);
    }
    if ( controllerType == 2 ) //RMC150
    {
        curveCmd[2] = CRMCLink::fn150VarCurValues * 4096 + offset;
        hr = pRMC->WriteFFile(CRMCLink::fn150CommandArea, 0,
            CRMCLink::rmc150CmdRegsPerAxis, curveCmd);
    }
    if ( controllerType == 3 ) //RMC200
    {
        curveCmd[2] = CRMCLink::fn200VarCurValues * 4096 + offset;
        hr = pRMC->WriteFFile(CRMCLink::fn200CommandArea, 0,
            CRMCLink::rmc200CmdRegsPerAxis, curveCmd);
    }

    return hr;
}



/* Function: writeCurve
 * ------------------
 * Description:
 *     Writes out curves containing vector into a buffer
 *     which is then written to the controller.
 *
 * Parameter(s):
 *     controllerType - simplified integer formatted specifier
 *     range - user specified range in Variable Table
 *     offset - user specified offset in Variable Table
 *     pRMC - Controller link parameter
 *     curveInfo - contains essential curve formatting
 *     curvesVector - contains all point data
 *
 * Return Value:
 *     returns E_FAIL upon failure
 *     returns S_OK upon success
 */

HRESULT writeCurve(int controllerType, int range, int offset, CRMCLink* pRMC, CurveData curveInfo, std::vector<float> curvesVector)
{
    int increment; // replaces maxSize in other parts of curve upload
    int curveID = 0; // curve number
    int transferAmount; // keeps track of how many variables are being uploaded
    int partialFlag = 0; // indicates if curve upload is a partial curve 
    int currentPoint = 0; // keeps track of current point in entire curves data structure
    int currentOffset = 0; // offset of current curve
    int maxSize = (range - 9); // Variable Table range with partial curve formatting (part 1)
    int pointsRemaining = curveInfo.pointCount; // points in current curve
    int totalPoints = (curveInfo.pointCount * curveInfo.curveCount);

    /* array for curve add command to be sent to the RMC*/
    float curveCmd[6];
    curveCmd[0] = 82;// curve add cmd
    curveCmd[1] = 0; // curve ID
    curveCmd[2] = 0; // table address start
    curveCmd[3] = 2; // interpolation
    curveCmd[4] = 0; // life cycle, std = 0, start once = 1, complete once = 2, permanent = 3
    
    /* cursor is a pointer that tracks memory location of data containing vector */
    std::vector<float> curveBuff(range);

    /* maxSize indicates the maximum amount of y value points that can be uploaded */
    maxSize = range - 9;

    while ( currentPoint < totalPoints )
    {
      /* If the curve is larger than the variable table limits */
      if ( pointsRemaining > maxSize)
        {
            if ( partialFlag == 0 ) { // first part of large curve download
                curveBuff[0] = 0;
                curveBuff[1] = 20; // format, evenly spaced
                curveBuff[2] = currentOffset; // offset of this part in curve 
                curveBuff[3] = maxSize+4; // points in this partial curve
                curveBuff[4] = (curveInfo.pointCount + 4); // total length
                curveBuff[5] = curveInfo.pointCount; // total points
                curveBuff[6] = 2; // Interpolation Options
                curveBuff[7] = 0; // Xo
                curveBuff[8] = curveInfo.xInterval; // Xinterval

                /* transfer data into buffer */
                for ( int i = 9; i < range; ++i )
                {
                    curveBuff[i] = curvesVector[currentPoint++];
                }
                currentOffset += (maxSize + 4); // offset for a part upload in partial curve
                pointsRemaining -= maxSize; // points remaining in current curve
                partialFlag = 1; //part#1 created 
            }
            
            else
            {
                increment = maxSize + 4;
                if ( increment > pointsRemaining )
                    increment = pointsRemaining;
                curveBuff[0] = 0;
                curveBuff[1] = 20; // format, evenly spaced
                curveBuff[2] = currentOffset; // offset of this part in curve 
                curveBuff[3] = increment; // points in this partial curve
                curveBuff[4] = curveInfo.pointCount + 4; // total length
                for ( int i = 5; i < (increment + 5); ++i )
                {
                    curveBuff[i] = curvesVector[currentPoint++];
                }
                currentOffset += increment;
                pointsRemaining -= increment;
            }

            if ( ! SUCCEEDED(writeToRMC(curveBuff, range, controllerType, offset, pRMC)) )
            {
                printf("Curve write failed\n");
                return E_FAIL;
            }

            if ( ! SUCCEEDED(writeCmd(controllerType, offset, curveCmd, pRMC)) ) 
            {
                printf("Command write failed\n");
                return E_FAIL;
            }  

            if ( delayLoop(controllerType, pRMC) == E_FAIL )
                return E_FAIL;
        }

        else /* If the curve fits in our variable table limits */
        {
            if ( partialFlag != 1 ) // importing smaller curve
            {
                curveBuff[0] = 0;
                curveBuff[1] = 0; // format 
                curveBuff[2] = curveInfo.pointCount; // total # of points
                curveBuff[3] = 2; // interpolation
                curveBuff[4] = 0; // Xo 
                curveBuff[5] = curveInfo.xInterval; // Xinterval
                transferAmount = pointsRemaining + 6;
                for ( int i = 6; i < transferAmount; ++i )
                {
                    curveBuff[i] = curvesVector[currentPoint++];
                }
                currentOffset += pointsRemaining;
            }

            else
            {
                increment = pointsRemaining + 4;
                curveBuff[0] = 0;
                curveBuff[1] = 20; // format, evenly spaced
                curveBuff[2] = currentOffset; // offset of this part in curve 
                curveBuff[3] = increment; // points in this partial curve
                curveBuff[4] = (curveInfo.pointCount + 4); // total # of points
                transferAmount = (pointsRemaining + 5);
                for ( int i = 5; i < transferAmount; ++i )
                {
                    curveBuff[i] = curvesVector[currentPoint++];
                }
                
                currentOffset += pointsRemaining;
            }

            if ( ! SUCCEEDED(writeToRMC(curveBuff, transferAmount, controllerType, offset, pRMC)) )
            {
                printf("Curve write failed\n");
                return E_FAIL;
            }

            if ( ! SUCCEEDED(writeCmd(controllerType, offset, curveCmd, pRMC)) ) {
                printf("Command write failed\n");
                return E_FAIL;
            }

            if ( delayLoop(controllerType, pRMC) == E_FAIL )
                return E_FAIL;

            printf("Imported Curve[%d]\n", curveID);
            pointsRemaining = curveInfo.pointCount;
            partialFlag = 0; //reset parital curve indicator
            currentOffset = 0;
            curveID++;
            curveCmd[1] = curveID;
        }
    }
    return S_OK;
}




/* Function: writeToRMC
 * ------------------
 * Description:
 *     Writes variable table curve to specified controller.
 *
 * Parameter(s):
 *      curveBuff - curve data array
 *      transferAmount - the amount of variable table elements to write
 *      controllerType - controller specifier
 *      offset - first variable table entry
 *      pRMC - link parameter
 *
 * Return Value:
 *     returns 1 on failiure
 *     returns 0 on success
 */

HRESULT writeToRMC(std::vector<float> &curveBuff, int transferAmount, int controllerType, int offset, CRMCLink* pRMC)
{
    HRESULT hr = S_OK; 
    if  ( controllerType == 1 ) //RMC70
        hr = pRMC->WriteFFile(CRMCLink::fn70VarCurValues, offset,
            transferAmount, &curveBuff[0]);
    if ( controllerType == 2 ) //RMC150
        hr = pRMC->WriteFFile(CRMCLink::fn150VarCurValues, offset,
            transferAmount, &curveBuff[0]);
    if ( controllerType == 3 ) //RMC200
        hr = pRMC->WriteFFile(CRMCLink::fn200VarCurValues, offset,
            transferAmount, &curveBuff[0]);

    return hr;
} 


/* Function: delayLoop
 * ------------------
 * Description:
 *     Delays the status bit check from controller.
 *
 * Parameter(s):
 *      controllerType - controller specifier
 *      pRMC - link parameter
 *
 * Return Value:
 *     returns E_FAIL upon failiure
 *     returns S_OK upon success
 */

HRESULT delayLoop(int controllerType, CRMCLink* pRMC)
{
    int delay = 0; // used as a timeout counter
    Sleep(10); // allows controller the time to reset status flag
    while (1) // this becomes more apparent over slower connection types
    {
        Sleep(10);
        if ( (checkController(controllerType, pRMC)) == S_OK )
            return S_OK;
        if ( delay > 100 )
        {
            printf("Writing to controller timeout\n");
            return E_FAIL; // timeout
        }
        delay++;
    }
}



/* Function: checkController
 * ------------------
 * Description:
 *     Checks the controllers status bit
 *
 * Parameter(s):
 *      controllerType - controller specifier
 *      pRMC - link parameter
 *
 * Return Value:
 *     returns E_FAIL upon failiure
 *     returns S_OK upon success
 */

HRESULT checkController(int controllerType, CRMCLink* pRMC)
{
    HRESULT hr;
    float curveReady[1];
    
    if ( controllerType == 1 ) //RMC70
        hr = pRMC->ReadFFile(CRMCLink::fn70VarCurValues, 0, 0, curveReady);
    if ( controllerType == 2 ) //RMC150
        hr = pRMC->ReadFFile(CRMCLink::fn150VarCurValues, 0, 0, curveReady);
    if ( controllerType == 3 ) //RMC200
        hr = pRMC->ReadFFile(CRMCLink::fn200VarCurValues, 0, 0, curveReady);

    if( curveReady[0] == 2.0 || 3.0 )
        return S_OK;
    else 
        return E_FAIL;
}


