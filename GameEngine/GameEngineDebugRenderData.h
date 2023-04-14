#pragma once
#include "GameEngineTransform.h"
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"

class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineDebugRenderData
{
	friend class CameraComponent;

public:
	GameEngineDebugRenderData();
	~GameEngineDebugRenderData();

	GameEngineDebugRenderData(const GameEngineDebugRenderData& _Other) 
		: Data_(_Other.Data_)
		, ShaderHelper_(_Other.ShaderHelper_)
		, PipeLine_(_Other.PipeLine_)
	{
	}

private:
	TransformData Data_;
	float4 Color_;
	GameEngineShaderResHelper ShaderHelper_;
	GameEngineRenderingPipeLine* PipeLine_;
};

