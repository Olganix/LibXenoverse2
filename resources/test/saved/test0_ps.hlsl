


//apparently , we have to specified registers, else you don't have anything working in game (same if in Xenoviewer, witch use names, it"s working).

uniform sampler2D ImageSampler1               : register(s1);										//black lines and line for damages
uniform sampler2D SamplerToon    							: register(s4);   								//other palette color.
uniform sampler2D ImageSamplerTemp14          : register(s14);									//palette color dyt.emb  : shaders use diffuse_factor, specular_factor, etc .. (each composante a line in texture) to get the color to use in a degraged.

uniform bool g_bOutputGlareMRT_PS							: register(b3);										//active or not the glow, by fill a the second color ouput.

uniform float4 g_vFadeMulti_PS								: register(c51);									// xyz: colorDiffuse is multiply by this to make fade color.  w : factor on composition of Fade with the rest.
uniform float4 g_vFadeRim_PS									: register(c52);									// xyz: additive value , multiply by a kind of reflect/gloss factor.
uniform float4 g_vFadeAdd_PS									: register(c53);									// xyz: additive value.

uniform float4 g_vLightVec0_PS								: register(c64);									//direction of the light (allready normalized)

uniform float4 g_MaterialCol0_PS							: register(c84);									// xyz : base of a color reducer witch depend of damageFactor (g_MaterialCol3_PS.x).
uniform float4 g_MaterialCol1_PS							: register(c85);									// xyz : second base of a color reducer witch depend of damageFactor (g_MaterialCol3_PS.x)
uniform float4 g_MaterialCol3_PS							: register(c87);									// x : factor to appear damages.

uniform float4 g_MaterialScale0_PS						: register(c92);									// major color of Glare, premultplied by the alpha. (see premultiplied https://en.wikipedia.org/wiki/Alpha_compositing, or just know color are multiply by their alpha , to have a better composition after)
uniform float4 g_MaterialScale1_PS						: register(c93);									// x: select witch part of palette texture you use (each material use is own part). for Apple, there is a texture of 128 bits of high. apparently, there is 8 parts (0.125 = 1/8), and in each part , there is 4 lines with degraded. 

uniform float4 g_vColor0_PS										: register(c105);									// color multiplier (+ premultiplied) for colorPalette0.
uniform float4 g_vColor1_PS										: register(c106);									// color multiplier (+ premultiplied) for colorPalette1.
uniform float4 g_vColor2_PS										: register(c107);									// color multiplier (+ premultiplied) for color_SamplerTemp0 (another colorPalette)
uniform float4 g_vColor12_PS									: register(c117);									// xyz : additive color for Glare.

uniform float4 g_vParam3_PS										: register(c139);									// each composant adjust some color by multiply them : x for all use of SamplerToon texture, y for ImageSamplerTemp14
uniform float4 g_vParam4_PS										: register(c140);									// xy : they are the min and max to convert distance from the [min,max] range to a factor in [0,1].
uniform float4 g_vParam5_PS										: register(c141);									// position Offset. may be to change the distance, and fading effect about that.
uniform float4 g_vParam7_PS										: register(c143);									// xy : minMax for screenPos.w.



