#include "common.fx"

float4 PS(VOut input) : SV_Target {
	return gComputeResult.Sample(samplerState0, input.uv);
}



