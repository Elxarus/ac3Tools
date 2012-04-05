@echo off

set ver_text=%1
set ver_file=%2
set timestamp=%date:~8,2%%date:~3,2%%date:~0,2%%time:~0,2%%time:~3,2%
if "%time:~0,1%" == " " set timestamp=%date:~8,2%%date:~3,2%%date:~0,2%0%time:~1,1%%time:~3,2%

if "%ver_text%"=="" set ver_text=test %timestamp%
if "%ver_file%"=="" set ver_file=test_%timestamp%

rem -------------------------------------------------------
rem Clean all and make the source distribution
:build_source

cd ../valib
call clean.cmd
cd ../ac3filter
call clean.cmd

del ..\ac3filter_%ver_file%.exe
del ..\ac3filter_%ver_file%_lite.exe
del ..\ac3filter_%ver_file%_src.zip
del ..\ac3filter_%ver_file%_pdb.zip

set src_arc="ac3filter_%ver_file%_src.zip"
set src_files=ac3filter\*.* valib\*.*

call vars.cmd
cd ..
if exist "%src_arc%" del "%src_arc%"
%pkzip% -add -rec -dir -excl=.hg -excl=Debug -excl=Release -excl=x64 -excl=*.ncb -lev=9 %src_arc% %src_files%
if errorlevel 1 goto fail
cd ac3filter


rem -------------------------------------------------------
rem Build projects
:build_projects

rem -------------------------
rem Build projects

set PROJECTS=acm filter intl tools\ac3config tools\spdif_test

call cmd\build_all vc9 Win32 %PROJECTS%
if errorlevel 1 goto fail

call cmd\build_all vc9 x64 %PROJECTS%
if errorlevel 1 goto fail

rem -------------------------------------------------------
rem Build translations
:build_translations

cd lang
call update.cmd
if errorlevel 1 goto fail

call build.cmd
if errorlevel 1 goto fail
cd ..

rem -------------------------------------------------------
rem Build the installer
:build_installer

call vars.cmd
if "%inno_setup%" == "" set inno_setup=iscc.exe
if not exist "%inno_setup%" goto err_iss

"%inno_setup%" /Dappver="%ver_text%" /o".." /f"ac3filter_%ver_file%" ac3filter.iss
if errorlevel 1 goto fail

"%inno_setup%" /Dappver="%ver_text%" /o".." /f"ac3filter_%ver_file%_lite" ac3filter_lite.iss
if errorlevel 1 goto fail

rem -------------------------------------------------------
rem Build PDB archive
:build_pdb

set pdb_arc="..\ac3filter_%ver_file%_pdb.zip"
set pdb_files=filter\release\ac3filter.pdb filter\x64\release\ac3filter64.pdb

%pkzip% -add -lev=9 %pdb_arc% %pdb_files%
if errorlevel 1 goto fail

rem -------------------------------------------------------
rem All OK

echo All OK!
goto end

rem -------------------------------------------------------
rem Error messages

:err_iss
echo Inno setup compiler (iscc.exe) not found. Set %%inno_setup%% variable at vars.cmd.

:fail
echo Build failed!
error 2>nul
:end
