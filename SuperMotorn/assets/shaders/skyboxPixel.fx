#include "common.fx"
float4 PS(VOut input) : SV_Target {
	return gCubeMap.Sample(samplerState0, input.positionW);
}


