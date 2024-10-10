struct Vin
{
    float3 pos : POSITION;
    float4 colour : COLOR;
};
struct Vout
{
    float4 pos : SV_POSITION;
    float4 colour : COLOR;

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
    return output;
}