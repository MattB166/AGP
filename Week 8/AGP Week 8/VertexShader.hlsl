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
cbuffer CBuffer0
{
    matrix WVP;
    float4 ambientLightCol;
    float4 directionalLightDir;
    float4 directionalLightCol;
};

Vout main(Vin Input)
{
    Vout output;
    output.pos = mul(WVP, float4(Input.pos, 1.0f));
    output.UV = Input.UV;
    
    float diffuseAmount = dot(directionalLightDir.xyz, Input.normal);
    diffuseAmount = saturate(diffuseAmount);
    float3 directionalFinal = directionalLightCol * diffuseAmount;
    output.colour = saturate(ambientLightCol + float4(directionalFinal, 1.0f));
    
    return output;
}