#pragma once
#include "GameEngine/GameEngineActor.h"

enum class HPBarZOrder
{
	InvincibilityIcon,
	Invincibility,
	BossPatternMark,
	MonsterDeath,
	MonsterIcon,
	HPBarCount,
	BossHPBar,
	BossHPBarBack,
	HPBarFront,
	HPBar,
	HPBarBack,
	Base
};

class MonsterBase;
class GameEngineUIRenderer;
class GameEngineProgressBarRenderer;
class MonsterHpBar : public GameEngineActor
{
public:
	MonsterHpBar();
	~MonsterHpBar();

protected:
	GameEngineUIRenderer* BaseRenderer_;

	GameEngineProgressBarRenderer* HPBarRenderer_;
	GameEngineUIRenderer* HPBarBackRenderer_;
	GameEngineUIRenderer* HPBarCountRenderer_[5];
	GameEngineUIRenderer* HPBarFrontRenderer_;

	GameEngineProgressBarRenderer* BossHPBarRenderer_;
	GameEngineUIRenderer* BossHPBarBackRenderer_;

	GameEngineUIRenderer* MonsterIconRenderer_;
	GameEngineUIRenderer* MonsterDeathRenderer_;

	GameEngineUIRenderer* InvincibilityRenderer_;
	GameEngineUIRenderer* InvincibilityIconRenderer_;

	GameEngineUIRenderer* MarkRenderer1_;
	GameEngineUIRenderer* MarkRenderer2_;

	MonsterBase* Monster_;
	MonsterBase* PrevMonster_;

	int MonsterHP_;
	int PrevHP_;
	bool IsBoss_;
	bool HitDamageOn_;
	float HitDamageUpTime_;
	float HitDamageDownTime_;
	float HPBarOffTime_;


public:
	void SetMonster(class MonsterBase* _Monster);
	void SetIsBoss(class MonsterBase* _Monster);

private:
	void Start() override;
	void Update(float _Time) override;

	void HPBarCount(int _Value);
	void HitDamageShake(float _Time);
};

