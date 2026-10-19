REM Copies the IC Imaging Control headers and libraries from their default installation location into the block.
REM Install the IC Imaging Control C++ SDK from The Imaging Source first:
REM https://www.theimagingsource.com/en-us/support/download/icimagingcontrolcpp-3.5.7/

xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\include" "include\icimagingcontrol"

xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\win32\debug" "lib\msw\x86"
xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\win32\release" "lib\msw\x86"

xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\x64\debug" "lib\msw\x64"
xcopy /s /Y "%HOMEDRIVE%%HOMEPATH%\Documents\IC Imaging Control 3.4\classlib\x64\release" "lib\msw\x64"

REM Append _x86 to the 32-bit libraries so the block manifest can select them with $(PlatformTarget).

cd lib\msw\x86
ren TIS_UDSHL11.dll TIS_UDSHL11_x86.dll
ren TIS_UDSHL11.lib TIS_UDSHL11_x86.lib
ren TIS_UDSHL11d.dll TIS_UDSHL11d_x86.dll
ren TIS_UDSHL11d.lib TIS_UDSHL11d_x86.lib
cd ..\..\..\
