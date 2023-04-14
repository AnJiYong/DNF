#pragma once
#include "MonsterBase.h"

class PeterTrap;
class GameEngineSoundPlayer;
class PeterThePiper : public MonsterBase
{
	friend class Pase3Rat;

public:
	PeterThePiper();
	~PeterThePiper();

private:
	float4 RandVector_;
	int AttackCount_;

	GameEngineSoundPlayer* Sound_;
	std::vector<class PeterPase2*> Pase2List_;
	std::vector<class Pase2Rythem*> Pase2UIList_;
	std::vector<class PeterPase2*> Pase3List_;
	std::vector<class Pase2Rythem*> Pase3UIList_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* EffectRenderer_;
	GameEngineImageRenderer* EffectRenderer2_;
	PeterTrap* PeterTrap_;

	float NextPaseStartTime_;
	float Pase2UpTime_;
	float Pase2UIOnTime_;
	float Pase3UpTime_;
	float Pase3AttackTime;
	bool ReviveEnd_;
	bool Pase2_;
	bool Pase2End_;
	bool Pase3_;
	int Pase2Count_;
	int Pase2UIIndex_;
	int Pase3Count_;
	bool IsAttack_;
	float DeathTimeScale_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Attack() override;
	void Hit() override;
	void MonsterDeath() override;

	void NestPatternCheck();
	void DirCheck();
	void Idle();
	void Move();
	void AttackState();
	void SingAttack();
	void Fire();
	void NextPase();
	void Revive();
	void Effect();
	void Pase2();
	void Pase2UI(float _YVelue);
	void Pase3();
	void ShockWave();

public:
	void Pase2Count(int _Color);
	void Pase3Count();
};