#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GameEngine/GameEngineFSM.h"
#include "GlobalEnum.h"
#include "Player.h"

struct MonsterState
{
	int HpMax_;				// �ִ� ü��
	int Damage_;			// ���ݷ�
	float Speed_;			// �̵� �ӵ�
	float AttackCoolTime_;	// Attack ���� ���� ��Ÿ��
	float MoveHoldTime_;	// Move ���� ���� �ð�
	float IdleHoldTime_;	// Idle ���� ���� �ð�
	
	int HP_;				// ü��
	float AttackTime_;		// Attack ���� ���� ��Ÿ��
	float MoveTime_;		// Move ���� ���� �ð�
	float IdleTime_;		// Idle ���� ���� �ð�
	float DeathTime_;		// ����ϰ� ������� ������ �ð�
};

class MapBase;
class GameEngineCollision;
class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class GameEngineUIRenderer;
class MonsterBase : public GameEngineActor
{
	friend class MonsterHpBar;
	
public:
	MonsterBase();
	~MonsterBase();

protected:
	GameEngineSoundPlayer* Sound_;
	GameEngineCollision* BodyCollision_;
	GameEngineCollision* TraceCollision_;
	GameEngineCollision* AttackCollision_;
	GameEngineFSM StateManager_;
	MapBase* CurMap_;
	float4 PlayerPos_;
	float4 PrevPos_;
	std::string MonsterName_;

	MonsterState State_;

	int MonsterNumber_;
	bool TargetCheck_;

	bool DirLeft_;
	bool DirLock_;
	bool Invincibility_;

	bool SuperArmor_;
	bool IsHitAnimation_;
	bool IsBoss_;
	bool CriticalHit_;

	float ZPos_;
	float ZSize_;
	float DeltaTime_;

	std::vector<AttackInfo> HitInfo_;

	int SoundCount_;

public:
	bool GetInvincibility()
	{
		return Invincibility_;
	}

	std::string GetMonsterName()
	{
		return MonsterName_;
	}

	int GetMonsterNumber()
	{
		return MonsterNumber_;
	}

	void SetMonsterNumber(int _Value)
	{
		MonsterNumber_ = _Value;
	}

	bool GetTargetCheck()
	{
		return TargetCheck_;
	}

	void SetTargetCheck(bool _Value)
	{
		TargetCheck_ = _Value;
	}

	MonsterState GetMonsterState()
	{
		return State_;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	virtual void Attack() {};
	virtual void Hit() {};
	virtual void MonsterDeath() {};

public:
	void Damage(GameEngineCollision* _Other);
	void HitDamage(AttackInfo _AttackInfo, PlayerInfo _PlayerInfo);
	void RoomSetting(MapBase* _Map, bool _BossRoom = false);
};