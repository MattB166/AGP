struct Vin
{
    float3 pos : POSITION;
    float4 colour : COLOR;
    float2 UV : TEXCOORD;
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
};

Vout main(Vin Input)
{
    Vout output;
    output.pos = mul(WVP, float4(Input.pos, 1.0f));
    output.colour = Input.colour;
    output.UV = Input.UV;
    return output;
}