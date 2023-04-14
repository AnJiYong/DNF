#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngine/GameEngineDevice.h"

class GameEngineDepthStencil : public GameEngineObjectNameBase
{
	friend class GameEngineDepthStencilManager;

public:
	GameEngineDepthStencil();
	~GameEngineDepthStencil();

	GameEngineDepthStencil(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil(GameEngineDepthStencil&& _Other) noexcept = delete;
	GameEngineDepthStencil& operator=(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil& operator=(GameEngineDepthStencil&& _Other) noexcept = delete;

private:
	void Create(const D3D11_DEPTH_STENCIL_DESC& _Info);

public:
	void ReCreate(const D3D11_DEPTH_STENCIL_DESC& _Info);

	void Setting();

	void Reset();

private:
	D3D11_DEPTH_STENCIL_DESC Info_;
	ID3D11DepthStencilState* State_;
};

