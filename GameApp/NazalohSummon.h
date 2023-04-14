#pragma once
#include "MonsterBase.h"

class NazalohSummon : public MonsterBase
{
public:
	NazalohSummon();
	~NazalohSummon();

private:
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* BoomRenderer_;
	GameEngineImageRenderer* EffectRenderer_;
	GameEngineImageRenderer* MarkRenderer_;
	float4 RandVector_;
	bool IsAttack_;
	float Dirf_;
	bool TriggerOn_;
	float AlphaTime_;
	bool Pase2_;

public:
	bool DeathCheck()
	{
		if (State_.HP_ <= 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Attack() override;
	void Hit() override;
	void MonsterDeath() override;

	void DirCheck();

	void Idle();
	void Move();
	void PlayerAttack();

	void SummonMonster();
	void TriggerOn(GameEngineCollision* _Other);

public:
	void Revive();
	void Boom();
	void Pase2Setting(int _Count);
};