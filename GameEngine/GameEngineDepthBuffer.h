#pragma once
#include "GameEngineTexture.h"

class GameEngineDepthBuffer : public GameEngineObjectNameBase
{
public:
	GameEngineDepthBuffer();
	~GameEngineDepthBuffer();

	GameEngineDepthBuffer(const GameEngineDepthBuffer& _Other) = delete;
	GameEngineDepthBuffer(GameEngineDepthBuffer&& _Other) noexcept = delete;
	GameEngineDepthBuffer& operator=(const GameEngineDepthBuffer& _Other) = delete;
	GameEngineDepthBuffer& operator=(GameEngineDepthBuffer&& _Other) noexcept = delete;

public:
	void Create(float4 _Scale);

	inline ID3D11DepthStencilView* GetDepthStencilView()
	{
		return DepthTexture_->GetDepthStencilView();
	}

private:
	GameEngineTexture* DepthTexture_;
};

