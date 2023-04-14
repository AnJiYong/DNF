#pragma once
#include "MonsterBase.h"

enum class MonsterType
{
	None,
	Infectee,
	Gackle,
	MeltKnight
};

class NomalMonster : public MonsterBase
{
public:
	NomalMonster();
	~NomalMonster();

private:
	GameEngineImageRenderer* ImageRenderer_;
	float4 RandVector_;

	MonsterType MonsterType_;
	bool IsAttack_;

public:
	void SetMonsterType(MonsterType _MonsterType)
	{
		MonsterType_ = _MonsterType;

		switch (_MonsterType)
		{
		case MonsterType::Infectee:
			MonsterName_ = "��Ƴ��� �ֹ�";
			break;
		case MonsterType::Gackle:
			MonsterName_ = "��Ŭ";
			break;
		case MonsterType::MeltKnight:
			MonsterName_ = "��Ʈ ����Ʈ";
			break;
		}
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Attack() override;
	void Hit() override;
	void MonsterDeath() override;

	void Idle();
	void Move();
	void PlayerAttack();
};