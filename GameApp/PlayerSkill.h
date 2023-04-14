#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GameEngine/GameEngineFSM.h"
#include "GlobalEnum.h"

class PlayerInfo;
class MonsterBase;
class GameEngineImageRenderer;
class PlayerSkill : public GameEngineActor
{
public:
	PlayerSkill();
	~PlayerSkill();

private:
	GameEngineImageRenderer* SkillEffect1Renderer_;
	GameEngineImageRenderer* SkillEffect2Renderer_;
	GameEngineFSM SkillStateManager_;
	MonsterBase* TargetMonster_;
	AttackInfo AttackInfo_;
	PlayerInfo* Info_;
	float4 Pos_;
	bool SkillOn_;
	float DeltaTIme_;
	float MindSwordTime_;
	int MindSwordCount_;
	int SkillCount_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void MindSwordFinish();

public:
	void SkillSetting(std::string _Name, int _Count, MonsterBase* _Monster, PlayerInfo* _Info);
};

