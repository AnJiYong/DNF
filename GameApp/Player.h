#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GameEngine/GameEngineFSM.h"
#include "GlobalEnum.h"

enum class PrevKey
{
	None,
	MoveLeft,
	MoveRight
};

class PlayerInfo
{	
public:
	int HPMax;
	int HP;
	int MPMax;
	int MP;
	float Critical;
};

class Avatar;
class GameEngineLevel;
class GameEngineUIRenderer;
class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();


	GameEngineCollision* BodyCollision_;

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* PlayerImageRenderer;
	GameEngineImageRenderer* SkillEffect1Renderer_;
	GameEngineImageRenderer* SkillEffect2Renderer_;
	GameEngineImageRenderer* SkillEffect3Renderer_;
	GameEngineImageRenderer* SkillEffect4Renderer_;
	GameEngineImageRenderer* SkillEffect5Renderer_;
	GameEngineImageRenderer* BuffEffectRenderer_;
	GameEngineCollision* AttackCollision_;

	GameEngineFSM TownStateManager_;
	GameEngineFSM DungeonStateManager_;

	Avatar* CurAvatar_;

	PlayerInfo Info_;

	GameEngineUIRenderer* TextRenderer_;

	float4 PrevPos_;
	PrevKey PrevKey_;
	float PrevKeyTime_;
	float DeltaTime_;
	float Speed;
	float JumpPower_;
	float ZPos_;
	float Acceleration_;
	float Dirf_;
	float DebuffTime_;
	float DebuffValue_;
	bool DebuffOn_;
	bool KeyLock_;
	bool DirLeft_;
	bool MoveLeft_;
	bool MoveRight_;
	bool MoveUp_;
	bool MoveDown_;
	bool DashLeft_;
	bool DashRight_;
	bool JumpUp_;
	bool IsDungeon_;
	bool NextAttack_;
	bool SuperArmor_;
	bool HitAnimationChange_;
	bool CollisionMonsterCheck_;
	float HitTime_;
	float ChargecrashTime_;

	int TripleslashCount_;
	bool TripleslashNext_;
	float TripleslashTime_;

	bool MomentaryslashNext_;

	bool GuardOn_;
	float GuardTime_;

	float MindSwordChargeTime_;
	bool MindSwordNext_;

	bool GorecrossNext_;

	bool IsSkill_;
	float4 Pivot_;
	float4 SKillPivot_;

	AttackInfo AttackInfo_;
	int	AttackCount_;
	std::string SkillKey_;

	std::vector<class MonsterBase*> MonsterList_;

public:
	int Damage_;

public:
	AttackInfo GetAttackInfo()
	{
		return AttackInfo_;
	}

	PlayerInfo GetPlayerInfo()
	{
		return Info_;
	}

	void SetIsDungeon(bool _IsDungeon)
	{
		IsDungeon_ = _IsDungeon;
		IsSkill_ = !_IsDungeon;
	}

	bool GetIsDungeon()
	{
		return IsDungeon_;
	}

	GameEngineCollision* GetBodyCollision()
	{
		return BodyCollision_;
	}

	float GetHPPercent()
	{
		return static_cast<float>(Info_.HP) / static_cast<float>(Info_.HPMax);
	}

	float GetMPPercent()
	{
		return static_cast<float>(Info_.MP) / static_cast<float>(Info_.MPMax);
	}

	bool GetPlayerDirection();

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;

	void PlayerAnimation();
	void SkillAnimation();
	void Collision();
	void FSM();

	void CommandCheck(float _DeltaTime);
	void SkillCheck();
	void DirCheck();
	void DebuffCheck(float _DeltaTime);

	void TownIdle();
	void TownMove();

	void MoveBase();
	void ColorCheckX();
	void ColorCheckY();

	void Idle();
	void Move();
	void Attack();
	void AttackFrame();
	void AttackEnd();
	void Dash();
	void DashAttack();
	void DashAttackEnd();
	void Jump();
	void JumpAttack();
	void JumpAttackFrame();
	void JumpAttackEnd();
	void HitAnimation();
	void HitAnimationAir();

	void SkillStart(class SkillSlot* _SkillSlot, std::string _Key);
	void SkillEndCoolTime(std::string _Name);
	void MonsterListClear();

	void Tripleslash();
	void Chargecrash();
	void Momentaryslash();
	void MomentaryslashEnd();
	void Flowmindone();
	void Flowmindtwo();
	void FlowmindtwoEnd();
	void Flowmindthree();
	void Guard();
	void MindSword();
	void Gorecross();

	void TelekineticSwords();
	void EtherealSlash();
	void Pentastrike();
	void ChargeBurst();
	void Lightning();

public:
	void ChangeItemImage(const std::string& _Name, ItemType _Type);
	void Hit(int _Damage, bool _AttackDirLeft);
	void Damage(GameEngineCollision* _Other);
	void HPPercentUp(float _Percent);
	void MPPercentUp(float _Percent);
	void Debuff(float _Time, float _Value);
};

