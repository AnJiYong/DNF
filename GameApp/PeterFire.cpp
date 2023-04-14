#include "Precompile.h"
#include "PeterFire.h"
#include "GlobalEnum.h"
#include "Player.h"
#include "GameManager.h"
#include "Warning.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

PeterFire::PeterFire()
	: ImageRenderer_(nullptr)
	, BodyCollision_(nullptr)
	, MoveVector_(float4::ZERO)
	, Pase2_(false)
	, Pase3_(false)
	, DeathTime_(5.f)
	, Speed_(300.f)
	, Sound_(nullptr)
{
}

PeterFire::~PeterFire()
{
}

void PeterFire::Pase2()
{
	Pase2_ = true;
	Warning* War = GetLevel()->CreateActor<Warning>();
	War->WarningCircle(GetTransform()->GetWorldPosition() + float4(0.f, -75.f, 0.f));
}

void PeterFire::Pase3()
{	
	Pase3_ = true;
	Warning* War = GetLevel()->CreateActor<Warning>();
	War->WarningCircle(GetTransform()->GetWorldPosition() + float4(0.f, -75.f, 0.f));
}

void PeterFire::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Fly", "PeterThePiperEffect", 0.1f, 42, 45);
	ImageRenderer_->CreateAnimationFolder("Pase2Fire", "PeterThePiperEffect", 0.04f, 15, 41);
	ImageRenderer_->CreateAnimationFolder("Pase3Fire", "PeterThePiperEffect", 0.04f, 51, 77);
	ImageRenderer_->SetChangeAnimation("Fly");

	ImageRenderer_->SetFrameCallBack("Pase2Fire", 30, std::bind(&PeterFire::Fire, this));
	ImageRenderer_->SetFrameCallBack("Pase3Fire", 66, std::bind(&PeterFire::Fire, this));

	ImageRenderer_->SetEndCallBack("Pase2Fire", std::bind(&PeterFire::Death, this));
	ImageRenderer_->SetEndCallBack("Pase3Fire", std::bind(&PeterFire::Death, this));

	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Fire));
	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 20.f, 10.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void PeterFire::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&PeterFire::Damage, this, std::placeholders::_1), CollisionCheck::Begin);

	if (true == Pase2_)
	{
		ImageRenderer_->SetRenderingPipeLine("TextureTrans");
		ImageRenderer_->SetChangeAnimation("Pase2Fire");
		BodyCollision_->GetTransform()->SetLocalScaling(float4{ 160.f, 80.f, 1.f });
		BodyCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 0.f, 0.f });
	}
	else if (true == Pase3_)
	{
		ImageRenderer_->SetRenderingPipeLine("TextureTrans");
		ImageRenderer_->SetChangeAnimation("Pase3Fire");
		BodyCollision_->GetTransform()->SetLocalScaling(float4{ 160.f, 80.f, 1.f });
		BodyCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 0.f, 0.f });
	}
	else
	{
		GetTransform()->SetWorldPositionZ(BodyCollision_->GetTransform()->GetWorldPosition().y);

		/*if (BodyCollision_->GetTransform()->GetWorldPosition().y > 60.f)
		{
			 Death();
		}*/
		
		ImageRenderer_->GetTransform()->SetLocalPosition(float4{ 0.f, 100.f, 0.f });

		DeathTime_ -= _DeltaTime;

		if (DeathTime_ <= 0.f)
		{
			Death();
		}

		GetTransform()->SetLocalDeltaTimeMove(MoveVector_ * Speed_ * 2.f);
	}
}

void PeterFire::Fire()
{
	if (false == Sound_->IsPlay())
	{
		BodyCollision_->GetTransform()->SetLocalPosition(float4( 0.f, -75.f, 0.f ));
		Sound_->Stop();
		Sound_->PlayOverLap("inout_crash.ogg", 0);
		Sound_->SetVolume(0.4f);
	}
}

void PeterFire::SetMoveVector(float4 _PlayerPos)
{	
	MoveVector_.x = _PlayerPos.x - GetTransform()->GetWorldPosition().x;
	MoveVector_.y = _PlayerPos.y - GetTransform()->GetWorldPosition().y;
	//MoveVector_.y += 50.f;
	MoveVector_.Normalize3D();

	if (MoveVector_.x >= 0.f)
	{
		ImageRenderer_->SetReverse(true);
	}
}

void PeterFire::SetMoveDir(float4 _Pos)
{
	float4 Pos = _Pos;
	Pos.Normalize3D();
	MoveVector_ = Pos;

	if (MoveVector_.x >= 0.f)
	{
		ImageRenderer_->SetReverse(true);
	}
}

void PeterFire::Damage(GameEngineCollision* _Other)
{
	Player* player = GameManager::GetInst().GetPlayer();

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
