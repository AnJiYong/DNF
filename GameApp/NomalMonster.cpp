#include "Precompile.h"
#include "NomalMonster.h"
#include "MapBase.h"
#include "GameManager.h"
#include "TopUI.h"

#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineCollision.h"

NomalMonster::NomalMonster()
	: ImageRenderer_(nullptr)
	, MonsterType_(MonsterType::None)
	, RandVector_(float4::ZERO)
	, IsAttack_(false)
{
}

NomalMonster::~NomalMonster()
{
}

void NomalMonster::Start()
{
	MonsterBase::Start();

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Attack", "Infectee", 0.1f, 0, 6);
	ImageRenderer_->CreateAnimationFolder("Move", "Infectee", 0.1f, 7, 11);
	ImageRenderer_->CreateAnimationFolder("Idle", "Infectee", 0.3f, 12, 13);
	ImageRenderer_->CreateAnimationFolder("Hit", "Infectee", 0.2f, 14, 15);
	ImageRenderer_->SetChangeAnimation("Attack");

	ImageRenderer_->CreateAnimationFolder("GackleAttack", "Gackle", 0.1f, 10, 16);
	ImageRenderer_->CreateAnimationFolder("GackleMove", "Gackle", 0.1f, 0, 8);
	ImageRenderer_->CreateAnimationFolder("GackleIdle", "Gackle", 0.3f, 9, 9);
	ImageRenderer_->CreateAnimationFolder("GackleHit", "Gackle", 0.2f, 35, 36);

	ImageRenderer_->CreateAnimationFolder("MeltKnightAttack", "MeltKnight", 0.1f, 10, 18);
	ImageRenderer_->CreateAnimationFolder("MeltKnightMove", "MeltKnight", 0.1f, 4, 9);
	ImageRenderer_->CreateAnimationFolder("MeltKnightIdle", "MeltKnight", 0.15f, 0, 3);
	ImageRenderer_->CreateAnimationFolder("MeltKnightHit", "MeltKnight", 0.2f, 19, 20);

	ImageRenderer_->SetEndCallBack("Attack", [&]() { StateManager_.ChangeState("Move"); });
	ImageRenderer_->SetEndCallBack("Hit", [&]() { StateManager_.ChangeState("Move"); });

	ImageRenderer_->SetEndCallBack("GackleAttack", [&]() { StateManager_.ChangeState("Move"); });
	ImageRenderer_->SetEndCallBack("GackleHit", [&]() { StateManager_.ChangeState("Move"); });

	ImageRenderer_->SetEndCallBack("MeltKnightAttack", [&]() { StateManager_.ChangeState("Move"); });
	ImageRenderer_->SetEndCallBack("MeltKnightHit", [&]() { StateManager_.ChangeState("Move"); });
	
	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 70.f, 35.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, -100.f });

	TraceCollision_->GetTransform()->SetLocalScaling(float4{ 100.f, 30.f, 1.f });
	TraceCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 50.f, 30.f, 1.f });
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });

	StateManager_.CreateState("Idle", std::bind(&NomalMonster::Idle, this));
	StateManager_.CreateState("Move", std::bind(&NomalMonster::Move, this));
	StateManager_.CreateState("Attack", std::bind(&NomalMonster::PlayerAttack, this));
	StateManager_.CreateState("Hit", std::bind(&NomalMonster::Hit, this));
	StateManager_.CreateState("Death", std::bind(&NomalMonster::MonsterDeath, this));
	StateManager_.ChangeState("Idle");

	State_.AttackCoolTime_ = 2.f;
	State_.MoveHoldTime_ = 1.f;
	State_.Speed_ = 50.f;
	State_.HpMax_ = 777;
	State_.HP_ = State_.HpMax_;
	State_.MoveTime_ = State_.MoveHoldTime_;
	State_.AttackTime_ = State_.AttackCoolTime_;
	State_.IdleHoldTime_ = 1.f;
	State_.IdleTime_ = State_.IdleHoldTime_;
}

void NomalMonster::Update(float _DeltaTime)
{
	MonsterBase::Update(_DeltaTime);
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });
	StateManager_.Update();

	if ("Move" == ImageRenderer_->GetCurrentAnimationName() ||
		"Idle" == ImageRenderer_->GetCurrentAnimationName() ||
		"GackleMove" == ImageRenderer_->GetCurrentAnimationName() ||
		"GackleIdle" == ImageRenderer_->GetCurrentAnimationName() ||
		"MeltKnightMove" == ImageRenderer_->GetCurrentAnimationName() ||
		"MeltKnightIdle" == ImageRenderer_->GetCurrentAnimationName())
	{
		DirLock_ = false;
	}
	else
	{
		DirLock_ = true;
	}

	if (false == DirLock_)
	{
		if (PlayerPos_.x >= BodyCollision_->GetTransform()->GetWorldPosition().x)
		{
			DirLeft_ = false;
			ImageRenderer_->SetReverse(false);
			ImageRenderer_->GetTransform()->SetLocalPosition(float4(-20.f, 50.f));
		}
		else
		{
			DirLeft_ = true;
			ImageRenderer_->SetReverse(true);
			ImageRenderer_->GetTransform()->SetLocalPosition(float4(20.f, 50.f));
		}

		if (MonsterType_ == MonsterType::Gackle)
		{
			ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 35.f));
		}
	}
}

