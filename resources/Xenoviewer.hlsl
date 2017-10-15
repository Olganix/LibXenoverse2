



#define pi 3.141592654

SamplerState diffuseSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};



////////////////////////////////////////////////////////////////////////////////
void normal_vs(		float4 position : POSITION,
		              float4 normal   : NORMAL,
		              out float4 oPosition  : SV_Position,
		              out float4 oNormal    : TEXCOORD0,
		              uniform float4x4 worldViewProj
		           )
{
  oPosition = mul(worldViewProj, position);
  oNormal = normal;
}
////////////////////////////////////////////////////////////////////////////////
void normal_ps( float4 sPosition   : SV_Position,
                float4 normal : TEXCOORD0,
                out float4 color     : SV_Target
              )
{
  color.xyz = (normal.xyz + float3(1,1,1)) / 2.0;
  color.w = 1;
}



////////////////////////////////////////////////////////////////////////////////
void tangent_vs(	float4 position : POSITION,
		              float4 tangent   : TANGENT,
		              out float4 oPosition  : SV_Position,
		              out float4 oTangent    : TEXCOORD0,
		              uniform float4x4 worldViewProj
		           )
{
  oPosition = mul(worldViewProj, position);
  oTangent = tangent;
}
////////////////////////////////////////////////////////////////////////////////
void tangent_ps( float4 sPosition   : SV_Position,
                float4 tangent : TEXCOORD0,
                out float4 color     : SV_Target
              )
{
  color.xyz = (tangent.xyz + float3(1,1,1)) / 2.0;
  color.w = 1;
}


////////////////////////////////////////////////////////////////////////////////
void texCoord_vs(	float4 position : POSITION,
		              float2 texCoord   : TEXCOORD0,
		              out float4 oPosition  : SV_Position,
		              out float2 oTexCoord    : TEXCOORD0,
		              uniform float4x4 worldViewProj
		           )
{
  oPosition = mul(worldViewProj, position);
  oTexCoord = texCoord;
}
////////////////////////////////////////////////////////////////////////////////
void texCoord_ps( float4 sPosition   : SV_Position,
            float2 texCoord : TEXCOORD0,
            out float4 color     : SV_Target
          )
{
  color.xyz = float3(texCoord,0);
  color.w = 1;
}


////////////////////////////////////////////////////////////////////////////////
void texCoord2_vs(	float4 position : POSITION,
		              float2 texCoord2   : TEXCOORD1,
		              out float4 oPosition  : SV_Position,
		              out float2 oTexCoord2    : TEXCOORD0,
		              uniform float4x4 worldViewProj
		           )
{
  oPosition = mul(worldViewProj, position);
  oTexCoord2 = texCoord2;
}
////////////////////////////////////////////////////////////////////////////////
void texCoord2_ps( float4 sPosition   : SV_Position,
            float2 texCoord2 : TEXCOORD1,
            out float4 color     : SV_Target
          )
{
  color.xyz = float3(texCoord2,0);
  color.w = 1;
}








////////////////////////////////////////////////////////////////////////////////
void color_vs(	float4 position : POSITION,
	              float4 color   : COLOR,
	              out float4 oPosition  : SV_Position,
	              out float4 oColor    : TEXCOORD0,
	              uniform float4x4 worldViewProj
	           )
{
  oPosition = mul(worldViewProj, position);
  oColor = color;
}
////////////////////////////////////////////////////////////////////////////////
void color_ps( 	float4 sPosition   : SV_Position,
                	float4 color : TEXCOORD0,
                	out float4 oColor     : SV_Target
              )
{
  oColor = float4(color.wzy, 1.0) * color.x;							//abgr (sadly, when I use dbxv2 shader, it's seam to have the right color order : RGBA, but it's not the case for hlsl in clasical Ogre way of doing, it's ABGR)
}
////////////////////////////////////////////////////////////////////////////////
void color_RGB_ps( 	float4 sPosition   : SV_Position,
                	float4 color : TEXCOORD0,
                	out float4 oColor     : SV_Target
              )
{
  oColor = float4(color.wzy, 1.0);												//abgr (sadly, when I use dbxv2 shader, it's seam to have the right color order : RGBA, but it's not the case for hlsl in clasical Ogre way of doing, it's ABGR)
}
////////////////////////////////////////////////////////////////////////////////
void color_A_ps( 	float4 sPosition   : SV_Position,
                	float4 color : TEXCOORD0,
                	out float4 oColor     : SV_Target
              )
{
  oColor = float4(color.x, color.x, color.x, 1.0);			// abgr (sadly, when I use dbxv2 shader, it's seam to have the right color order : RGBA, but it's not the case for hlsl in clasical Ogre way of doing, it's ABGR)
}


