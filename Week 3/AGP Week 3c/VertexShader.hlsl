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

Vout main(Vin Input)
{
    Vout output;
    output.pos = float4(Input.pos, 1);
    output.colour = Input.colour;
    return output;
}