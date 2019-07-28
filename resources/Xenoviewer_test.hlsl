



void	test_cell_vs(
					float3 position		: POSITION,
					float3 normal		: NORMAL,
					float3 tangent		: TANGENT,
					float2 texCoord0	: TEXCOORD,
					float2 texCoord1	: TEXCOORD1,
					out		float4 oPos			: SV_POSITION,
					//out		float4 oPos : POSITION,					//for cg debug (nvidia cgc)
					out		float2 oTexCoord0	: TEXCOORD,
					out		float2 oTexCoord1	: TEXCOORD1,
					out		float3 oTBN_H		: TEXCOORD2,
					out		float  oFog			: FOG,
					out		float4 oWLPB_SMpos	: TEXCOORD4,
					out		float3 oDistanceCameraZ : TEXCOORD5,
					uniform float4x4 g_mWVP_VS,
					uniform float4x4 g_mWLPB_SM_VS,
					uniform float4x3 g_mWV_VS,
					uniform float4x3 g_mW_VS,
					uniform float4 g_vTexTile01_VS,
					uniform float4 g_vHeightFog_VS,
					uniform float4 g_vFog_VS,
					uniform float4 g_vLightVec0_VS,								//allready on object space apparently.
					uniform float4 g_vEyePos_VS
					)
{
	// r0 = v0 = pos 
	// v1 = normal
	// v2 = tangent
	// v3 = texCoord0
	// v4 = texCoord1

	// o0 = oPos
	// o1_0 = oTexCoord0
	// o1_1 = oTexCoord1
	// o2_1 = oFog
	// o2_0 = oTBN_H		//specular_factor relative to local Tangent-Binormal-Normal repere
	// o3 = oWLPB_SMpos		// position after multiplication with matrice WorldLightProjectionB?????_ShadowMap ?
	// o4 = oDistanceCameraZ

	// r1.yzw = binormal
	// r2 = V (vector camera -> vertexPosition)



	//mov r0.xyz, v0.xyzx
	//mov r0.w, l(1.000000)
	float4 pos = float4(position.xyz, 1);


	//dp4 o0.x, r0.xyzw, cb0[0].xyzw
	//dp4 o0.y, r0.xyzw, cb0[1].xyzw
	//dp4 o0.z, r0.xyzw, cb0[2].xyzw
	//dp4 r1.x, r0.xyzw, cb0[3].xyzw
	//mov o0.w, r1.x
	oPos = mul(g_mWVP_VS, pos);

	//mul o1.xy, v3.xyxx, cb1[1].xyxx
	//mul o1.zw, v4.xxxy, cb1[1].zzzw
	oTexCoord0 = texCoord0.xy * g_vTexTile01_VS.xy;
	oTexCoord1 = texCoord1.xy * g_vTexTile01_VS.zw;


	//mul r1.yzw, v1.zzxy, v2.yyzx
	//mad r1.yzw, v1.yyzx, v2.zzxy, -r1.yyzw
	//dp3 r2.x, r1.yzwy, r1.yzwy
	//rsq r2.x, r2.x
	//mul r1.yzw, r1.yyzw, r2.xxxx
	////r1.yzw = (normal.yzx * tangent.zxy) - (normal.zxy * tangent.yzx);
	////r2.x = dot(r1.yzw,r1.yzw);
	////r2.x =  1.0/sqrt(r2.x) ;
	float3 binormal = normalize(cross(normal, tangent));

	//add r2.xyz, -v0.xyzx, cb2[17].xyzx
	//dp3 r2.w, r2.xyzx, r2.xyzx
	//rsq r2.w, r2.w
	//mad r2.xyz, r2.xyzx, r2.wwww, cb2[5].xyzx
	//dp3 r2.w, r2.xyzx, r2.xyzx
	//rsq r2.w, r2.w
	//mul r2.xyz, r2.wwww, r2.xyzx
	float3 V = normalize(g_vEyePos_VS.xyz - pos.xyz);		// vector camera -> vertexPosition
	float3 H = normalize(V + g_vLightVec0_VS.xyz);		// specular_factor

	//dp3 o2.x, v2.xyzx, r2.xyzx
	//dp3 o2.y, r1.yzwy, r2.xyzx
	//dp3 o2.z, v1.xyzx, r2.xyzx
	oTBN_H = float3(dot(tangent, H), dot(binormal, H), dot(normal, H));		//position relative to local Tangent-Binormal-Normal repere



	//////////////// Fog
	//dp4 r1.y, r0.xyzw, cb0[37].xyzw
	//mad_sat r1.y, r1.y, cb1[10].y, cb1[10].x
	////r1.y = dot(pos, g_mW_VS._m01_m11_m21_m31);
	float fog_wHeight = saturate(mul(g_mW_VS, pos.xyz).y * g_vHeightFog_VS.y + g_vHeightFog_VS.x);	//fog factor depend of height of vertex in world space (maximum fog at 1.0, minimum 0 because of saturate).
	//mad_sat r1.z, r1.x, cb1[10].w, cb1[10].z
	//mad_sat r1.x, r1.x, cb1[11].w, cb1[11].z
	//add r1.y, r1.z, r1.y
	//min r1.y, r1.y, l(1.000000)
	//mul o2.w, r1.y, r1.x
	float fog_height_offset = saturate(oPos.w * g_vHeightFog_VS.w + g_vHeightFog_VS.z);		// fog factor depend of oPos.w (normally == 1)	=> Ogre use fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_FOG_PARAMS);, so could have trouble on it.
	float fog_factor = saturate(oPos.w * g_vFog_VS.w + g_vFog_VS.z);						// fog factor depend of oPos.w (normally == 1) , with other parameter.
	oFog = min(fog_wHeight + fog_height_offset, 1.0) * fog_factor;


	//dp4 o3.x, r0.xyzw, cb0[20].xyzw
	//dp4 o3.y, r0.xyzw, cb0[21].xyzw
	//dp4 o3.z, r0.xyzw, cb0[22].xyzw
	//dp4 o3.w, r0.xyzw, cb0[23].xyzw
	oWLPB_SMpos = mul(g_mWLPB_SM_VS, pos);										// position after multiplication with matrice WorldLightProjectionB?????_ShadowMap ?


	//mov o4.xy, l(0,0,0,0)
	//dp4 o4.z, r0.xyzw, cb0[34].xyzw
	oDistanceCameraZ = float3(0, 0, mul(g_mWV_VS, pos.xyz).z);									//distance from camera (no projection) of the vertex

	return;
}





























