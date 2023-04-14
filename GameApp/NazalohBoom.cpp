#include "Precompile.h"
#include "NazalohBoom.h"
#include "GameManager.h"
#include "Player.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

NazalohBoom::NazalohBoom()
	: Sound_(nullptr)
	, BodyCollision_(nullptr)
	, ImageRenderer_(nullptr)
	, DelayTime_(0.f)
	, DelayOn_(false)
{
}

NazalohBoom::~NazalohBoom()
{
}

void NazalohBoom::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(-10.f, -20.f, 0.f));
	ImageRenderer_->CreateAnimationFolder("NazalohBoom", "NazalohBoom", 0.1f, 0, 25);
	ImageRenderer_->CreateAnimationFolder("NazalohBoom2", "NazalohBoom2", 0.1f, 0, 25);
	ImageRenderer_->SetChangeAnimation("NazalohBoom");

	ImageRenderer_->SetFrameCallBack("NazalohBoom", 3, [&]() {BodyCollision_->GetTransform()->SetLocalPosition(float4(0.f, 0.f)); });
	ImageRenderer_->SetFrameCallBack("NazalohBoom", 10, [&]() {BodyCollision_->GetTransform()->SetLocalPosition(float4(10000.f, 0.f)); });

	ImageRenderer_->SetFrameCallBack("NazalohBoom2", 16, [&]() {BodyCollision_->GetTransform()->SetLocalPosition(float4(0.f, 0.f)); });
	ImageRenderer_->SetFrameCallBack("NazalohBoom2", 23, [&]() {BodyCollision_->GetTransform()->SetLocalPosition(float4(10000.f, 0.f)); });

	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Fire));
	BodyCollision_->GetTransform()->SetLocalScaling(float4(120.f, 120.f, 1.f)); 
	BodyCollision_->GetTransform()->SetLocalPosition(float4(10000.f, 0.f));

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void NazalohBoom::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&NazalohBoom::PlayerHit, this, std::placeholders::_1), CollisionCheck::Begin);
}

void NazalohBoom::PlayerHit(GameEngineCollision* _Other)
{
	GameManager::GetInst().GetPlayer()->GetTransform()->SetWorldPosition(float4(-435.f, -150.f, 0.f));
}

void NazalohBoom::BoomDelay(bool _Value)
{
	ImageRenderer_->AnimaOff();
	BodyCollision_->GetTransform()->SetLocalPosition(float4(10000.f, 0.f));

	if (true == _Value)
	{
		ImageRenderer_->SetChangeAnimation("NazalohBoom");
	}
	else
	{
		ImageRenderer_->SetChangeAnimation("NazalohBoom2");
	}
}
