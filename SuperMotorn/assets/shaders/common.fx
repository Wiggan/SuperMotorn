struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};
struct DirectionalLight 
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
};
struct PointLight 
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float	pad;
	float3 attenuation;
	float range;
};
cbuffer cbPerObject : register(b0) {
	float4x4 gWorld;
	Material gMaterial;
	float3	 gColor;
	int	 	 gUseDiffuseMap;
	int	 	 gUseSpecularMap;
	int	 	 gUseGlossMap;
	int	 	 gUseGlowMap;
	float	 pad1234;
};
cbuffer cbPerFrame : register(b1) {
	float4x4 	gView;
	float4x4 	gProjection;
	float3	 	gCameraPosition;
	int		 	gPointLightCount;
	PointLight 	gPointLights[10];
};
cbuffer cbSeldom : register(b2) {
	DirectionalLight 	gDirectionalLight;
	float4 				gFogColor;
	float				gFogStart;
	float				gFogRange;
	int					gUseSkyColor;
};
TextureCube gCubeMap : register( t0 );
Texture2D gDiffuseTexture : register( t1 );
Texture2D gSpecularTexture : register( t2 );
Texture2D gGlossTexture : register( t3 );
Texture2D gGlowTexture : register( t4 );
Texture2D gInput1 : register( t5 );
Texture2D gInput2 : register( t6 );
Texture2D gComputeResult : register( t7 );
RWTexture2D<float4> gOutput;
SamplerState samplerState0 : register( s0 );
struct VOut
{
	float4 positionH : SV_POSITION;
	float3 positionW : POSITION;
	float2 uv : TEXCOORD0;
	float4 normalW : TEXCOORD1;
	float3 color : COLOR;
};
struct VIn
{
	float3 positionL : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
	float2 uv : TEX0;
	float2 uv2 : TEX1;
};
struct POut
{
	float4 diffuse: SV_Target0;
    float4 glow: SV_Target1;
};
