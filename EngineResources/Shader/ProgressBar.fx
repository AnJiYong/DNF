#include "CbufferHeader.fx"

cbuffer TextureCutData : register(b1)
{
    float2 TextureCutDataPos;
    float2 TextureCutDataSize;
};

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
};

VertexOut ProgressBar_VS(VertexIn _in)
{
    VertexOut Out;

    Out.Position = _in.Position;

    Out.Position.w = 1.f;
    Out.Position = mul(Out.Position, WVP);

    Out.Texcoord.x = (_in.Texcoord.x * TextureCutDataSize.x) + TextureCutDataPos.x;
    Out.Texcoord.y = (_in.Texcoord.y * TextureCutDataSize.y) + TextureCutDataPos.y;

    return Out;
}

cbuffer CorrectResultColor : register(b0)
{
    float4 vMulColor;
    float4 vPlusColor;
};

cbuffer ProgressBarCBuffer : register(b1)
{
    float Percent;
    int PregressDirection;
    float2 Empty2;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 ProgressBar_PS(VertexOut _in) : SV_Target0
{
    if (PregressDirection == 0 && Percent < _in.Texcoord.y)
    {        
        clip(-1);
    }
    else if (PregressDirection == 1 && 1.f - Percent > _in.Texcoord.y)
    {
        clip(-1);
    }
    else if (PregressDirection == 2 && Percent < _in.Texcoord.x)
    {
        clip(-1);
    }
    else if (PregressDirection == 3 && 1.f - Percent > _in.Texcoord.x)
    {
        clip(-1);
    }

    float4 Color = (Tex.Sample(Smp, _in.Texcoord.xy) * vMulColor);

    Color += vPlusColor;
    return Color;
}
