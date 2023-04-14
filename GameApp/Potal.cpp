#include "Precompile.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "Potal.h"
#include "GlobalEnum.h"
#include "TownLevel.h"

Potal::Potal()
	: ImageRenderer_()
	, ImageCollision()
{
}

Potal::~Potal()
{
}

void Potal::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Potal", "Gatedown", 0.1f);
	ImageRenderer_->SetChangeAnimation("Potal");

	UpImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	UpImageRenderer_->CreateAnimationFolder("Potal", "Gateup", 0.1f);
	UpImageRenderer_->SetChangeAnimation("Potal");

	ImageCollision = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Potal));
	ImageCollision->GetTransform()->SetLocalScaling(float4( 80.f, 40.f, 1.f ));
	ImageCollision->GetTransform()->SetLocalPosition(float4(10.f, -65.f));
}

void Potal::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(ImageCollision->GetTransform(), CollisionType::Rect);
#endif // DEBUG

	ImageCollision->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&Potal::Teleport, this));
}

void Potal::Teleport()
{
	TownLevel* level = (TownLevel*)GetLevel();
	level->Teleport();
}