void test0_ps( 	float4 sPosition   	: SV_Position,
								float2 texcoord 		:	TEXCOORD0,																// UV0
								float3 normal 			:	TEXCOORD1,																// normal in world space
								float3 eyePos 			:	TEXCOORD2,																// camera's position in world space
								float3 position			:	TEXCOORD4,																// position in world space
								float4 screenPos 		:	TEXCOORD6,																// position in screenSpace.
								
                out float4 oColor    : SV_Target,																// final color of the pixel
                out float4 oColor1   : SV_Target1																// color on a second buffer, for post effect, like Glares.
              )
{ 
		float uv_line0 = g_MaterialScale1_PS.x * 0.125 + 0.015625;									//selection line in degraded's texture for Diffuse0 for ImageSamplerTemp14 and SamplerToon. g_MaterialScale1_PS.x selection on of 8 part in high. it's on the middle of the first line (there is 4 lines into a part).
    float uv_line1 = g_MaterialScale1_PS.x * 0.125 + 0.046875;									//selection line in degraded's texture for Diffuse1 (x will be diffuseFactor). in the middle of the second line.
		float uv_line2 = g_MaterialScale1_PS.x * 0.125 + 0.078125;									//selection line in degraded's texture for Specular (x will be specularFactor). in the middle of the third line.


		//usefull vectors
    float3 P = (g_vParam5_PS.xyz - position);
    float distance = length(P);
    P = normalize(P);
    float3 V = normalize(eyePos);
    float3 N = normalize(normal);
    float3 H = normalize(V + g_vLightVec0_PS.xyz);															//may be for specular ?
    
    
    float distFactor = 1.0 - ( saturate( (distance - g_vParam4_PS.x) / (g_vParam4_PS.y - g_vParam4_PS.x) ) );	//distance is scaled into [0,1] range
        
    //Color from palettte
		float a14 = ((dot(N, P) + 1.0) / 2.0) * distFactor;													// will be maximise if N parallele to P (=vector center of objet -> position of the pixel). Note : "(X + 1.0) / 2.0" is for scale from [-1,1] to [0,1] whitch is more usefull for using as a textureCoordinate.
    float a11 = 1.0 - dot(V, N);																								// maximise is V paralelle to N = a kind of reflect factor ? glossFactor ? 
		float a15 = saturate( a11 ) * distFactor;																		// saturate is for clamp into [0,1] if the value is out of range.
    
		float4 color_palette0 = (g_vColor0_PS * g_vColor0_PS.w) * tex2D(ImageSamplerTemp14, float2(a14, uv_line0));		//premultiplication of g_vColor0_PS
    float4 color_palette1 = (g_vColor1_PS * g_vColor1_PS.w) * tex2D(ImageSamplerTemp14, float2(a15, uv_line1));		//premultiplication of g_vColor1_PS
    float4 colorDiffuse_base = (color_palette0 + color_palette1) * g_vParam3_PS.y;
    
		
		
    //Color from SamplerToon
    float a18 = saturate( a11 );
		float diffuseFactor = (dot(N, g_vLightVec0_PS.xyz) + 1.0)/2.0;							//  [-1,1] => [0,1]
    
		float4 color_diffuse_Toon0 = tex2D(SamplerToon, float2(a18, uv_line1));
    float4 color_diffuse_Toon1 = tex2D(SamplerToon, float2(diffuseFactor, uv_line0));
    
    float4 colorDiffuse_base1 = colorDiffuse_base + (color_diffuse_Toon0 + color_diffuse_Toon1) * g_vParam3_PS.x;
    
    
    // strokes behind diffuse
    float4 color_traits = tex2D(ImageSampler1, texcoord);												//there is different stroke into : x is for damages, w for toon/edges strokes.
    
    float a1 = saturate(  (screenPos.w - g_vParam7_PS.x) / (g_vParam7_PS.y - g_vParam7_PS.x) );		//a1, a4 depend of a kind of camera distance in screenView. 
    float a4 = -2 * pow(a1,3) + 3 * pow(a1, 2);
    
    float a9 = a4 * g_vParam7_PS.w + (1 - a4) * g_vParam7_PS.z;
    float a19 = a9 * g_MaterialScale1_PS.w + 0.5;
    color_traits.w = color_traits.w - a19;																			//toon/edges strokes are inversed (because we can't "add" black color). the base of the strenght of the color depend of a kind of camera distance in ScreenView.
    
    
    // colorReducer for appear damages.
		float3 color7_b = g_MaterialCol0_PS.xyz;
    if(color_traits.w<0)
    	color7_b = float3(1,1,1);
    float a20 = color_traits.x * g_MaterialCol3_PS.x;														//is about damages strokes
		float3 colorDiffuse_reducer = lerp( color7_b, g_MaterialCol1_PS.xyz, a20);	//because we can't "add" black color for strokes, they make a colorReducer whitch will be mulitply by diffuseColor. 
    
    
		//
		float a8 = saturate( dot( normalize( P + V ) , N) ) * distFactor;						//a kind of Scattering (light throught skin) ?
    float4 color_SamplerTemp0 = (tex2D(ImageSamplerTemp14, float2(a8, uv_line2)) * (g_vColor2_PS * g_vColor2_PS.w)) * g_vParam3_PS.y;			////premultiplication
     
		//specular
    float specularFactor = saturate( dot(H, N) );
		float4 colorSpecular = tex2D(SamplerToon, float2(specularFactor, uv_line2)); 
    
    
    //composition
    float3 color_final = colorDiffuse_base1.xyz * colorDiffuse_reducer + colorSpecular.xyz * g_vParam3_PS.x + color_SamplerTemp0.xyz;	//colorReducer will make black stroke for toon/edges and damages. 
    
    
    //fade
    float3 color_fade = color_final.xyz * g_vFadeMulti_PS.xyz + g_vFadeRim_PS.xyz * a11 + g_vFadeAdd_PS.xyz; 
		
		//Ouput Color 
		oColor.xyz = color_fade * g_vFadeMulti_PS.w + color_final.xyz * (1.0 - g_vFadeMulti_PS.w);	//composition premultiplication
		oColor.a = color_diffuse_Toon1.w;
    
    
    //Glare Color
    oColor1 = float4(0,0,0, color_diffuse_Toon1.w);
    if(g_bOutputGlareMRT_PS)
    	oColor1.xyz = g_MaterialScale0_PS.xyz * g_MaterialScale0_PS.w + g_vColor12_PS.xyz;	//premultiplication of g_MaterialScale0_PS
}






