#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

class GameEngineBlend : public GameEngineObjectNameBase
{
	friend class GameEngineBlendManager;

public:
	GameEngineBlend();  	
	~GameEngineBlend();  

protected:		 	
	GameEngineBlend(const GameEngineBlend& _other) = delete;  	
	GameEngineBlend(GameEngineBlend&& _other) noexcept = delete;  	
	GameEngineBlend& operator=(const GameEngineBlend& _other) = delete;  	
	GameEngineBlend& operator=(const GameEngineBlend&& _other) = delete;  

private:
	void Create(const D3D11_BLEND_DESC& _Info, float4 _Factor, unsigned int _Mask);

public:
	void ReCreate(const D3D11_BLEND_DESC& _Info, float4 _Factor, unsigned int _Mask);

	inline ID3D11BlendState** GetSamplerState()
	{
		return &State_;
	}

	void Setting();

	void Reset();

private:		 	
	ID3D11BlendState* State_;
	D3D11_BLEND_DESC Info_;
	float4 Factor_;
	unsigned int Mask_;
};

