@echo off

SETLOCAL

call "E:\Windows Development Tools\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat" x64

set CFLAGS=/W4 /Zi /nologo /Fetexterer.exe /MD /MP4 /std:c11
set CFLAGS=%CFLAGS% /Iw:\text_renderer\libraries\glfw\include
set LINKFLAGS=glfw3.lib opengl32.lib shell32.lib gdi32.lib user32.lib kernel32.lib /link
set LINKFLAGS=%LINKFLAGS% /LIBPATH:w:\text_renderer\libraries\glfw\lib-vc2022

set CFILES=

pushd "W:\text_renderer\code"
for /R %%c in (*.c) do call set CFILES=%%c %%CFILES%%
popd

IF NOT EXIST "W:\text_renderer\bin" mkdir W:\text_renderer\bin
pushd "W:\text_renderer\bin"
cl %CFLAGS% %CFILES% %LINKFLAGS%
popd

ENDLOCAL
