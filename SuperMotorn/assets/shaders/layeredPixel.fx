#include "common.fx"

void calculateDirectionalLight(in float3 pToCamera, in DirectionalLight pLight, in Material pMaterial, in float3 pNormal, out float4 pAmbient, out float4 pDiffuse, out float4 pSpecular)
{
	pAmbient = pMaterial.ambient * pLight.ambient;
	pDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	pSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float3 lightVector = -pLight.direction;
	float diffuseFactor = dot(lightVector, pNormal);
	if(diffuseFactor > 0.0f) {
		pDiffuse = diffuseFactor * pMaterial.diffuse * pLight.diffuse;
		float3 v = reflect(-lightVector, pNormal);
		float specularFactor = pow(max(dot(v, pToCamera), 0.0f), pMaterial.specular.w);
		pSpecular = specularFactor * pMaterial.specular * pLight.specular * diffuseFactor;
		//pSpecular = max(dot(v, pToCamera), 0.0f);
		//pSpecular = specularFactor;// * pMaterial.specular * pLight.specular;
	}
}

void calculatePointLight(in float3 pToCamera, in PointLight pLight, in Material pMaterial, in float3 pPosition, in float3 pNormal, out float4 pAmbient, out float4 pDiffuse, out float4 pSpecular)
{
	pAmbient = pMaterial.ambient * pLight.ambient;
	pDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	pSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float3 lightVector = pLight.position - pPosition;
	float d = length(lightVector);
	if(d > pLight.range) {
		return;
	}
	lightVector /= d;
	
	float diffuseFactor = dot(lightVector, pNormal);
	[flatten]
	if(diffuseFactor > 0.0f) {
		float att = 1.0f / dot(pLight.attenuation, float3(1.0f, d, d*d));
		pDiffuse = diffuseFactor * pMaterial.diffuse * pLight.diffuse * att;
		float3 v = reflect(-lightVector, pNormal);
		float specularFactor = pow(max(dot(v, pToCamera), 0.0f), pMaterial.specular.w);
		pSpecular = specularFactor * pMaterial.specular * pLight.specular * att * diffuseFactor;
	}
}
POut PS(VOut input) : SV_Target {
	float3 toCamera = normalize(gCameraPosition - input.positionW);
	float distToCamera = length(toCamera);
	float4 texColor = float4(gColor, 1.0f);
	if( gUseDiffuseMap ) {
		texColor = input.color.x * gDiffuseTexture.Sample(samplerState0, input.uv);
	}
	if( gUseGlossMap ) {
		texColor += input.color.y * gGlossTexture.Sample(samplerState0, input.uv);
	}
	if(gUseGlowMap) {
		texColor += input.color.z * gGlowTexture.Sample(samplerState0, input.uv);
	}
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 A, D, S;
	float3 lightDir = normalize(gDirectionalLight.direction);
	calculateDirectionalLight(toCamera, gDirectionalLight, gMaterial, input.normalW.xyz, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;

	for(int i=0; i < gPointLightCount; i++) {
		calculatePointLight(toCamera, gPointLights[i], gMaterial, input.positionW, input.normalW.xyz, A, D, S);
		ambient += A;
		diffuse += D;
		specular += S;
	}
	if( gUseSpecularMap == 1 ) {
		specular *= gSpecularTexture.Sample(samplerState0, input.uv);
	}
	float fogLerp = saturate((distToCamera - gFogStart) / gFogRange);
	float4 color = texColor * (ambient + diffuse) + specular;
	if ( gUseSkyColor ) {
		color = lerp(color, gCubeMap.Sample(samplerState0, input.positionW), fogLerp);
	} else {
		color = lerp(color, gFogColor, fogLerp);
	}
	color.a = texColor.a;
	POut output;
	output.diffuse = color;
	return output;
}



