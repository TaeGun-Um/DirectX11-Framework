#include "Light.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct Output
{
    float4 POSITION : SV_Position;
    float4 TEXCOORD : TEXCOORD;
};

// 빛계산을 통해서 빛의 반사를 구현하고 나면
// 그 빛을 계산하는 시점에 따라서 예전에는 구분을 했다.
// 이제는 구분이 거의 의미가 없다.
// 빛이라는 컬러를 구해내는 것이 된다.

// 그걸 버텍스 쉐이더에서하면 그걸 점단위로 하면 플랫쉐이딩
// 그걸 버텍스 쉐이더에서하면 그걸 고로쉐이딩
// 그걸 픽셀 쉐이더에서하면 그걸 퐁쉐이딩

// 그래픽카드에서 이뤄지는것.
Output DeferredPostLight_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    return NewOutPut;
}

struct LightOutPut
{
    float4 DifLight : SV_Target0;
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
};

Texture2D DifLightTex : register(t0);
Texture2D SpcLightTex : register(t1);
Texture2D AmbLightTex : register(t2);
Texture2D ShadowTestTex : register(t4);
SamplerState POINTWRAP : register(s0);

LightOutPut DeferredPostLight_PS(Output _Input) 
{
    LightOutPut NewOutPut = (LightOutPut) 0;
    
    float4 Shadow = ShadowTestTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    
    NewOutPut.DifLight = DifLightTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    NewOutPut.SpcLight = SpcLightTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    NewOutPut.AmbLight = AmbLightTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    
    if (0 != Shadow.x)
    {
        NewOutPut.DifLight = 0.2f / Shadow.x;
        NewOutPut.SpcLight = 0.2f / Shadow.x;
    }
    
    return NewOutPut;
}

