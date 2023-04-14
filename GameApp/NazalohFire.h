#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class GameEngineCollision;
class NazalohFire : public GameEngineActor
{
public:
	NazalohFire();
	~NazalohFire();

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineCollision* BodyCollision_;
	float4 MoveVector_;
	float Speed_;
	float Pivot_;
	float StrateTime_;
	float AcceleSpeed_;
	float Dirf_;
	bool Hit_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SetMoveDir(float4 _Pos);
	void Damage(GameEngineCollision* _Other);
};