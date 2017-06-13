

SET filename=%1
if "%filename%" == "Convert_AllFolder" goto :sortie
if "%filename%" == "Convert_EmzFile" goto :sortie
if "%filename%" == "Convert_ShaderFile" goto :sortie
if "%filename%" == "Convert_CompileHlsl" goto :sortie
if "%filename%" == "libxenoverse" goto :sortie
if "%filename%" == "libxenoverse_Read" goto :sortie
if "%filename%" == "libxenoverse_Write" goto :sortie

echo "-------- Convert file %filename% --------"


COPY "%filename%.emz" "working"
start /B /Wait %EmzPackPath% .\working\%filename%.emz
del /s .\working\%filename%.emz

start /B /Wait %EmbPackPath% .\working\%filename%.emb

for %%j in (.\working\%filename%\*.xvu) do start /B /Wait Convert_ShaderFile.cmd %%~j false
for %%j in (.\working\%filename%\*.xpu) do start /B /Wait Convert_ShaderFile.cmd %%~j true

start /B /Wait %EmbPackPath% .\working\%filename%
start /B /Wait %EmzPackPath% .\working\%filename%.emb
del /s .\working\%filename%.emb

::for %%j in (.\working\%filename%\*.*) do DEL /s %%~j
::Rmdir .\working\%filename%


COPY ".\working\%filename%.emz" "result"
del /s .\working\%filename%.emz

::Rmdir .\working

:sortie
EXIT
