struct VIn
{
	float4 position : POSITION;
	float4	colour : COLOR;
	float2 UV : TEXCOORD;
	float3 normal : NORMAL;

};
struct VOut
{
    float4 pos : SV_POSITION;
    float4 colour : COLOR;
    float3 uvw : TEXCOORD;
};
cbuffer CBuffer0
{
    matrix WVP;
};

VOut main(VIn Input)
{
	VOut output;
	output.pos = mul(WVP, Input.position);
    output.uvw = Input.position.xyz;
	output.colour = Input.colour;
	return output;
	
}