#include "Precompile.h"
#include "PeterPase2.h"
#include "PeterThePiper.h"
#include "GlobalEnum.h"
#include "Pase3Rat.h"
#include "NomalMonster.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

PeterPase2::PeterPase2()
	: ImageRenderer_(nullptr)
	, BodyCollision_(nullptr)
	, TriggerCollision_(nullptr)
	, Sound_(nullptr)
	, Peter_(nullptr)
	, Renderer_(nullptr)
	, IsUp_(false)
	, Color_(0)
{
}

PeterPase2::~PeterPase2()
{
}

void PeterPase2::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 5.f));
	ImageRenderer_->CreateAnimationFolder("Pase2Start", "PeterThePiperEffect", 0.15f, 0, 4, false);
	ImageRenderer_->CreateAnimationFolder("Pase2Up", "PeterThePiperEffect", 0.2f, 1, 4, false);
	ImageRenderer_->CreateAnimationFolder("Pase3", "PeterThePiperEffect", 0.05f, 46, 50);
	ImageRenderer_->SetChangeAnimation("Pase2Start");

	Renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Renderer_->CreateAnimationFolder("Up", "PeterThePiperEffect", 0.07f, 5, 14, false);
	Renderer_->CreateAnimationFolder("Down", "PeterThePiperEffect", 0.07f, 14, 5, false);
	Renderer_->CreateAnimationFolder("Up2", "PeterThePiperEffect", 0.07f, 89, 98, false);
	Renderer_->CreateAnimationFolder("Down2", "PeterThePiperEffect", 0.07f, 98, 89, false);

	Renderer_->SetEndCallBack("Up", [&]() { Renderer_->SetChangeAnimation("Down"); });
	Renderer_->SetEndCallBack("Down", std::bind(&PeterPase2::MonsterDeath, this));
	Renderer_->SetEndCallBack("Up2", [&]() { Renderer_->SetChangeAnimation("Down2"); });
	Renderer_->SetEndCallBack("Down2", std::bind(&PeterPase2::MonsterDeath, this));

	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Trigger));
	BodyCollision_->GetTransform()->SetLocalScaling(float4( 40.f, 60.f, 1.f ));
	BodyCollision_->GetTransform()->SetLocalPosition(float4( - 2000.f, -2000.f, -100.f ));

	TriggerCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Trigger));
	TriggerCollision_->GetTransform()->SetLocalScaling(float4( 10.f, 500.f, 1.f ));
	TriggerCollision_->GetTransform()->SetWorldPosition(float4( 0.f, -90.f, -100.f ));

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void PeterPase2::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushDebugRender(TriggerCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	TriggerCollision_->GetTransform()->SetWorldPosition(float4( 0.f, -90.f, -100.f ));
	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::MouseSub)
		, std::bind(&PeterPase2::Catch, this));

	ImageRenderer_->GetTransform()->SetWorldPositionZ(static_cast<float>(ZOrder::GroundEffect));
}

void PeterPase2::Catch()
{
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		Sound_->PlayOverLap("vo_shk.mp3", 0);

		IsUp_ = false;
		BodyCollision_->GetTransform()->SetLocalPosition(float4( - 2000.f, -2000.f, -100.f ));

		Peter_->Pase2Count(Color_);
	}
}

void PeterPase2::MonsterDeath()
{
	IsUp_ = false;
	BodyCollision_->GetTransform()->SetLocalPosition(float4( - 2000.f, -2000.f, -100.f));
}

void PeterPase2::Up()
{
	IsUp_ = true;

	ImageRenderer_->SetChangeAnimation("Pase2Up", true);

	GameEngineRandom Random;
	int Number = Random.RandomInt(0, 1);
	if (Number == 0)
	{
		Renderer_->SetChangeAnimation("Up");
		Color_ = 0;
	}
	else
	{
		Renderer_->SetChangeAnimation("Up2");
		Color_ = 1;
	}

	BodyCollision_->GetTransform()->SetLocalPosition(float4( 10.f, 30.f, -100.f ));	
}

void PeterPase2::Gray()
{
	ImageRenderer_->SetChangeAnimation("Pase3");
	ImageRenderer_->SetRenderingPipeLine("TextureTrans");

	Pase3Rat* Rat = GetLevel()->CreateActor<Pase3Rat>();
	float4 Pos = GetTransform()->GetWorldPosition();
	Pos.y += 30.f;
	Rat->GetTransform()->SetWorldPosition(Pos);
	Rat->SetPeter(Peter_);
}

void PeterPase2::White()
{
	ImageRenderer_->SetChangeAnimation("Pase3");
	ImageRenderer_->SetRenderingPipeLine("TextureTrans");

	Pase3Rat* Rat = GetLevel()->CreateActor<Pase3Rat>();
	float4 Pos = GetTransform()->GetWorldPosition();
	Pos.y += 30.f;
	Rat->GetTransform()->SetWorldPosition(Pos);
	Rat->SetRatW();
	Rat->SetPeter(Peter_);
}
