#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class PeterTrap : public GameEngineActor
{
public:
	PeterTrap();
	~PeterTrap();

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* AuraRenderer_;
	GameEngineCollision* BodyCollision_;
	bool Play_;
	bool Pase3_;
	bool DirLeft_;
	bool IsFire_;
	float AttackTime_;
	float ButterFlyTime_;
	bool ButterFlyChange_;
	int ButterFlyCount_;
	float ButterFlyHardModeTime_;
	float4 RandPos_;
	bool PeterDeath_;
	bool SoundOn_;

public:
	void SetPlay(bool _Play, bool _Pase3 = false, bool _Death = false)
	{
		Play_ = _Play;
		Pase3_ = _Pase3;
		PeterDeath_ = _Death;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Damage(GameEngineCollision* _Other);
	void AttackColliosinPos();
	void Pase3Fire();
	void TrapStart();
	void TrapPattern(float _DeltaTime);
	void ButterFlyPattern(float _DeltaTime);
};