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
 float3 pos;
 float padding;
};

Vout main(Vin Input)
{
    Vout output;
    output.pos = float4(Input.pos,1.0) + float4(pos, 0.0);
    output.colour = Input.colour;
    return output;
}