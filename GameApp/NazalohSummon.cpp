#include "Precompile.h"
#include "NazalohSummon.h"
#include "MapBase.h"
#include "GameManager.h"
#include "TopUI.h"

#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineCollision.h"

NazalohSummon::NazalohSummon()
	: ImageRenderer_(nullptr)
	, BoomRenderer_(nullptr)
	, RandVector_(float4::ZERO)
	, IsAttack_(false)
	, TriggerOn_(false)
	, Dirf_(0.f)
	, AlphaTime_(1.f)
	, Pase2_(false)
{
}

NazalohSummon::~NazalohSummon()
{
}

void NazalohSummon::Start()
{
	MonsterBase::Start();
	EffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	EffectRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 1.f));
	EffectRenderer_->CreateAnimationFolder("Summon", "Summon", 0.1f, 0, 13, false);
	EffectRenderer_->SetChangeAnimation("Summon");

	EffectRenderer_->SetFrameCallBack("Summon", 5, std::bind(&NazalohSummon::SummonMonster, this));
	EffectRenderer_->SetEndCallBack("Summon", [&]() { EffectRenderer_->AnimaOff(); });

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 50.f));
	ImageRenderer_->CreateAnimationFolder("Attack", "Infectee", 0.1f, 0, 6);
	ImageRenderer_->CreateAnimationFolder("Move", "Infectee", 0.1f, 7, 11);
	ImageRenderer_->CreateAnimationFolder("Idle", "Infectee", 0.3f, 12, 13);
	ImageRenderer_->CreateAnimationFolder("Hit", "Infectee", 0.2f, 14, 15);
	ImageRenderer_->CreateAnimationFolder("Death", "Infectee", 0.1f, 14, 18, false);
	ImageRenderer_->SetChangeAnimation("Idle");
	ImageRenderer_->SetAlpha(0.f);
	ImageRenderer_->Off();

	ImageRenderer_->SetEndCallBack("Attack", [&]() { StateManager_.ChangeState("Move"); });
	ImageRenderer_->SetEndCallBack("Hit", [&]() { StateManager_.ChangeState("Move"); });

	MarkRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	MarkRenderer_->SetRenderingPipeLine("TextureTrans");
	MarkRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 130.f, 1.f));
	MarkRenderer_->SetImage("NazalohSummon0.png");
	MarkRenderer_->Off();

	BoomRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	BoomRenderer_->SetRenderingPipeLine("TextureTrans");
	BoomRenderer_->CreateAnimationFolder("Boom", "Darkboom", 0.05f, 0, 12);
	BoomRenderer_->GetTransform()->SetLocalPosition(float4(-10.f, 170.f));
	BoomRenderer_->Off();

	BoomRenderer_->SetEndCallBack("Boom", [&]() { Off(); });

	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 70.f, 35.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, -100.f });

	TraceCollision_->GetTransform()->SetLocalScaling(float4{ 100.f, 30.f, 1.f });
	TraceCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 50.f, 30.f, 1.f });
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });

	StateManager_.CreateState("Idle", std::bind(&NazalohSummon::Idle, this));
	StateManager_.CreateState("Move", std::bind(&NazalohSummon::Move, this));
	StateManager_.CreateState("Attack", std::bind(&NazalohSummon::PlayerAttack, this));
	StateManager_.CreateState("Hit", std::bind(&NazalohSummon::Hit, this));
	StateManager_.CreateState("Death", std::bind(&NazalohSummon::MonsterDeath, this));
	StateManager_.ChangeState("Idle");

	MonsterName_ = "¸áÆ® ³ªÀÌÆ®";

	State_.AttackCoolTime_ = 2.f;
	State_.MoveHoldTime_ = 1.f;
	State_.Speed_ = 50.f;
	State_.HpMax_ = 7;
	State_.HP_ = State_.HpMax_;
	State_.MoveTime_ = State_.MoveHoldTime_;
	State_.AttackTime_ = State_.AttackCoolTime_;
	State_.IdleHoldTime_ = 1.f;
	State_.IdleTime_ = 10.f;
}