//strange MatSpcRBG in .emm is not used

void	test_cell_ps(
						float4 pos				: SV_POSITION,
						//float4 pos				: POSITION,					//for cg debug (nvidia cgc)
						float2 texCoord0		: TEXCOORD,				//previously float4 (but not needed)
						float2 texCoord1		: TEXCOORD1,
						float4 TBN_H			: TEXCOORD2,
						float fog				: FOG,
						float4 WLPB_SMpos		: TEXCOORD4,
						float3 distanceCameraZ	: TEXCOORD5,
						out		float4 oColor	: SV_TARGET,
						out		float4 oColor1	: SV_TARGET1,
						//out		float4 oColor : COLOR,					//for cg debug (nvidia cgc)
						//out		float4 oColor1 : COLOR1,
						uniform float4		g_vShadowMap_PS,
						uniform float4		g_vShadowColor_PS,
						uniform float4		g_vTone_PS,
						uniform float4		g_vFogMultiColor_PS,
						uniform float4		g_vFogAddColor_PS,
						uniform float4		g_vSpecular_PS,
						uniform float4		g_vShadowParam_PS,
						uniform float4		g_vLightSpc0_PS,
						uniform bool		g_bFog_PS,
						uniform bool		g_bOutputGlareMRT_PS,
						uniform sampler2D		Texture_ImageSampler0 :	register(s0),
						uniform sampler2D		Texture_ImageSampler1 : register(s1),
						uniform sampler2D		Texture_ImageSampler2 : register(s2),
						uniform sampler2D		Texture_SamplerShadowMap : register(s7)
						)
{
	/*
	// v0 = pos
	// v1 = texCoord0
	// v1_1 = texCoord1
	// v2 = TBN_H
	// v2_1 = fog
	// v3 = WLPB_SMpos
	// v4 = distanceCameraZ

	// o0 = oColor
	// o1 = oColor1
	*/


	////////////////////////// ShadowMap:  many samples around pixel in shadowMap texture to avoid flicking/filtering.
	//mul r0.x, v3.w, cb0[0].y
	//mul r2.x, r0.x, cb0[0].w
	//mad r0.yz, r0.xxxx, cb0[0].wwww, v3.xxyx
	//div r0.yz, r0.yyzy, v3.wwww
	//mad r0.w, -cb0[0].z, v3.w, v3.z
	//div_sat r0.w, r0.w, v3.w
	//sample_c_indexable(texture2d)(float,float,float,float) r1.x, r0.yzyy, t7.xxxx, s7, r0.w
	float sm_delta_uv = (WLPB_SMpos.w * g_vShadowMap_PS.y) * g_vShadowMap_PS.w;						// g_vShadowMap_PS and g_vShadowColor_PS always to 0 by xenoviewer
	float sm_precision_uv = saturate((-g_vShadowMap_PS.z * WLPB_SMpos.w + WLPB_SMpos.z) / WLPB_SMpos.w);

	float2 texCoord_sm = float2(WLPB_SMpos.x + sm_delta_uv, WLPB_SMpos.y + sm_delta_uv) / WLPB_SMpos.w;
	float sm_factor_0 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;			//possible error , missing r0.w (sm_precision_uv) parameter // always empty.png (black full transparent) in xenoviewer

	//mad r0.yz, -r0.xxxx, cb0[0].wwww, v3.xxyx
	//div r0.xy, r0.yzyy, v3.wwww
	//sample_c_indexable(texture2d)(float,float,float,float) r1.w, r0.xyxx, t7.xxxx, s7, r0.w
	texCoord_sm = float2(WLPB_SMpos.x - sm_delta_uv, WLPB_SMpos.y - sm_delta_uv) / WLPB_SMpos.w;			//look like take a sample with delta diff.
	float sm_factor_3 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;			//possible error , missing r0.w (sm_precision_uv) parameter 

	//mov r2.yw, -r2.xxxx
	//add r3.xyzw, r2.yxxy, v3.xyxy
	//div r3.xyzw, r3.xyzw, v3.wwww
	//sample_c_indexable(texture2d)(float,float,float,float) r1.y, r3.xyxx, t7.xxxx, s7, r0.w
	//sample_c_indexable(texture2d)(float,float,float,float) r1.z, r3.zwzz, t7.xxxx, s7, r0.w
	texCoord_sm = float2(WLPB_SMpos.x - sm_delta_uv, WLPB_SMpos.y + sm_delta_uv) / WLPB_SMpos.w;
	float sm_factor_1 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;
	texCoord_sm = float2(WLPB_SMpos.x + sm_delta_uv, WLPB_SMpos.y - sm_delta_uv) / WLPB_SMpos.w;
	float sm_factor_2 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;

	//mov r2.z, l(0)
	//mad r0.yz, r2.zzxz, l(0.000000, 1.000000, -1.000000, 0.000000), v3.xxyx
	//div r0.yz, r0.yyzy, v3.wwww
	//sample_c_indexable(texture2d)(float,float,float,float) r2.w, r0.yzyy, t7.xxxx, s7, r0.w
	texCoord_sm = float2(WLPB_SMpos.x, WLPB_SMpos.y - sm_delta_uv) / WLPB_SMpos.w;
	float sm_factor_8 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;

	//add r2.xy, r2.wzww, v3.xyxx	(r2.w = -sm_delta_uv)
	//div r2.xy, r2.xyxx, v3.wwww
	//sample_c_indexable(texture2d)(float,float,float,float) r2.z, r2.xyxx, t7.xxxx, s7, r0.w
	texCoord_sm = float2(WLPB_SMpos.x - sm_delta_uv, WLPB_SMpos.y) / WLPB_SMpos.w;
	float sm_factor_7 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;

	//add r1.xyzw, r2.xzzx, v3.xyxy (r2.z = 0;)
	//div r1.xyzw, r1.xyzw, v3.wwww
	//sample_c_indexable(texture2d)(float,float,float,float) r2.x, r1.xyxx, t7.xxxx, s7, r0.w
	//sample_c_indexable(texture2d)(float,float,float,float) r2.y, r1.zwzz, t7.xxxx, s7, r0.w
	texCoord_sm = float2(WLPB_SMpos.x + sm_delta_uv, WLPB_SMpos.y) / WLPB_SMpos.w;
	float sm_factor_5 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;
	texCoord_sm = float2(WLPB_SMpos.x, WLPB_SMpos.y + sm_delta_uv) / WLPB_SMpos.w;
	float sm_factor_6 = tex2D(Texture_SamplerShadowMap, texCoord_sm).r;

	//dp4 r0.x, r1.xyzw, l(0.125000, 0.125000, 0.125000, 0.125000)
	//dp4 r0.y, r2.xyzw, l(0.125000, 0.125000, 0.125000, 0.125000)
	//add r0.x, r0.y, r0.x
	float sm_factor = dot(float4(sm_factor_0, sm_factor_1, sm_factor_2, sm_factor_3), float4(0.125000, 0.125000, 0.125000, 0.125000)) + dot(float4(sm_factor_5, sm_factor_6, sm_factor_7, sm_factor_8), float4(0.125000, 0.125000, 0.125000, 0.125000).xyzw);

	//add r0.yzw, -cb0[6].xxyz, l(0.000000, 1.000000, 1.000000, 1.000000)
	//mad r0.xyz, r0.xxxx, r0.yzwy, cb0[6].xyzx
	//add r1.xyz, -r0.xyzx, l(1.000000, 1.000000, 1.000000, 0.000000)
	////r1.xyz = float3(1.000000, 1.000000, 1.000000) - (sm_factor * (float3(1.000000, 1.000000, 1.000000) - g_vShadowColor_PS.xyz) + g_vShadowColor_PS.xyz);
	float3 shadow_color_factor = (float3(1.000000, 1.000000, 1.000000) - g_vShadowColor_PS.xyz) * (1 - sm_factor);			//=> always 1 in xenoviewer 

	//add r0.w, -cb5[0].x, cb5[0].y
	//div r0.w, l(1.000000, 1.000000, 1.000000, 1.000000), r0.w
	//add r1.w, v4.z, -cb5[0].x
	//mul_sat r0.w, r0.w, r1.w
	//mad r1.w, r0.w, l(-2.000000), l(3.000000)
	//mul r0.w, r0.w, r0.w
	//mul r0.w, r0.w, r1.w
	//mad r0.xyz, r0.wwww, r1.xyzx, r0.xyzx
	float shadow_dist_factor = pow(saturate((distanceCameraZ.z - g_vShadowParam_PS.x) / (g_vShadowParam_PS.y - g_vShadowParam_PS.x)), 3) * -2.000000 + 3.000000 * pow(saturate((distanceCameraZ.z - g_vShadowParam_PS.x) / (g_vShadowParam_PS.y - g_vShadowParam_PS.x)), 2);		//could have infinite in xenoviewer because division by 0. if not the result is 3, else is 1.0
	float3 shadow_color = shadow_color_factor * shadow_dist_factor + (float3(1.000000, 1.000000, 1.000000) - g_vShadowColor_PS.xyz) * sm_factor + g_vShadowColor_PS.xyz; //all white in xenoviewer ? => ok , it's 0 for shadow, so if you multiply by diffuse color , you have the result of shadows, so it's Ok).







	////////////////////////// Diffuse
	//sample_indexable(texture2d)(float,float,float,float) r2.xyzw, v1.zwzz, t1.xyzw, s1
	//sample_indexable(texture2d)(float,float,float,float) r3.xyzw, v1.xyxx, t0.xyzw, s0
	//mul r2.xyz, r2.xyzx, r3.xyzx
	float4 dif_main_color = tex2D(Texture_ImageSampler1, texCoord1);				//in the case of floor05 BFcel01 it's the texture 31 with general color.
	float4 dif_detail_color = tex2D(Texture_ImageSampler0, texCoord0);				//it's the diffuse color for details (generally with repetition)
	float3 dif_color = dif_main_color.xyz * dif_detail_color.xyz;



	////////////////////////// NormalMap and specular : get a new normal with normalmap texture information (relative to TangentBinormalNormal (local to vertex) space)
	//sample_indexable(texture2d)(float,float,float,float) r1.xyz, v1.xyxx, t2.xyzw, s2
	//mad r1.xyz, r1.xyzx, l(2.000000, 2.000000, 2.000000, 0.000000), l(-1.000000, -1.000000, -1.000000, 0.000000)
	//dp3 r0.w, r1.xyzx, r1.xyzx
	//rsq r0.w, r0.w
	//mul r1.xyz, r0.wwww, r1.xyzx
	float3 normal_details_inTBN = normalize(tex2D(Texture_ImageSampler2, texCoord0).xyz * float3(2.000000, 2.000000, 2.000000) + float3(-1.000000, -1.000000, -1.000000)); // Texture_ImageSampler2 is the normalmap of Texture_ImageSampler0 (details), [0,1] => [-1, 1] + normalize. that give new normal for details in TBN (local to the vertex) space.

	//dp3 r0.w, v2.xyzx, v2.xyzx
	//rsq r0.w, r0.w
	//mul r2.xyz, r0.wwww, v2.xyzx	
	float3 TBN_H_norm = normalize(TBN_H.xyz);

	//dp3_sat r0.w, r1.xyzx, r2.xyzx
	//log r0.w, r0.w
	//mul r0.w, r0.w, cb2[18].y
	//exp r0.w, r0.w
	//mul r0.w, r0.w, cb2[18].x
	//mul r1.xyz, r0.wwww, cb2[7].xyzx
	float spec_factor = dot(normal_details_inTBN, TBN_H_norm);		// normal for details in worldspace.
	float3 spec_color_tmp = (pow(2, log2(saturate(spec_factor))  * g_vSpecular_PS.y)  * g_vSpecular_PS.x) * g_vLightSpc0_PS.xyz;		//change light specular from normalmap details.

	//mul r1.xyz, r1.xyzx, r2.wwww
	//mul r1.xyz, r1.xyzx, r3.wwww
	float3 spec_color = spec_color_tmp * dif_main_color.w * dif_detail_color.w;


	//mad r0.xyz, r2.xyzx, r0.xyzx, r1.xyzx
	float3 color_base = dif_color * shadow_color + spec_color;					// Note : no shadow give shadow_color all white.






	////////////////////////// Fog
	//mad r1.xyz, cb0[8].xyzx, r0.xyzx, cb0[9].xyzx
	//add r2.xyz, r0.xyzx, -r1.xyzx
	//mad r1.xyz, v2.wwww, r2.xyzx, r1.xyzx
	////float3 color_withFog = fog * (color_base - (g_vFogMultiColor_PS.xyz * color_base + g_vFogAddColor_PS.xyz)) + (g_vFogMultiColor_PS.xyz * color_base + g_vFogAddColor_PS.xyz);
	float3 color_withFog = lerp(g_vFogMultiColor_PS.xyz * color_base + g_vFogAddColor_PS.xyz, color_base, fog);		//in xenoviewer, bey default g_vFogMultiColor_PS is white and g_vFogAddColor_PS is black. => we just have color_base

	//movc o0.xyz, cb8[0].zzzz, r1.xyzx, r0.xyzx
	//mov o0.w, l(1.000000)
	oColor = float4(((g_bFog_PS) ? color_withFog : color_base), 1.0);





	////////////////////////// Glares
	//add r0.x, r0.y, r0.x
	//add r0.x, r0.z, r0.x
	//add r0.x, r0.x, -cb0[7].w
	//mul r0.xyz, r0.xxxx, cb0[7].xyzx
	//movc o1.xyz, cb8[0].wwww, r0.xyzx, l(0,0,0,0)
	//mov o1.w, l(1.000000)
	float3 glare_color = (color_base.x + color_base.y + color_base.z - g_vTone_PS.w) * g_vTone_PS.xyz;		// xenoviewr default : g_vTone_PS is 1, and g_bOutputGlareMRT_PS = 0, and oColor1 is never used.
	oColor1 = float4(((g_bOutputGlareMRT_PS) ? glare_color : float3(0, 0, 0)), 1.0);


	//oColor = oColor * 0.000000001 + float4(texCoord0, 0, 1);
	//oColor = oColor * 0.000000001 + dif_main_color;
	//oColor = oColor * 0.000000001 + tex2D(Texture_ImageSampler1, texCoord1);
	//oColor = oColor * 0.000000001 + float4(dif_color , 1);
	//oColor = oColor * 0.000000001 + tex2D(Texture_ImageSampler2, texCoord0);
	//oColor = oColor * 0.000000001 + float4(spec_factor, spec_factor, spec_factor, 1);
	//oColor = oColor * 0.000000001 + float4(spec_color_tmp, 1);
	//oColor = oColor * 0.000000001 + float4(g_vLightSpc0_PS.xyz, 1);
	//oColor = oColor * 0.000000001 + float4(spec_color, 1);
	//oColor = oColor * 0.000000001 + float4(dif_color * shadow_color, 1);
	//oColor = oColor * 0.000000001 + float4(0.5 * shadow_color, 1);
	//oColor = oColor * 0.000000001 + float4(color_base, 1);
	//oColor = oColor * 0.000000001 + float4(color_withFog, 1);
	
	
	
	
	

	return;
}
