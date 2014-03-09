#include "common.fx"

float4 PS(VOut input) : SV_Target {
	return (gDiffuseTexture.Sample(samplerState0, input.uv) + float4(0.3f, 0.3f, 0.3f, 1.0f)) * gGlowTexture.Sample(samplerState0, input.uv);
}



