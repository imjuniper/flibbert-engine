struct VSInput
{
    float3 Position: POSITION;
};

cbuffer Matrices
{
    float4x4 u_Projection;
    float4x4 u_View;
    float4x4 u_Model;
}

float4 VSMain(VSInput input) : SV_Position
{
    float4x4 world = mul(u_Projection, u_View);
    world = mul(world, u_Model);
    return mul(world, float4(input.Position, 1.0));
}
