#include "common.fx"

[numthreads(32,32,1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID) {
	float4 color = float4(0,0,0,0);
	[unroll]
	for(int i=-4; i<5; i++) {
		color += gInput2[int2(clamp(dispatchThreadID.x+i, 0, gInput2.Length.x-1), dispatchThreadID.y)]*0.15f;
	}
	gOutput[dispatchThreadID.xy] = color;
}




