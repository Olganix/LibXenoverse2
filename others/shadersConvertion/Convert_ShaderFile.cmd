

SET filename=%1
SET isPixelShader=%2 


start /B /Wait %HlslAsmPackPath% %filename%
DEL /s %filename%
start /B /Wait %AsmToHlslCodePath% %filename%.asm
DEL /s %filename%.asm
start /B /Wait Convert_CompileHlsl.cmd %filename% %isPixelShader%
DEL /s %filename%.hlsl


:sortie
EXIT
