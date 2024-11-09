#define MAX_POINT_LIGHTS 8
struct Vin
{
    float3 pos : POSITION;
    float4 colour : COLOR;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};
struct Vout
{
    float4 pos : SV_POSITION;
    float4 colour : COLOR;
    float2 UV : TEXCOORD;

};
struct PointLight
{
    float4 position;
    float4 colour;
    float strength;
    bool enabled;
    float2 padding;
};
cbuffer CBuffer0
{
    matrix WVP;
    float4 ambientLightCol;
    float4 directionalLightDir;
    float4 directionalLightCol;
    PointLight pointLights[MAX_POINT_LIGHTS];
};

Vout main(Vin Input)
{
    Vout output;
    output.pos = mul(WVP, float4(Input.pos, 1.0f));
    output.UV = Input.UV;
    
    float diffuseAmount = dot(directionalLightDir.xyz, Input.normal);
    diffuseAmount = saturate(diffuseAmount);
    float3 directionalFinal = directionalLightCol * diffuseAmount;
    
    float3 pointFinal = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if(!pointLights[i].enabled)
            continue;
        
        float4 pointLightDir = normalize(pointLights[i].position - float4(Input.pos, 1.0f));
        float3 pointLightDistance = length(pointLights[i].position - float4(Input.pos, 1.0f));
        float3 pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = dot(pointLightDir.xyz, Input.normal) * pointLightAttenuation;
        pointAmount = saturate(pointAmount);
        pointFinal += pointLights[i].colour * pointAmount;
    }
    
    
    
    
    output.colour = saturate(ambientLightCol + float4(directionalFinal, 1.0f) + float4(pointFinal, 1));
    
    return output;
}