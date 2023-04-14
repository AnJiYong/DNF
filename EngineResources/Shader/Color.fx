#include "CbufferHeader.fx"
// b32 까지 가능했던걸로 아는데
// 그냥 생각하지 맙시다.
// 512바이트 가 최대일겁니다.

float4 Color_VS(float4 pos : POSITION) : SV_POSITION
{
    // pos *= World;

    pos.w = 1.f;

    pos = mul(pos, WVP);

    return pos;
}

cbuffer ResultColor : register(b0)
{
    float4 vColor;
};

float4 Color_PS(float4 pos : SV_POSITION) : SV_Target0
{
    return vColor;
}