void NomalMonster::Attack()
{
	if (State_.AttackTime_ >= 0.f || true == IsHitAnimation_ || true == Invincibility_)
	{
		return;
	}

	StateManager_.ChangeState("Attack");
}

void NomalMonster::Hit()
{
	StateManager_.ChangeState("Hit");

	if (true == SuperArmor_)
	{
		return;
	}

	IsHitAnimation_ = true;

	switch (MonsterType_)
	{
	case MonsterType::Infectee:
		ImageRenderer_->SetChangeAnimation("Hit");
		break;
	case MonsterType::Gackle:
		ImageRenderer_->SetChangeAnimation("GackleHit");
		break;
	case MonsterType::MeltKnight:
		ImageRenderer_->SetChangeAnimation("MeltKnightHit");
		break;
	}
}

void NomalMonster::MonsterDeath()
{
	StateManager_.ChangeState("Death");
	ImageRenderer_->AnimationStop();
	ImageRenderer_->SetPlusColor(float4(1.f, 1.f, 1.f, 0.f));

	Invincibility_ = true;

	DeathTime_ -= DeltaTime_;

	if (DeathTime_ <= 0.f)
	{
		GameManager::GetInst().GetUI()->MonsterHPBarOff();
		CurMap_->MonsterDeath();
		Death();
	}
}

void NomalMonster::Idle()
{

	SuperArmor_ = false;
	Invincibility_ = false;
	IsHitAnimation_ = false;
	IsAttack_ = false;

	switch (MonsterType_)
	{
	case MonsterType::Infectee:
		ImageRenderer_->SetChangeAnimation("Idle");
		break;
	case MonsterType::Gackle:
		ImageRenderer_->SetChangeAnimation("GackleIdle");
		break;
	case MonsterType::MeltKnight:
		ImageRenderer_->SetChangeAnimation("MeltKnightIdle");
		break;
	}

	State_.IdleTime_ -= DeltaTime_;

	if (State_.IdleTime_ <= 0.f)
	{
		State_.IdleTime_ = State_.IdleHoldTime_;

		GameEngineRandom Random;
		RandVector_.x = Random.RandomFloat(-1.f, 1.f);;
		RandVector_.y = Random.RandomFloat(-1.f, 1.f);
		StateManager_.ChangeState("Move");
	}
}

void NomalMonster::Move()
{
	switch (MonsterType_)
	{
	case MonsterType::Infectee:
		ImageRenderer_->SetChangeAnimation("Move");
		break;
	case MonsterType::Gackle:
		ImageRenderer_->SetChangeAnimation("GackleMove");
		break;
	case MonsterType::MeltKnight:
		ImageRenderer_->SetChangeAnimation("MeltKnightMove");
		break;
	}

	PrevPos_ = GetTransform()->GetWorldPosition();
	State_.MoveTime_ -= DeltaTime_;

	if (State_.MoveTime_ <= 0.f)
	{
		State_.MoveTime_ = State_.MoveHoldTime_;
		StateManager_.ChangeState("Idle");
		return;
	}

	float4 Vector;
	Vector.x = PlayerPos_.x - GetTransform()->GetWorldPosition().x;
	Vector.y = PlayerPos_.y - GetTransform()->GetWorldPosition().y;

	if (State_.AttackTime_ >= 0.f)
	{
		Vector = RandVector_;
	}

	Vector.Normalize3D();

	GetTransform()->SetLocalDeltaTimeMove(Vector * State_.Speed_);

	if (BodyCollision_->GetTransform()->GetWorldPosition().y > -20.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().y < -300.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().x < -450.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().x > 450.f)
	{
		GetTransform()->SetWorldPosition(PrevPos_);

		State_.MoveTime_ = State_.MoveHoldTime_;
		StateManager_.ChangeState("Idle");
		return;
	}
}

void NomalMonster::PlayerAttack()
{
	switch (MonsterType_)
	{
	case MonsterType::Infectee:
		ImageRenderer_->SetChangeAnimation("Attack");
		break;
	case MonsterType::Gackle:
		ImageRenderer_->SetChangeAnimation("GackleAttack");
		break;
	case MonsterType::MeltKnight:
		ImageRenderer_->SetChangeAnimation("MeltKnightAttack");
		break;
	}

	State_.AttackTime_ = State_.AttackCoolTime_;

	if (false == IsAttack_)
	{
		IsAttack_ = true;

		if (true == DirLeft_)
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -35.f, 0.f, 0.f });
		}
		else
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 35.f, 0.f, 0.f });
		}
	}
}
