REM This script copies the IC Imaging Control libraries from there default installation locations into the Block
REM folder.  This assumes that you have already installed the C+++ SDK from ImaingSource's Website:
REM http://www.theimagingsource.com/en_US/support/downloads/details/icimagingcontrol/

xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\include" "include\icimagingcontrol"

xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\win32\debug" "lib\msw\x86"
xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\win32\release" "lib\msw\x86"

xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\x64\debug" "lib\msw\x64"
xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\x64\release" "lib\msw\x64"

REM Append _x86 to the 32 bit libraries for ease of linking.

cd lib\msw\x86
ren TIS_UDSHL11.dll TIS_UDSHL11_x86.dll
ren TIS_UDSHL11.lib TIS_UDSHL11_x86.lib
ren TIS_UDSHL11d.dll TIS_UDSHL11d_x86.dll
ren TIS_UDSHL11d.lib TIS_UDSHL11d_x86.lib
cd ..\..\..\