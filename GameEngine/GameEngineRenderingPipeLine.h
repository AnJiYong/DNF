#pragma once
#include "GameEngine/GameEngineDevice.h"
#include "GameEngineBase/GameEngineMath.h"
#include "GameEngineShaderResHelper.h"

class GameEngineBlend;
class GameEngineShader;
class GameEngineDepthStencil;
class GameEngineConstantBuffer;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEnginePixelShader;
class GameEngineRasterizer;
class GameEngineRenderingPipeLine : public GameEngineObjectNameBase
{
public:
	GameEngineRenderingPipeLine();
	~GameEngineRenderingPipeLine();

protected:
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _other) = delete;
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept;

	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _other) = delete;
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine&& _other) = delete;

private:
	void InputAssembler1();
	void InputAssembler2();
	void VertexShader();
	void Rasterizer();
	void PixelShader();
	void OutPutMerger();

public:
	inline GameEnginePixelShader* GetPixelShader() const
	{
		return PixelShader_;
	}

	inline GameEngineVertexShader* GetVertexShader() const
	{
		return VertexShader_;
	}

	void SetInputAssembler1InputLayOutSetting(const std::string& _Name);

	void SetInputAssembler1VertexBufferSetting(const std::string& _Name);

	void SetVertexShader(const std::string& _Name);

	void SetInputAssembler2IndexBufferSetting(const std::string& _Name);

	void SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY Topology_);

	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	void SetOutputMergerDepthStencil(const std::string& _Name);

	void RenderingPipeLineSetting();

	void Rendering();

	void Reset();

private:
	GameEngineVertexBuffer* VertexBuffer_;
	GameEngineVertexShader* InputLayOutVertexShader_;
	GameEngineVertexShader* VertexShader_;
	GameEngineIndexBuffer* IndexBuffer_;
	D3D11_PRIMITIVE_TOPOLOGY Topology_;
	GameEngineRasterizer* Rasterizer_;
	GameEnginePixelShader* PixelShader_;
	GameEngineBlend* Blend_;
	GameEngineRenderTarget* RenderTarget_;
	GameEngineDepthStencil* DepthStencil_;
};

