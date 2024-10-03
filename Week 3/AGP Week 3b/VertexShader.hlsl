struct Vin
{
    float4 pos : POSITION;
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
    output.pos = Input.pos;
    output.colour = Input.colour;
    return output;
}