#pragma once
#include "GameEngineImageRenderer.h"
#include "Enums.h"

struct ProgressBarCBuffer
{
	ProgressBarCBuffer() : Percent(1.f), PregressDirection(0), Empty1(0.f), Empty2(0.f)
	{
	}

	float Percent;
	int PregressDirection;
	float Empty1;
	float Empty2;
};

class GameEngineTexture;
class GameEngineFolderTexture;
class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineProgressBarRenderer : public GameEngineImageRenderer
{
public:
	GameEngineProgressBarRenderer();
	~GameEngineProgressBarRenderer();

protected:
	GameEngineProgressBarRenderer(const GameEngineProgressBarRenderer& _other) = delete;
	GameEngineProgressBarRenderer(GameEngineProgressBarRenderer&& _other) noexcept = delete;

private:
	GameEngineProgressBarRenderer& operator=(const GameEngineProgressBarRenderer& _other) = delete;
	GameEngineProgressBarRenderer& operator=(const GameEngineProgressBarRenderer&& _other) = delete;

private:
	void Start() override;
	void Render() override;

public:
	void SetRenderGroup(int _Order) override;
	void SetProgressBarDirect(int _ProgressBarDir);
	void SetPercent(float _Percent);

private:
	ProgressBarCBuffer ProgressBarData_;
};

