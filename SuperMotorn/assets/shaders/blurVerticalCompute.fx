#include "common.fx"

[numthreads(32,32,1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID) {
	float4 color = float4(0,0,0,0);
	[unroll]
	for(int i=-4; i<5; i++) {
		color += gInput2[int2(dispatchThreadID.x, clamp(dispatchThreadID.y+i, 0, gInput2.Length.y-1))]*0.15f;
	}
	gOutput[dispatchThreadID.xy] = color;
}




