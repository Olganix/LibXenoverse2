


void normal_ps( float4 sPosition   	: SV_Position,
								float3 texcoord 		:	TEXCOORD0,
								float3 normal 			:	TEXCOORD1,
                out float4 color    : SV_Target
              )
{
  color.xyz = (normal.xyz + float3(1,1,1)) / 2.0;
  color.w = 1;
}
