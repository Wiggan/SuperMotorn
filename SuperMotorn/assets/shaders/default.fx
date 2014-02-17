struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};
struct DirectionalLight 
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
};
struct PointLight 
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float	pad;
	float3 attenuation;
	float range;
};
cbuffer cbPerObject : register(b0) {
	float4x4 gWorld;
	Material gMaterial;
	float3	 gColor;
	int	 	 gUseTexture;
};
cbuffer cbPerFrame : register(b1) {
	float4x4 	gView;
	float4x4 	gProjection;
	float3	 	gCameraPosition;
	int		 	gPointLightCount;
	PointLight 	gPointLights[3];
};
cbuffer cbSeldom : register(b2) {
	DirectionalLight 	gDirectionalLight;
	float4 				gFogColor;
	float				gFogStart;
	float				gFogRange;
};
struct VIn
{
	float3 positionL : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEX0;
	float2 uv2 : TEX1;
};
struct VOut
{
	float4 positionH : SV_POSITION;
	float3 positionW : POSITION;
	float2 uv : TEXCOORD0;
	float4 normalW : TEXCOORD1;
};
VOut VS(VIn input) {
	VOut output;
	output.positionH 	= mul(float4(input.positionL, 1.0f), gWorld);
	output.positionW 	= output.positionH.xyz;
	output.positionH 	= mul(output.positionH, gView);
	output.positionH 	= mul(output.positionH, gProjection);
	output.uv 			= input.uv;
	output.normalW 		= normalize(mul(float4(input.normal, 0.0f), gWorld));
	return output;
}
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
		pSpecular = specularFactor * pMaterial.specular * pLight.specular* diffuseFactor;
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
float4 PS(VOut input) : SV_Target {
	float3 toCamera = gCameraPosition - input.positionW;
	float distToCamera = length(toCamera);
	float4 texColor = float4(gColor, 1.0f);

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
	float4 color = texColor * (ambient + diffuse) + specular;
	float fogLerp = saturate((distToCamera - gFogStart) / gFogRange);
	color = lerp(color, gFogColor, fogLerp);
	color.a = texColor.a;
	return color;
}



