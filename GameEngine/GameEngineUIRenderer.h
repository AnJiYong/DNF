#pragma once
#include "GameEngineImageRenderer.h"
#include "ThirdParty/Include/FW1Font/FW1FontWrapper.h"

class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineUIRenderer : public GameEngineImageRenderer
{
	friend GameEngineLevel;
	friend CameraComponent;

public:
	GameEngineUIRenderer();
	~GameEngineUIRenderer();

	GameEngineUIRenderer(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer(GameEngineUIRenderer&& _Other) noexcept = delete;
	GameEngineUIRenderer& operator=(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer& operator=(GameEngineUIRenderer&& _Other) noexcept = delete;

private:
	void Start() override;
	void Render() override;

public:
	void SetOutLine(float4 _Color)
	{
		OutLine_ = true;
		OutLineColor_ = _Color;
	}

	void SetRenderGroup(int _Order) override;

	void TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, float4 _Color = float4::RED, 
		float4 _Pivot = float4::ZERO, FW1_TEXT_FLAG _Flag = FW1_CENTER);

protected:
	std::string FontName_;
	std::string PrintText_;
	float FontSize_;
	float4 FontPivot_;
	float4 Color_;
	FW1_TEXT_FLAG Flag_;

	float4 OutLineColor_;
	bool OutLine_;

	static int UIRendererCount;
	static GameEngineRenderTarget* FontTarget_;
};

