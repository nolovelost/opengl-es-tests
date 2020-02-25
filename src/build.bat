@echo off

mkdir ..\build
pushd ..\build
REM set include=C:\Imagination\PowerVR_Graphics\PowerVR_SDK\SDK_2019_R1.1\include
set include_external="X:\_external\opengles3-book\External\Include"
set include_common="X:\_external\opengles3-book\Common"
set include_v140="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.23.28105\include"
set include_crt="C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt"
set include_win="C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um"
set include_shared="C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\shared"
set includes=/I%include_external% /I%include_crt% /I%include_win% /I%include_v140% /I%include_shared%
set commons=..\Common\Win32\esUtil_win32.cpp ..\Common\esShader.c ..\Common\esTransform.c ..\Common\esShapes.c ..\Common\esUtil.c
set libEGL="X:\opengl-es-tests\lib\EGL\libEGL.lib"
set libGLESv2="X:\opengl-es-tests\lib\GLES2\libGLESv2.lib"
set lib_sys=User32.lib Gdi32.lib
set libraries=%libEGL% %libGLESv2% %lib_sys%
cl  %includes% /Zi %commons% ..\src\ogles.cpp %libraries%
popd
