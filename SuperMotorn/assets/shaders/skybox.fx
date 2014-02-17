cbuffer cbPerObject : register(b0) {
	float4x4 gWorld;
	float4x4 gMaterial;
	float3	 gColor;
	int	 	 gUseTexture;
};
cbuffer cbPerFrame : register(b1) {
	float4x4 	gView;
	float4x4 	gProjection;
};
TextureCube gCubeMap : register( t0 );
SamplerState samplerState0 : register( s0 );
struct VIn
{
	float3 positionL : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEX0;
	float2 uv2 : TEX1;
};
struct VOut
{
	float4 positionH : SV_POSITION;
	float3 positionL : POSITION;
};
VOut VS(VIn input) {
	VOut output;
	output.positionH 	= mul(float4(input.positionL, 1.0f), gWorld).xyzw;
	output.positionH 	= mul(output.positionH, gView);
	output.positionH 	= mul(output.positionH, gProjection);
	output.positionL 	= input.positionL;
	return output;
}
float4 PS(VOut input) : SV_Target {
	return gCubeMap.Sample(samplerState0, input.positionL);
}



