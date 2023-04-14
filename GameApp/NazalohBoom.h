#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineCollision;
class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class NazalohBoom : public GameEngineActor
{
public:
	NazalohBoom();
	~NazalohBoom();

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineCollision* BodyCollision_;
	float DelayTime_;
	bool DelayOn_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void PlayerHit(GameEngineCollision* _Other);

public:
	void BoomDelay(bool _Value);
};