



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
  oColor = color;
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







