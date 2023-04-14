#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"

class GameEngineLevel;
class GameEngineActor;
class GameEngineComponent : public GameEngineObjectNameBase
{
	friend GameEngineActor;

public:
	GameEngineComponent();
	virtual ~GameEngineComponent() = 0;

	GameEngineComponent(const GameEngineComponent& _Other) = delete;
	GameEngineComponent(GameEngineComponent&& _Other) noexcept = delete;
	GameEngineComponent& operator=(const GameEngineComponent& _Other) = delete;
	GameEngineComponent& operator=(GameEngineComponent&& _Other) noexcept = delete;

protected:
	virtual void InitComponent(GameEngineActor* Actor_);

	virtual void Start() = 0;
	virtual void Update(float _DeltaTime) = 0;

public:
	GameEngineActor* GetActor()
	{
		return Actor_;
	}

	GameEngineLevel* GetLevel();

private:
	GameEngineActor* Actor_;
};

