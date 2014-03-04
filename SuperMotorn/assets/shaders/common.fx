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
	float2	pad1234;
};
cbuffer cbPerFrame : register(b1) {
	float4x4 	gView;
	float4x4 	gProjection;
	float3	 	gCameraPosition;
	int		 	gPointLightCount;
	PointLight 	gPointLights[3];
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
Texture2D gInput1 : register( t4 );
Texture2D gInput2 : register( t5 );
RWTexture2D<float4> gOutput;
SamplerState samplerState0 : register( s0 );
struct VOut
{
	float4 positionH : SV_POSITION;
	float3 positionW : POSITION;
	float2 uv : TEXCOORD0;
	float4 normalW : TEXCOORD1;
};
struct VIn
{
	float3 positionL : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEX0;
	float2 uv2 : TEX1;
};

