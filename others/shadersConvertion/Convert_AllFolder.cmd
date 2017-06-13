
SET EmzPackPath=D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\emzpack.exe
SET EmbPackPath=D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\embpack.exe
SET HlslAsmPackPath=D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\hlslasm.exe
SET AsmToHlslCodePath=D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\asmToHlslCode.exe
SET DirectXShadersCompilatorPath="C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"


Mkdir working
Mkdir result

for %%i in (.\*.emz) do start /B /Wait Convert_EmzFile.cmd %%~ni

PAUSE