@echo off
if "%1" == "" goto usage
if "%1" == "all" goto build_all
if not exist "%1" goto no_file
if "%1" == "common_packages.tex" goto inc_file
if "%1" == "common_style.tex" goto inc_file
if "%1" == "common_rus.tex" goto inc_file
goto build_file

:build_file
echo Building file %1
call clean_temp.cmd
pdflatex %1
pdflatex %1
pdflatex %1
call clean_temp.cmd
if not exist %~n1.pdf echo %1 : error: cannot build PDF && set errorlevel=1
goto end

:build_all
echo Building all files
call clean_temp.cmd
for %%f in (*.tex) do call build_pdf.cmd %%f 
call clean_temp.cmd
goto end

:no_file
echo File %1 does not exists
goto end

:inc_file
echo File %1 is include file (do not build)
goto end

:usage
echo Usage
echo   build_pdf file.tex - build pdf for the given tex file
echo   build_pdf all      - build pdf for all tex files
goto end

:end