#include "common.fx"
VOut VS(VIn input) {
	VOut output;
	output.positionH 	= mul(float4(input.positionL, 1.0f), gWorld).xyzw;
	output.positionH 	= mul(output.positionH, gView);
	output.positionH 	= mul(output.positionH, gProjection);
	output.positionW 	= input.positionL;
	output.uv 			= float2(0.0f, 0.0f); 
	output.normalW 		= float4(0,0,0,0);
	return output;
}

