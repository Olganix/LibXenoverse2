

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"  /T vs_3_0 /E main_vs /Od /Fo TOON_UNIF_STAIN3_DFD_W_VS.xvu TOON_UNIF_STAIN3_DFD_W_VS.xvu.hlsl
::::PAUSE
::COPY TOON_UNIF_STAIN3_DFD_W_VS.xvu shader_age_vs
::D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\embpack.exe shader_age_vs

::"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"  /T ps_3_0 /E main_ps /Od /Fo TOON_UNIF_STAIN3_DFD_PS.xpu TOON_UNIF_STAIN3_DFD_PS.xpu.hlsl
::::PAUSE
::COPY TOON_UNIF_STAIN3_DFD_PS.xpu shader_age_ps
::D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\embpack.exe shader_age_ps


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"  /T vs_3_0 /E main_vs /Od /Fo TOON_UNIF_STAIN3_DFD_W_VS.xvu TOON_UNIF_STAIN3_DFD_W_VS.xvu.hlsl
::::PAUSE
::COPY TOON_UNIF_STAIN3_DFD_W_VS.xvu shader_age_vs
::D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\embpack.exe shader_age_vs


"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"  /T ps_3_0 /E main_ps /Od /Fo TN0002_UNIF_STN2d2_XVM_DFD_PS.xpu TN0002_UNIF_STN2d2_XVM_DFD_PS.xpu.hlsl
::PAUSE
COPY TN0002_UNIF_STN2d2_XVM_DFD_PS.xpu shader_age_ps
D:\Tests\DragonBall_Xenoverse\LibXenoverse-master\LibXenoverse-master\LibXenoverse\bin\Debug\embpack.exe shader_age_ps