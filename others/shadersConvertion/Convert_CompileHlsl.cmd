

SET filename=%1
if "%2" == "true" goto:pixelShader 


%DirectXShadersCompilatorPath% /T vs_3_0 /E main_vs /Od /Fo %filename% %filename%.hlsl
goto:sortie

:pixelShader
%DirectXShadersCompilatorPath% /T ps_3_0 /E main_ps /Od /Fo %filename% %filename%.hlsl
goto:sortie


:sortie
EXIT