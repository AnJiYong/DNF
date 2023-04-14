#include "Precompile.h"
#include "NazalohFire.h"
#include "GlobalEnum.h"
#include "Player.h"
#include "GameManager.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

NazalohFire::NazalohFire()
	: ImageRenderer_(nullptr)
	, BodyCollision_(nullptr)
	, MoveVector_(float4::ZERO)
	, Speed_(200.f)
	, Sound_(nullptr)
	, Pivot_(0.f)
	, StrateTime_(0.5f)
	, AcceleSpeed_(0.f)
	, Dirf_(0.f)
	, Hit_(false)
{
}

NazalohFire::~NazalohFire()
{
}

void NazalohFire::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4( 20.f, 80.f, 0.f ));
	ImageRenderer_->CreateAnimationFolder("Throwposion", "Throwposion", 0.1f, 0, 2);
	ImageRenderer_->CreateAnimationFolder("Throwposion_end", "Throwposion_end", 0.1f, 0, 4 , false);
	ImageRenderer_->SetChangeAnimation("Throwposion");

	ImageRenderer_->SetEndCallBack("Throwposion_end", [&]() { Death(); });

	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Fire));
	BodyCollision_->GetTransform()->SetLocalScaling(float4( 20.f, 10.f, 1.f ));

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void NazalohFire::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&NazalohFire::Damage, this, std::placeholders::_1), CollisionCheck::Begin);

	GetTransform()->SetLocalDeltaTimeMove(MoveVector_ * Speed_);
	StrateTime_ -= _DeltaTime;


	if (StrateTime_ <= 0.f && false == Hit_)
	{
		ImageRenderer_->GetTransform()->SetLocalPosition(float4(-40.f * Dirf_, 90.f - Pivot_, 0.f));
		ImageRenderer_->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, Pivot_ * Dirf_));

		AcceleSpeed_ += _DeltaTime * 3.f;
		Pivot_ += _DeltaTime * 40.f + AcceleSpeed_;
		Speed_ -= _DeltaTime * 60.f;

		if (Speed_ < 0.f)
		{
			Speed_ = 0.f;
		}

		if (Pivot_ >= 90.f)
		{
			ImageRenderer_->SetChangeAnimation("Throwposion_end");
			MoveVector_ = float4::ZERO;
			ImageRenderer_->GetTransform()->SetLocalPosition(float4(-40.f * Dirf_, 0.f, 0.f));
			ImageRenderer_->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, 0.f));
		}
	}
}

void NazalohFire::SetMoveDir(float4 _Pos)
{
	float4 Pos = _Pos;
	Pos.Normalize3D();
	MoveVector_ = Pos;

	if (MoveVector_.x >= 0.f)
	{
		Dirf_ = -1.f;
		ImageRenderer_->SetReverse(false);
	}
	else
	{
		Dirf_ = 1.f;
		ImageRenderer_->SetReverse(true);
	}

	ImageRenderer_->GetTransform()->SetLocalPosition(float4(-40.f * Dirf_, 90.f, 0.f));
	BodyCollision_->GetTransform()->SetLocalPosition(float4(-50.f * Dirf_, 0.f, 0.f));
}

void NazalohFire::Damage(GameEngineCollision* _Other)
{
	Hit_ = true;

	Player* player = GameManager::GetInst().GetPlayer();

	ImageRenderer_->GetTransform()->SetLocalPosition(float4(-60.f * Dirf_, 90.f - Pivot_, 0.f));
	ImageRenderer_->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, -90.f * Dirf_));
	ImageRenderer_->SetChangeAnimation("Throwposion_end");
	MoveVector_ = float4::ZERO;

	player->Debuff(1.8f, 0.5f);

	if (player->BodyCollision_->GetTransform()->GetWorldPosition().x <= BodyCollision_->GetTransform()->GetWorldPosition().x)
	{
		player->Hit(500, false);
	}
	else
	{
		player->Hit(500, true);
	}

	BodyCollision_->GetTransform()->SetLocalPosition(float4( 10000.f, 0.f, 0.f ));
}
