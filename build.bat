@echo off

SETLOCAL

call "E:\Windows Development Tools\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat" x64

set PATH=%PATH%;w:\text_renderer\libraries\freetype\release_dll\win64
set PATH=%PATH%;w:\text_renderer\libraries\glfw\lib-vc2022

set CFLAGS=/W4 /WX /Zi /nologo /Fe:texterer.exe /MP4 /wd4100 /FC /TC
set CFLAGS=%CFLAGS% /wd4996
set CFLAGS=%CFLAGS% /wd4267
set CFLAGS=%CFLAGS% /I w:\text_renderer\libraries\glfw\include
set CFLAGS=%CFLAGS% /I w:\text_renderer\libraries\freetype\include
set CFLAGS=%CFLAGS% /I w:\text_renderer\libraries\cglm\include
set CFLAGS=%CFLAGS% /I w:\text_renderer\libraries\glad\include
set LINKFLAGS=/link glfw3_mt.lib freetype.lib opengl32.lib shell32.lib gdi32.lib user32.lib kernel32.lib cglm.lib
set LINKFLAGS=%LINKFLAGS% /LIBPATH:W:\text_renderer\libraries\glfw\lib-vc2022
set LINKFLAGS=%LINKFLAGS% /LIBPATH:W:\text_renderer\libraries\freetype\release_static\vs2015-2022\win64
set LINKFLAGS=%LINKFLAGS% /LIBPATH:w:\text_renderer\libraries\cglm\win\Release
set LINKFLAGS=%LINKFLAGS% /ignore:4099

set CFILES=W:\text_renderer\libraries\glad\src\glad.c %CFILES%
pushd "W:\text_renderer\code"
for /R %%c in (*.c) do call set CFILES=%%c %%CFILES%%
popd

IF NOT EXIST "W:\text_renderer\bin" mkdir W:\text_renderer\bin
pushd "W:\text_renderer\bin"
cl %CFLAGS% %CFILES% %LINKFLAGS%
popd

ENDLOCAL
