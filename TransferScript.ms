/******************************************************************************
* Project: 3dsMax to RMC Plot Tool (version 5.0)
* Function: Write out 3dsMax data and run the transfer function
* Date: 5/29/2020
******************************************************************************/

/* resets button prompt */
try (cui.unregisterdialogbar maxscriptseries) catch()
try(destroydialog maxscriptseries) catch()

rollout maxscriptseries "Transfer to RMC" 
(

	--------------------------------------------------------------
	/* pathnames for the linker program and curve data  */
	--------------------------------------------------------------
	global programPath = "C:\\Users\\mskovorodin\\Documents\\3dsMaxProject\\3dsMaxCurveConverter\\x64\\Release\\3dsMaxCurveConverter.exe"
	global dataPath = "C:\\Users\\mskovorodin\\Documents"
	global dataPathString = "C:\Users\mskovorodin\Documents"


	--------------------
	/* User Interface */
	--------------------
	Button btn1 "Run Program" width: 136
	
	dropdownlist controllerList "Controller:" items:#("Select","dtRMC70", "dtRMC150", "dtRMC200") 
	on controllerList selected i do global dType = controllerList.items[i]
		
	dropdownlist connectionList "Connection Type:" items:#("Select", "Ethernet", "USB", "Serial") 
	on connectionList selected i do global cType = connectionList.items[i] 
		
	dropdownlist buildList "Build File(s):" items:#("Select", "dataRot.csv", "3dsCurves.rmccrvs", "3dsCurves.xml", "Include All", "Skip Build") 
	on buildList selected i do global build = buildList.items[i] 
		
	edittext offsetTxt "Variable Table Offset:" fieldWidth:135 labelOnTop:true
	on offsetTxt changed txt0 do
	(
		if ( txt0 != "" ) do global offset = txt0 as integer
	)	
	
	edittext rangeTxt "Variable Table Range:" fieldWidth:135 labelOnTop:true
	on rangeTxt changed txt1 do
	(
		if ( txt1 != "" ) do global range = txt1 as integer
	)	
	
	edittext linkTxt "Link Parameter:" fieldWidth:135 labelOnTop:true
	on linkTxt changed txt2 do
	(
		if ( txt2 != "" ) do global linkParam = txt2 
	)
	
	edittext startTxt "Start Frame:" fieldWidth:135 labelOnTop:true
	on startTxt changed txt3 do
	(
		if ( txt3 != "" ) do global startFrame = txt3 as integer
	)
	
	edittext endTxt "End Frame:" fieldWidth:135 labelOnTop:true
	on endTxt changed txt4 do
	(
		if ( txt4 != "" ) do global endFrame = txt4 as integer
	)
	
	edittext fpsTxt "Frames Per Second:" fieldWidth:135 labelOnTop:true
	on fpsTxt changed txt5 do
	(
		if ( txt5 != "" ) do global fps = txt5 as integer
	)
	
	progressbar dataTransfer  color:gray 


    ------------------------------------------------------------------------------------
	/* writeData functions will grab positional data and write it to an RMCTools file */
	------------------------------------------------------------------------------------
	Fn writeDataXRMC item itemID file step = 
	(
		itemTitle =  "dataPosX_" + itemID as string
		global itemName = item.name as string
		format "   <Curve\n       ID=\"%\"\n" counter to:file
		format "       Name=\"%\"\n" itemTitle to:file
		format "       Description=\"X-axis position data for %\"\n       Interpolation=\"2\"\n" itemName to:file
		format "       VelLimit=\"0\"\n       AccelLimit=\"0\">\n" to:file
		timeT = 0
		
		for t = startFrame to endFrame do
		(
			at time t format "       <CurvePoint Flags=\"0\" XValue=\"%\" YValue=\"%\"/>\n" timeT item.pos.x to:file
			timeT += step
		)
		format "   </Curve>\n" to:file
		counter += 1
	)
	
	Fn writeDataYRMC item itemID file step = 
	(
		itemTitle =  "dataPosY_" + itemID as string
		global itemName = item.name as string
		format "   <Curve\n       ID=\"%\"\n" counter to:file
		format "       Name=\"%\"\n" itemTitle to:file
		format "       Description=\"Y-axis position data for %\"\n       Interpolation=\"2\"\n" itemName to:file
		format "       VelLimit=\"0\"\n       AccelLimit=\"0\">\n" to:file
		timeT = 0
		
		for t = startFrame to endFrame do
		(
			at time t format "       <CurvePoint Flags=\"0\" XValue=\"%\" YValue=\"%\"/>\n" timeT item.pos.y to:file
			timeT += step
		)
		format "   </Curve>\n" to:file
		counter += 1
	)
	
	Fn writeDataZRMC item itemID file step = 
	(
		itemTitle =  "dataPosZ_" + itemID as string
		global itemName = item.name as string
		format "   <Curve\n       ID=\"%\"\n" counter to:file
		format "       Name=\"%\"\n" itemTitle to:file
		format "       Description=\"Z-axis position data for %\"\n       Interpolation=\"2\"\n" itemName to:file
		format "       VelLimit=\"0\"\n       AccelLimit=\"0\">\n" to:file
		timeT = 0
		
		for t = startFrame to endFrame do
		(
			at time t format "       <CurvePoint Flags=\"0\" XValue=\"%\" YValue=\"%\"/>\n" timeT item.pos.z to:file
			timeT += step
		)
		format "   </Curve>\n" to:file
		counter += 1
	)
	
	
    -------------------------------------------------------------------------------
	/* writeData functions will grab positional data and write it to an xml file */
	-------------------------------------------------------------------------------
	
	Fn writeDataX item itemID file = 
	(
		format "    <Curve>\n" to:file
		timeT = 0
		
		for t = startFrame to endFrame do
		(
			at time t format "       <Point>%</Point>\n" item.pos.x to:file
			timeT += step
		)
		format "    </Curve>\n" to:file
	)
	
	Fn writeDataY item itemID file = 
	(
		format "    <Curve>\n" to:file
		timeT = 0
		
		for t = startFrame to endFrame do
		(
			at time t format "       <Point>%</Point>\n" item.pos.y to:file
			timeT += step
		)
		format "    </Curve>\n" to:file
	)
	
	Fn writeDataZ item itemID file = 
	(
		format "    <Curve>\n" to:file
		timeT = 0
		
		for t = startFrame to endFrame do
		(
			at time t format "       <Point>%</Point>\n" item.pos.z to:file
			timeT += step
		)
		format "    </Curve>\n" to:file
	)
	
	
    ----------------------------------------------------------------------------------------
	/*   writeRot functions will grab rotational data and write it to an excel file   */
	----------------------------------------------------------------------------------------
	Fn writeRotX item file step = 
	(
		format "Time, X Rot.\n" to:file 
		timeT = 0
		for t = startFrame to endFrame do
		(
			at time t format "%,%\n" timeT item.rotation.x to:file
			timeT += step
		)
		close file	
	)
	
	Fn writeRotY item file step = 
	(
		format "Time, Y Rot.\n" to:file
		timeT = 0
		for t = startFrame to endFrame do
		(
			at time t format "%,%\n" timeT item.rotation.y to:file
			timeT += step
		)
		close file	
	)
	
	Fn writeRotZ item file step = 
	(
		format "Time, Z Rot.\n" to:file
		timeT = 0
		for t = startFrame to endFrame do
		(
			at time t format "%,%\n" timeT item.rotation.z to:file
			timeT += step
		)
		close file	
	)

	
	/* 'Run Program' pressed */
	on btn1 pressed do 
	(
		/* check user parameters */
		if( dType == "Select" ) then 
		(
			messagebox "Make selection for Controller"
			return 1
		)
		
		if( cType == "Select" ) then
		(
			messagebox "Make selection for Connection Type"
			return 1
		)
		
		if ( classOf offset !=  integer or \
		classOf startFrame !=  integer or \
		classOf endFrame !=  integer or \
		classOf range !=  integer or \
		classOf fps != integer )  then 
		(
			messagebox "Variables must be of type int"
			return 1
		)
		
		objs = selection as array 
		if ( objs.count == 0 ) then 
		(
			messagebox "No Objects Selected"
			return 1
		)
		
		global counter = 1 /* keeps track of file number */
		curveCount = (objs.count * 3) /* total amount of curves */
		pointCount = ((endFrame - startFrame)+1) /* points in single curve */
		global step = (1000/fps) /* xInterval */ 
		progress = 0 /* progress bar completion */
		itemID = 1 
		
		if( range < 24 ) then
		(
			messagebox "Offset must at least be 24"
			return 1
		)
		
			
		/* check if range falls within the variable table limits for the particular controller */	
		if( dType == "dtRMC150" ) then
		(
			if( offset > 1000 ) then 
			(
				messagebox "Offset must be less than 1000"
				return 1
			)
			if( range > 1024 ) then
			(
				messagebox "Range cannot exceed 1024 for this controller"
				return 1
			)
			space = 1024 - offset 
			if( range > space ) then
			(
				messagebox "range exceeds available space in the controller"
				return 1
			)
		)
				
		else if( dType == "dtRMC70" ) then
		(
			if( offset > 1000 ) then 
			(
				messagebox "Offset must be less than 1000"
				return 1
			)
			if( range > 1024 ) then
			(
				messagebox "Range cannot exceed 1024 for this controller"
				return 1
			
			)
			space = 1024 - offset 
			if( range > space ) then
			(
				messagebox "range exceeds available space in the controller"
				return 1
			)
		)
				
		else
		(
			if( offset > 4072 ) then 
			(
				messagebox "Offset must be less than 4072"
				return 1
			)
			if( range > 4096 ) then
			(
				messagebox "Range cannot exceed 4096 for this controller"
				return 1
			
			)
			space = 4096 - offset 
			if( range > space ) then
			(
				messagebox "range exceeds available space in the controller"
				return 1
			)
		)
				
		/* if the program  fails for any reason, we will close the files */
		try(
				if( build == "Select" ) then
				(
					messagebox "Make selection for Build File(s)"
					return 1
				)
				
				else if( build == "Include All" ) then
				(
					/* create file directories for rotational and positional data */
					rotDirectory = dataPathString as string + "\3dsMaxData\RotationalData"
					makeDir rotDirectory
					posDirectory = dataPathString as string + "\3dsMaxData\PositionalData"
					makeDir posDirectory	
					
					/* Create an xml file for the data */
					fileName = posDirectory + "\3dsCurves.rmccrvs" 
					fileRMC = createFile fileName 
					fileName = posDirectory + "\3dsCurves.xml" 
					fileData = createFile fileName 
					
					/* file formatting essentials */
					format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" to:fileRMC
					format "<Curves>\n" to:fileRMC
					
					format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" to:fileData
					format "<Root>\n" to:fileData
					format "    <curveCount>%</curveCount>\n" curveCount to:fileData
					format "    <pointCount>%</pointCount>\n" pointCount to:fileData
					format "    <xInterval>%</xInterval>\n" step to:fileData
								
					For item in objs do 
					(
					/* progress bar value */
						dataTransfer.value = 100.*(progress)/curveCount
						
						/* creates excel files for rotation */
						fileName = rotDirectory + "\dataRotX_" + itemID as string + ".csv" 
						fileRotX = createFile fileName 
						fileName = rotDirectory + "\dataRotY_" + itemID as string + ".csv" 
						fileRotY = createFile fileName 
						fileName = rotDirectory + "\dataRotZ_" + itemID as string + ".csv" 
						fileRotZ = createFile fileName 
				
						writeDataXRMC item itemID fileRMC step
						writeDataYRMC item itemID fileRMC step
						writeDataZRMC item itemID fileRMC step
						progress += 1
						writeDataX item itemID fileData 
						writeDataY item itemID fileData 
						writeDataZ item itemID fileData 
						progress += 1
						writeRotX item fileRotX step
						writeRotY item fileRotY step
						writeRotZ item fileRotZ step
						progress += 1
						
						/* moving onto a new curve */
						itemID += 1
					)
					format "</Curves>\n" to:fileRMC
					close fileRMC	
					format "</Root>\n" to:fileData
					close fileData
					dataTransfer.value = 100
				)
				
				else if( build == "dataRot.csv" ) then
				(
					/* create file directories for rotational and positional data */
					rotDirectory = dataPathString as string + "\3dsMaxData\RotationalData"
					makeDir rotDirectory
		
					For item in objs do 
					(
						dataTransfer.value = 100.*(progress)/curveCount
						
						/* creates excel files for rotation */
						fileName = rotDirectory + "\dataRotX_" + itemID as string + ".csv" 
						fileRotX = createFile fileName 
						fileName = rotDirectory + "\dataRotY_" + itemID as string + ".csv" 
						fileRotY = createFile fileName 
						fileName = rotDirectory + "\dataRotZ_" + itemID as string + ".csv" 
						fileRotZ = createFile fileName 
						writeRotX item fileRotX step
						progress += 1
						writeRotY item fileRotY step
						progress += 1
						writeRotZ item fileRotZ step
						progress += 1
						
						itemID += 1
					)
					dataTransfer.value = 100
				)
				
				else if( build == "3dsCurves.rmccrvs" ) then
				(
					posDirectory = dataPathString as string + "\3dsMaxData\PositionalData"
					makeDir posDirectory	
					/* create a xml file for the data */
					fileName = posDirectory + "\3dsCurves.rmccrvs" 
					fileRMC = createFile fileName 
					
					format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" to:fileRMC
					format "<Curves>\n" to:fileRMC

					For item in objs do 
					(
						dataTransfer.value = 100.*(progress)/curveCount
						writeDataXRMC item itemID fileRMC step
						progress += 1
						writeDataYRMC item itemID fileRMC step
						progress += 1
						writeDataZRMC item itemID fileRMC step
						progress += 1
						
						itemID += 1
					)
					dataTransfer.value = 100
					format "</Curves>\n" to:fileRMC
					close fileRMC
				)
				
				else if( build == "3dsCurves.xml" ) then
				(
					posDirectory = dataPathString as string + "\3dsMaxData\PositionalData"
					makeDir posDirectory	
					/* create a xml file for the data */
					fileName = posDirectory + "\3dsCurves.xml" 
					fileData = createFile fileName 
					
					/* file formatting essentials */
					/*format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" to:fileData*/
					format "<Root>\n" to:fileData
					format "    <curveCount>%</curveCount>\n" curveCount to:fileData
					format "    <pointCount>%</pointCount>\n" pointCount to:fileData
					format "    <xInterval>%</xInterval>\n" step to:fileData 
					
					For item in objs do 
					(
						dataTransfer.value = 100.*(progress)/curveCount
						writeDataX item itemID fileData 
						progress += 1
						writeDataY item itemID fileData 
						progress += 1
						writeDataZ item itemID fileData 
						progress += 1

						itemID += 1
					)
					dataTransfer.value = 100
					format "</Root>\n" to:fileData
					close fileData	
				)
				
				/* launch program */
				paramString = " -f " + dataPath as string + "\\3dsMaxData\\PositionalData\\3dsCurves.xml" + " -m "  + dType as string + " -i " + linkParam as string + " -c " + cType as string + " -o " + offset as string + " -r " + range as string
				ShellLaunch programPath paramString
				dataTransfer.value = 0 /* reset progress bar */
				
				return 0
			)
			
			catch
			(
				/* error prompt */
				format "*** % ***\n" (getCurrentException())
				if( build == "Include All" ) then
				(
					close fileRMC
					close fileData
				)
				else if( build == "3dsCurves.rmccrvs" ) then close fileRMC
				else if( build == "3dsCurves.xml" ) then close fileData

				return 1
			)
	)

)

createdialog maxscriptseries
cui.registerDialogBar maxscriptseries
