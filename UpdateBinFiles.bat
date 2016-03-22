REM - Script to prepare for Release

SET BINTARGET=bin
SET REGFREECOM=\ARP\BridgeLink\RegFreeCOM


xcopy /Y /d %REGFREECOM%\Win32\Release\XBeamRate*.dll		%BINTARGET%\Win32\
xcopy /Y /d %REGFREECOM%\x64\Release\XBeamRate*.dll		%BINTARGET%\x64\
xcopy /Y /d Documentation\doc\html\*           			%BINTARGET%\Docs\
xcopy /Y /d Documentation\XBRate.dm 				%BINTARGET%\Docs
xcopy /Y /d XBRate.tip 	 		                	%BINTARGET%\

xcopy /Y /d Images\*.png					%BINTARGET%\Images\
xcopy /Y /d Images\*.gif					%BINTARGET%\Images\
xcopy /Y /d Images\*.jpg					%BINTARGET%\Images\