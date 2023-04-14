#pragma once
#include "GameEngineComponent.h"
#include "GameEngineTransform.h"

class GameEngineTransformComponent : public GameEngineComponent
{
	friend GameEngineActor;

public:
	GameEngineTransformComponent();
	virtual ~GameEngineTransformComponent() = 0;

	GameEngineTransformComponent(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent(GameEngineTransformComponent&& _Other) noexcept = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent& operator=(GameEngineTransformComponent&& _Other) noexcept = delete;

public:
	GameEngineTransform* GetTransform()
	{
		return &Transform_;
	}

	void AttachTransform(GameEngineTransform* _Transform);

private:
	GameEngineTransform Transform_;
};

