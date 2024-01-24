# 3dsMax-Curve-Importer-RMC-Tools
This tool streamlines the process of transferring curve data from 3dsMax into an RMC. Currently supports RMC200, RMC150, & RMC70 series of controllers.

<img width="1144" alt="Screenshot 2024-01-24 at 1 50 04 PM" src="https://github.com/Matthew-Skovorodin/3dsMax-Curve-Importer-RMC-Tools/assets/146593166/6c2dbfe3-1455-4414-8412-395b828bea61">


## Resources
3dsMaxCurveConverter.exe – Command line executable program that receives inputs: -f filepath, -m controller type, -i link parameter, -c connection type, -o variable table offset, -r variable table range

TransferScript.ms – Script that is run in 3dsMax, it will automatically run the executable after finishing XMLtoRMC.h – Header file for 3dsMaxCurveConverter.cpp

RMCLink.h – Header file for RMC link library

tinyxml2.h – Header file for xml parser library

## Procedure
Three lines of code will have to be modified in the TransferScript.ms file in order to use the program.

01 - In File Explorer copy the pathname of 3dsMaxCurveConverter.exe 

02 - Replace the path programPath found in line 16 with this pathname
Note: single '\' must be replaced with double '\\' for the script to properly interpret the path

03 - Replace the path dataPath found in line 17 with the location you wish to export data
Note: single '\' must be replaced with double '\\' for the script to properly interpret the path

04 - Replace the path dataPathString found in line 18 with the same path as dataPath, except with single '\' rather than the double '\\'

## Startup Operation
Having the script run at the 3dsMax startup will simplify the process of running the program.

01 - In File Explorer copy the TransferScript.ms script file

02 - Navigate to the startup folder for 3dsMax: C:\Users\username\AppData\Local\Autodesk\3dsMax\2021 - 64bit\ENU\scripts\startup
Note: you may have to enable Show hidden files, folders, and drives to view AppData, additionally, in the username field, you must enter the name relative to your system

03 - Paste the TransferScript.ms script file into the folder and boot up 3dsMax

## Manual Operation
01 - In 3dsMax, within the Scripting menu select Open Script 

02 - Open the TransferScript.ms file

03 - In the MaxScript window select Tools then Evaluate All

## How it Works
01 - In 3dsMax the script will pull Positional / Rotational data from the highlighted objects in the scene 

02 - This data will be transmitted to a file named 3dsCurves.xml dedicated for use with the 3dsMaxCurveConverter executable

03 - The script will then run the executable program with the command line arguments described in the Resources section

04 - The executable program will process the data using the tinyxml2 library and send it to the RMC’s variable table using the RMCLink library, where it is then imported using curve add (82) command

## Using the Program
01 - In 3dsMax select the object(s) you would like to import curves for 

02 - Specify the Controller you are using in the dropdown list

03 - Specify the Connection Type you are using in the dropdown list

04 - Specify the files you would like to build in the Build File(s) dropdown list Note: if you have not yet created a 3dsCurves.xml file, no curve will exist for import, additionally, the 3dsCurves.rmccrvs file may be used directly with the RMCTools curve import function

05 - Specify the Variable Table Offset
Note: for the RMC70 and RMC150 the offset can range anywhere from 0 - 1000 for the RMC 200 the offset can range anywhere from 0 – 4076

06 - Specify the Variable Table Range you would like to use in the variable table
Note: larger ranges will produce faster curve imports
for the RMC70 and RMC150 the range can be set anywhere from 24 - 1024
for the RMC 200 the range can be set anywhere from 24 – 4096
make sure the area in the variable table that is being written to has REAL types, this will be true by default, additionally, any variables found within the specified range will be overwritten

07 - Specify the Link Parameter
Note: an RMC supports only one USB connection at a time. If the controller you wish to import curves into is connected to RMCTools via USB, 3dsMax must connect via Ethernet
if using USB the format is as follows:
RMC75E: The controller’s serial number "7573xxyyzz" RMC150: The Ethernet Address "00-50-A0-xx-yy-zz" RMC200: The CPU module's serial number "22Cyynnnn"
if using Serial the format is as follows: serial port such as "COM1" and "COM2"
if using Ethernet the format is as follows:
IP address such as “192.168.0.10” or a textual host name, such as “rmc.deltamotion.com”

08 - Specify the Start Frame of the curve(s)

09 - Specify the End Frame of the curve(s)

10 - Specify the Frames Per Second of the animation

11 - Select the Run Program button
Note: the progress bar on the bottom will indicate the status of the curve file creation, additionally, a terminal window will appear indicating the status of curve transfer to the RMC
