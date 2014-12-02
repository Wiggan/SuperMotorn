#include "common.fx"
POut PS(VOut input) : SV_Target {
	float3 toCamera = normalize(gCameraPosition - input.positionW);
	float distToCamera = length(toCamera);
	float4 texColor = gColor;
	if( gUseDiffuseMap == 1 ) {
		texColor *= gDiffuseTexture.Sample(samplerState0, input.uv);
	}
	float4 color = texColor;
	float fogLerp = saturate((distToCamera - gFogStart) / gFogRange);
	if ( gUseSkyColor ) {
		color = lerp(color, gCubeMap.Sample(samplerState0, input.positionW), fogLerp);
	} else {
		color = lerp(color, gFogColor, fogLerp);
	}
	color.a = texColor.a;
	float4 glow = float4(0.0f, 0.0f, 0.0f, 0.0f); // gColor;
	if(gUseGlowMap) {
		glow = gGlowTexture.Sample(samplerState0, input.uv);
	} 
	glow.a = texColor.a;
	POut output;
	output.diffuse = color;
	output.glow = glow;
	return output;
}