uniform sampler2D rttAll : register(s0);

////////////////////////////////////////////////////////////////////////////////
void symmetrySolved_ps(  	float2 texCoord 			: TEXCOORD0,
				                	out float4 oColor     : COLOR
				              )
{
	oColor = tex2D(rttAll, float2(texCoord.x, texCoord.y) );
}






////////////////////////////////////////////////////////////////////////////////
void testOpacity_vs(	float4 position : POSITION,
				              out float4 oPosition  : SV_Position,
				              uniform float4x4 worldViewProj
				           )
{
  oPosition = mul(worldViewProj, position);
}
////////////////////////////////////////////////////////////////////////////////
void testOpacity_ps( 	float4 sPosition   : SV_Position,
		                	out float4 oColor     : SV_Target
		              )
{
  oColor = float4(1.0, 0.0, 0.0, 0.5);
}











////////////////////////////////////////////////////////////////////////////////
//version test to try to understand sauzer sword display
void test_vs(	float4 position : POSITION,
							float4 normal : NORMAL,
							float2 texCoord : TEXCOORD,
							float4 vertexColor : COLOR,
							
							out float4 oPosition  : SV_Position,
							out float4 oColor  		: TEXCOORD0,
							out float4 oTexCoord2	: TEXCOORD2,
							out float4 oTexCoord3	: TEXCOORD3,
							out float4 oTexCoord4	: TEXCOORD4,
							out float4 oTexCoord5	: TEXCOORD5,
							out float4 oTexCoord6	: TEXCOORD6,
							out float4 oTexCoord7	: TEXCOORD7,
							out float4 oTexCoord8	: TEXCOORD8,
							
              uniform float4x4 g_mWVP_VS,
              uniform float4x3 g_mWV_VS,
              
              uniform float4 g_vTexTile01_VS,
              
              uniform float4 g_TexScroll0_VS,
              uniform float4 g_TexScroll1_VS,
              
              uniform float4 g_MaterialCol0_VS,
              uniform float4 g_MaterialCol1_VS,
              uniform float4 g_MaterialCol2_VS,
              uniform float4 g_MaterialCol3_VS,
              
              uniform float4 g_Incidence_VS,
              uniform float4 g_GlareCoeff_VS,
              
              uniform float g_bVersatile0_VS,
              uniform float g_bVersatile1_VS,
              uniform float g_bVersatile2_VS,
              uniform float g_bVersatile3_VS
           )
{
  
	////////////////////////////////////////////
	
	// mov r0.xyz, v0.xyzx
	// mov r0.w, l(1.000000)
	// dp4 o0.x, r0.xyzw, cb0[0].xyzw
	// dp4 o0.y, r0.xyzw, cb0[1].xyzw
	// dp4 o0.z, r0.xyzw, cb0[2].xyzw
	// dp4 o0.w, r0.xyzw, cb0[3].xyzw
	oPosition = mul( g_mWVP_VS, position );								// vertex position from Object's Space to World's Space.  
	
	
	////////////////////////////////////////////
	
	//mov o1.xyzw, v3.xyzw
	oColor = vertexColor;																																	//vertex color.
	
	
	////////////////////////////////////////////
	
	//mad r0.x, v2.x, cb1[1].x, cb3[1].x
	//mad r0.y, v2.y, cb1[1].y, -cb3[1].y
	//mad r0.z, v2.x, cb1[1].z, cb3[2].x
	//mad r0.w, v2.y, cb1[1].w, -cb3[2].y
	float4 vect1 = texCoord.xyxy * g_vTexTile01_VS + float4(g_TexScroll0_VS.x, -g_TexScroll0_VS.y, g_TexScroll1_VS.x, -g_TexScroll1_VS.y);
	
	//max r1.xyzw, r0.xyzw, l(0.000000, -1.000000, 0.000000, -1.000000)
	//min r1.xyzw, r1.xyzw, l(1.000000, 0.000000, 1.000000, 0.000000)
	float4 vect2 = min( max( vect1 , float4(0,-1,0,-1) ), float4(1,0,1,0));
	
	//movc o2.xyzw, cb8[2].wwww, r1.xyzw, r0.xyzw												//Component-wise conditional move.  https://msdn.microsoft.com/fr-fr/library/windows/desktop/hh447189(v=vs.85).aspx
	oTexCoord2 =  (g_bVersatile3_VS!=0) ? vect2 : vect1;								// in our case, VsFlag3" type="Bool" value="false" =>  using vect2
	
	
	
	////////////////////////////////////////////
	
	//dp3 r0.x, v1.xyzx, cb0[32].xyzx
	//dp3 r0.y, v1.xyzx, cb0[33].xyzx
	//dp3 r0.z, v1.xyzx, cb0[34].xyzx
	float3 vect3 = mul(g_mWV_VS, normal.xyz).xyz;								// normal from Object's Space to (may be WLPB is for World Light Projection (B ? and PM ?) screenspace if the camera was positioned/oriented from light.
	
	//dp3 r0.x, r0.xyzx, r0.xyzx
	//rsq r0.x, r0.x
	//mul r0.x, r0.x, r0.z
	float a0 = vect3.z / length(vect3);												// depend of depth.
	
	//add r0.y, cb3[18].x, l(-1.000000)
	//mad_sat r0.y, |r0.x|, cb3[18].x, -r0.y
	//float a1 = saturate( abs(a0) * g_Incidence_VS.x - (g_Incidence_VS.x - 1));
	float a1 = saturate(  1.0 - (1.0 - abs(a0)) * g_Incidence_VS.x );
	
	
	
	//add r0.xzw, cb3[7].xxyz, -cb3[8].xxyz
	//mad o3.xyz, r0.yyyy, r0.xzwx, cb3[8].xyzx
	//oTexCoord3.xyz = (g_MaterialCol2_VS.xyz - g_MaterialCol3_VS.xyz) * a1 + g_MaterialCol3_VS.xyz;
	//oTexCoord3.xyz = a1 * g_MaterialCol2_VS.xyz + (1.0 - a1) * g_MaterialCol3_VS.xyz;
	oTexCoord3.xyz = lerp(g_MaterialCol3_VS.xyz, g_MaterialCol2_VS.xyz, a1);
	
	
	//add_sat r0.x, |r0.x|, cb3[18].y
	float a2 = saturate( abs(a0) + g_Incidence_VS.y ); 
	
	//mul r0.x, r0.x, r0.x
	//mul r0.x, r0.x, r0.x
	//mul o3.w, r0.x, r0.x
	oTexCoord3.w = pow(a2, 8);
	
	
	////////////////////////////////////////////
	
	//mov o4.xyzw, cb3[5].xyzw
	oTexCoord4 = g_MaterialCol0_VS;
	
	////////////////////////////////////////////
	
	//add r0.x, cb3[5].w, l(-1.000000)
	//mad o5.w, v3.x, r0.x, l(1.000000)
	//mov o5.xyz, l(0,0,0,0)
	//oTexCoord5 = float4(0, 0, 0, vertexColor.x * (g_MaterialCol0_VS.w - 1.0) + 1.0);
	oTexCoord5 = float4(0, 0, 0, lerp(1.0, g_MaterialCol0_VS.w, vertexColor.x));		//lerp / transparent composition depend of vertexColor.x, affect 0 -> g_MaterialCol0_VS.w 
	
	////////////////////////////////////////////
	
	//mov o6.xyz, l(0,0,0,0)
	//mov o6.w, cb3[6].w
	oTexCoord6 = float4( 0, 0, 0, g_MaterialCol1_VS.w);
	
	////////////////////////////////////////////
	
	//movc o7.xyz, cb8[2].xyzx, l(1.000000,1.000000,1.000000,0), l(0,0,0,0)
	//mov o7.w, l(0)
	oTexCoord7 = float4( ((g_bVersatile0_VS != 0) ? 1 : 0), ((g_bVersatile1_VS != 0) ? 1 : 0), ((g_bVersatile2_VS != 0) ? 1 : 0) , 0); //some kind of color filter ?
	
	
	////////////////////////////////////////////
	
	//mov o8.xyzw, cb3[21].xyzw
	oTexCoord8 = g_GlareCoeff_VS;

}
////////////////////////////////////////////////////////////////////////////////
void test_ps( 	float4 sPosition   : SV_Position,
								float4 vertexColor : TEXCOORD0,
								float4 texCoord2   : TEXCOORD2,
								float4 texCoord3   : TEXCOORD3,
								float4 texCoord4   : TEXCOORD4,
								float4 texCoord5   : TEXCOORD5,
								float4 texCoord6   : TEXCOORD6,
								float4 texCoord7   : TEXCOORD7,
								float4 texCoord8   : TEXCOORD8,
								
		            out float4 oColor     : SV_Target,
		            
		            uniform sampler2D sampler0 : register(s0),
								    	
		            uniform float4 g_vTone_PS,
		            uniform float g_bOutputGlareMRT_PS,
		            
		            
		            //doublons mais pas grave
		            uniform float4 g_MaterialCol0_VS,
              	uniform float4 g_MaterialCol1_VS,
              	uniform float g_bVersatile0_VS,
              	uniform float g_bVersatile1_VS
		        )
{
  

	//mul r0.xyz, v1.xyzx, v4.xyzx
	float3 color1 = vertexColor.xyz * g_MaterialCol0_VS.xyz;
	
	//sample_indexable(texture2d)(float,float,float,float) r1.xyzw, v2.xyxx, t0.xyzw, s0
	float4 color_diffuse = tex2D( sampler0, texCoord2.xy ); 
	
	//mad r2.xyz, v4.xyzx, v1.xyzx, r1.xyzx
	//float3 color2 = color1 + color_diffuse;
	
	
	//mad r0.xyz, r1.xyzx, r0.xyzx, -r2.xyzx
	//float3 color3 = color_diffuse * color1 - (color1 + color_diffuse);
	float3 color3 = -( color1 +  (1.0 - color1) * color_diffuse.xyz);										//lerp depend of color1
	
	//mad r0.xyz, v7.xxxx, r0.xyzx, r2.xyzx
	//add r0.xyz, -r1.xyzx, r0.xyzx
	//mad r0.xyz, v1.xxxx, r0.xyzx, r1.xyzx
	float3 color4 = (color3 * g_bVersatile0_VS + color1) * vertexColor.x + color_diffuse.xyz; // here also a sort of lerp with color.x.
	
	
	//mul r0.w, r1.w, v5.w
	//mul r0.w, r0.w, v1.w
	//mul r0.w, r0.w, v6.w
	//mul r1.w, r0.w, v3.w
	//float a0 = color_diffuse.w * texCoord5.w * color.w * g_MaterialCol1_VS.w * texCoord3.w;				// oTexCoord5.w is a lerp with color.x, oTexCoord3.w depend of the depth from the light
	float a0 = color_diffuse.w * ((1.0 - vertexColor.x) + vertexColor.x * g_MaterialCol0_VS.w) * vertexColor.w * g_MaterialCol1_VS.w * texCoord3.w;
	
	
	
	//add r2.xyz, r0.xyzx, v3.xyzx
	//mad r0.xyz, r0.xyzx, v3.xyzx, -r2.xyzx
	//mad r1.xyz, v7.yyyy, r0.xyzx, r2.xyzx
	//mov o0.xyzw, r1.xyzw
	//oColor = float4( (color4 * texCoord3.xyz - (color4 + texCoord3.xyz)) * g_bVersatile1_VS  + (color4 + texCoord3.xyz), a0);
	oColor = float4( (color4 + texCoord3.xyz) - (color4  + (1.0 - color4) * texCoord3.xyz) * g_bVersatile1_VS , a0);
	
	
	
	//oColor = oColor * 0.0000001 + color_diffuse;
	//oColor = oColor * 0.0000001 + vertexColor.xyzw;
	//oColor = oColor * 0.0000001 + float4(vertexColor.w, vertexColor.w, vertexColor.w, 1.0);
	//oColor = oColor * 0.0000001 + float4(vertexColor.x, vertexColor.x, vertexColor.x, 1.0);
	//oColor = oColor * 0.0000001 + float4(color4, 1.0);
	//oColor = oColor * 0.0000001 + float4(a0, a0, a0, 1.0);			// swd_b est forcement transparent hors des bordures. comme si le tranchant n'etait que la partie visible.
	//oColor.w = 1.0; 
	
	
	
	////////////////////////////// Glare part
	//mad r0.xyz, r1.xyzx, v8.xyzx, -cb0[7].wwww
	//mul r0.xyz, r0.xyzx, cb0[7].xyzx
	//movc o1.xyz, cb8[0].wwww, r0.xyzx, l(0,0,0,0)
	//mov o1.w, r1.w
	//oColor1 = float4( ( ( g_bOutputGlareMRT_PS != 0 ) ?  ((oColor * g_GlareCoeff_VS - g_vTone_PS.w) * g_vTone_PS.xyz) : 0.0) , a0);
}


