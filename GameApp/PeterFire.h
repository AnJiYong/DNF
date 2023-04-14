#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class GameEngineCollision;
class PeterFire : public GameEngineActor
{
public:
	PeterFire();
	~PeterFire();

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineCollision* BodyCollision_;
	float4 MoveVector_;
	bool Pase2_;
	bool Pase3_;
	float DeathTime_;
	float Speed_;

public:
	void Pase2();

	void Pase3();

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Fire();

public:
	void SetSpeed(float _Value)
	{
		Speed_ = _Value;
	}

	void SetMoveVector(float4 _PlayerPos);
	void SetMoveDir(float4 _Pos);
	void Damage(GameEngineCollision* _Other);
};