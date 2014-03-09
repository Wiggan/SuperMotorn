#include "common.fx"

VOut VS(VIn input) {
	VOut output;
	output.positionH 	= float4(input.positionL, 1.0f);
	output.positionW 	= input.positionL.xyz;
	output.uv 			= float2(input.uv.x, -input.uv.y); 
	output.normalW 		= float4(input.positionL, 1.0f);
	return output;
}



