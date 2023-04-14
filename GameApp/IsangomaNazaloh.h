#pragma once
#include "MonsterBase.h"

class Warning;
class NazalohBoom;
class NazalohSummon;
class GameEngineSoundPlayer;
class IsangomaNazaloh : public MonsterBase
{
public:
	IsangomaNazaloh();
	~IsangomaNazaloh();

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* FrontEffectRenderer_;
	GameEngineImageRenderer* BackEffectRenderer_;
	std::vector<NazalohSummon*> SummonList_;
	std::vector<NazalohBoom*> BoomList_;
	std::vector<Warning*> WarningList_;

	bool IsAttack_;
	float4 RandVector_;
	float Dirf_;
	float AlphaValue_;
	bool AttackChange_;

	bool NextPase_;
	float Pase2SummonTime_;
	int Pase2SummonCount_;
	float4 Pase2SummonPos_[6];
	bool Pase2_;
	bool Pase3_;
	bool Pase3End_;
	bool FadeOut_;
	float FadeOutTime_;
	int Pase3Count_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Attack() override;
	void Hit() override;
	void MonsterDeath() override;

	void NextPatternCheck();
	void DirCheck();

	void Idle();
	void Move();
	void AttackState();
	void Summon();
	void SummonMonster();
	void SummonBoom();

	void NextPase();
	void Pase2();
	void Pase3();
	void Pase3Next(GameEngineCollision* _Other);
	void Pase3NextPattern();
};