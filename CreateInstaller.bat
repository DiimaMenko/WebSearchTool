@echo off
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "datestamp=%YYYY%%MM%%DD%"
set "timestamp=%HH%%Min%%Sec%"
set "fullstamp=%YYYY%-%MM%-%DD%_%HH%-%Min%-%Sec%"

@echo on

rem preparing directories

rmdir ..\WebSearchToolInstaller\ /s /Y

mkdir ..\WebSearchToolInstaller\packages\com.dmytro.WebSearchTool\data
mkdir ..\WebSearchToolInstaller\packages\com.dmytro.WebSearchTool\meta
mkdir ..\WebSearchToolInstaller\config
mkdir .\releases\%fullstamp%

rem copy files
rem note that release version of app must be built

copy /Y ..\build-WebSearchTool-Desktop_Qt_5_15_2_MinGW_64_bit-Release\release\WebSearchTool.exe ..\WebSearchToolInstaller\packages\com.dmytro.WebSearchTool\data\
copy /Y ..\WebSearchTool\config.xml ..\WebSearchToolInstaller\config\
copy /Y ..\WebSearchTool\package.xml ..\WebSearchToolInstaller\packages\com.dmytro.WebSearchTool\meta\
copy /Y ..\WebSearchTool\license.txt ..\WebSearchToolInstaller\packages\com.dmytro.WebSearchTool\meta\

rem run windeployqt tool

D:\Programs\Qt\5.14.2\mingw73_64\bin\windeployqt.exe ..\WebSearchToolInstaller\packages\com.dmytro.WebSearchTool\data\WebSearchTool.exe --qmldir .\

rem run installer creator (binarycreator)

D:\Programs\Qt\Tools\QtInstallerFramework\4.0\bin\binarycreator.exe -c ..\WebSearchToolInstaller\config\config.xml -p ..\WebSearchToolInstaller\packages ..\WebSearchToolInstaller\WebSearchToolInstaller.exe 

rem copying installer to release directory
copy ..\WebSearchToolInstaller\WebSearchToolInstaller.exe  .\releases\%fullstamp%\
pause