void NazalohSummon::Update(float _DeltaTime)
{
	MonsterBase::Update(_DeltaTime);

	if (AlphaTime_ < 1.f)
	{
		AlphaTime_ += _DeltaTime;
		ImageRenderer_->SetAlpha(AlphaTime_);
	}

	if (false == TriggerOn_)
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4( 10000.f, 10000.f, 0.f ));
	}

	StateManager_.Update();

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&NazalohSummon::TriggerOn, this, std::placeholders::_1));

	if (false == StateManager_.IsCurrentState("Death"))
	{
		DirCheck();
	}
}

void NazalohSummon::Attack()
{
	if (State_.AttackTime_ >= 0.f || true == IsHitAnimation_ || true == Invincibility_)
	{
		return;
	}

	StateManager_.ChangeState("Attack");
}

void NazalohSummon::Hit()
{
	if (true == SuperArmor_ || true == Invincibility_)
	{
		return;
	}

	StateManager_.ChangeState("Hit");

	IsHitAnimation_ = true;
	ImageRenderer_->SetChangeAnimation("Hit");
}

void NazalohSummon::MonsterDeath()
{
	if (false == TriggerOn_)
	{
		if ("Death" != ImageRenderer_->GetCurrentAnimationName())
		{
			StateManager_.ChangeState("Death");
			ImageRenderer_->SetChangeAnimation("Death");
			Invincibility_ = true;

			GameManager::GetInst().GetUI()->MonsterHPBarOff();
		}
	}
}

void NazalohSummon::DirCheck()
{
	if ("Move" == ImageRenderer_->GetCurrentAnimationName() ||
		"Idle" == ImageRenderer_->GetCurrentAnimationName())
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
			Dirf_ = 1.f;
			ImageRenderer_->SetReverse(false);
		}
		else
		{
			Dirf_ = -1.f;
			ImageRenderer_->SetReverse(true);
		}
	}
}

void NazalohSummon::Idle()
{
	SuperArmor_ = false;
	Invincibility_ = false;
	IsHitAnimation_ = false;
	IsAttack_ = false;

	ImageRenderer_->SetChangeAnimation("Idle");
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

void NazalohSummon::Move()
{
	ImageRenderer_->SetChangeAnimation("Move");

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

	if (BodyCollision_->GetTransform()->GetWorldPosition().y > 3.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().y < -251.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().x < -450.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().x > 450.f)
	{
		GetTransform()->SetWorldPosition(PrevPos_);

		State_.MoveTime_ = State_.MoveHoldTime_;
		StateManager_.ChangeState("Idle");
		return;
	}
}

void NazalohSummon::PlayerAttack()
{
	ImageRenderer_->SetChangeAnimation("Attack");

	State_.AttackTime_ = State_.AttackCoolTime_;

	if (false == IsAttack_)
	{
		IsAttack_ = true;
		AttackCollision_->GetTransform()->SetLocalPosition(float4( 35.f * Dirf_, 0.f, 0.f ));
	}
}

void NazalohSummon::SummonMonster()
{
	ImageRenderer_->On();
	State_.IdleTime_ = 1.f;
	AlphaTime_ = 0.f;
}

void NazalohSummon::TriggerOn(GameEngineCollision* _Other)
{
	if (State_.HP_ <= 0.f && false == Pase2_)
	{
		TriggerOn_ = true;
		Boom();
	}
}

void NazalohSummon::Revive()
{
	StateManager_.ChangeState("Idle");
	State_.IdleTime_ = State_.IdleHoldTime_;
	State_.HP_ = 7;
}

void NazalohSummon::Boom()
{
	TriggerOn_ = true;
	ImageRenderer_->AnimaOff();
	BoomRenderer_->SetChangeAnimation("Boom"); 
	AttackCollision_->GetTransform()->SetLocalPosition(float4( 0.f, 0.f, 0.f ));
}

void NazalohSummon::Pase2Setting(int _Count)
{
	Pase2_ = true;
	MarkRenderer_->On();
	std::string Name = "NazalohSummon";
	std::string Middle = std::to_string(_Count);
	Name.append(Middle);
	Name.append(".png");
	MarkRenderer_->SetImage(Name);
}
