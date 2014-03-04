#include "common.fx"

VOut VS(VIn input) {
	VOut output;
	output.positionH 	= mul(float4(input.positionL, 1.0f), gWorld);
	output.positionW 	= output.positionH.xyz;
	output.positionH 	= mul(output.positionH, gView);
	output.positionH 	= mul(output.positionH, gProjection);
	output.uv 			= float2(input.uv.x, -input.uv.y); 
	output.normalW 		= normalize(mul(float4(input.normal, 0.0f), gWorld));
	return output;
}



