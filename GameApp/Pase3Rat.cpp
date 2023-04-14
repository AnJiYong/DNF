#include "Precompile.h"
#include "Pase3Rat.h"
#include "PeterThePiper.h"
#include "GlobalEnum.h"
#include "PeterFire.h"
#include "Warning.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

Pase3Rat::Pase3Rat()
	: RatW_(false)
	, RatWDeath_(false)
	, Peter_(nullptr)
	, MoveVector_(float4::ZERO)
	, ImageRenderer_(nullptr)
{
}

Pase3Rat::~Pase3Rat()
{
}

void Pase3Rat::Start()
{
	MonsterBase::Start();
	DirLock_ = true;
	Invincibility_ = true;

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalScaling(float4(0.f, 60.f));
	ImageRenderer_->CreateAnimationFolder("RatB", "RatB", 0.15f, 0, 8);
	ImageRenderer_->CreateAnimationFolder("RatW", "RatW", 0.15f, 0, 8);
	ImageRenderer_->CreateAnimationFolder("RatWDeath", "RatW", 0.08f, 18, 24);
	ImageRenderer_->SetChangeAnimation("RatB");

	ImageRenderer_->SetStartCallBack("RatWDeath", std::bind(&Pase3Rat::RatWDeath, this));
	ImageRenderer_->SetEndCallBack("RatWDeath", [&]() { Death(); });

	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 100.f, 35.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 20.f, -30.f, -100.f });

	State_.Speed_ = 200.f;
}

void Pase3Rat::Update(float _DeltaTime)
{
	MonsterBase::Update(_DeltaTime);

	if ("Idle" == Peter_->ImageRenderer_->GetCurrentAnimationName())
	{
		Boom();
	}

	if (true == RatW_ && false == RatWDeath_)
	{
		ImageRenderer_->SetChangeAnimation("RatW");
	}

	MoveVector_.x = 0.f - GetTransform()->GetWorldPosition().x;
	MoveVector_.y = -90.f - GetTransform()->GetWorldPosition().y;
	MoveVector_.Normalize3D();

	if (MoveVector_.x <= 0.f)
	{
		ImageRenderer_->SetReverse(true);
		BodyCollision_->GetTransform()->SetLocalPosition(float4{ -20.f, -30.f, -100.f });
	}

	GetTransform()->SetLocalDeltaTimeMove(MoveVector_* State_.Speed_);

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Weapon)
		, std::bind(&Pase3Rat::Catch, this));

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Trigger)
		, std::bind(&Pase3Rat::Boom, this));
}

void Pase3Rat::Catch()
{
	if (true == RatW_)
	{
		RatWDeath_ = true;
		ImageRenderer_->SetChangeAnimation("RatWDeath");
		State_.Speed_ = 0.f;
	}
	else
	{
		float4 Pos = BodyCollision_->GetTransform()->GetWorldPosition();

		Warning* War = GetLevel()->CreateActor<Warning>();
		War->WarningCircle(Pos);

		PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
		Peter_->ShockWave();

		Pos.y += 75.f;
		Fire->Pase2();
		Fire->SetMoveVector(PlayerPos_);
		Fire->GetTransform()->SetWorldPosition(Pos);

		Death();
	}
}

void Pase3Rat::Boom()
{	
	float4 Pos = BodyCollision_->GetTransform()->GetWorldPosition();

	Warning* War = GetLevel()->CreateActor<Warning>();
	War->WarningCircle(Pos);

	PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();

	Pos.y += 75.f;
	Fire->Pase3();
	Fire->SetMoveVector(PlayerPos_);
	Fire->GetTransform()->SetWorldPosition(Pos);

	Death();
}

void Pase3Rat::RatWDeath()
{
	Peter_->Pase3Count();
	Sound_->PlayOverLap("vo_shk.mp3", 0);
}
