#include "common.fx"

[numthreads(32,32,1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID) {
	gOutput[dispatchThreadID.xy] = gInput1[dispatchThreadID.xy] + gInput2[dispatchThreadID.xy];
}






