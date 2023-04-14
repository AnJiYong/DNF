#include "Precompile.h"
#include "TestAirMonster.h"
#include "GameEngineBase\GameEngineSoundManager.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "GlobalEnum.h"
#include "GameManager.h"
#include "MapBase.h"

TestAirMonster::TestAirMonster()
	: ImageRenderer_(nullptr)
	, ImageCollision_(nullptr)
	, ZImageCollision_(nullptr)
	, Sound_(nullptr)
	, ZPos(75.f)
	, HP_(6)
{
}

TestAirMonster::~TestAirMonster()
{
}

void TestAirMonster::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->SetImage("Test.png");
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 200.f));

	ImageRenderer1_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer1_->SetImage("Test1.png");
	ImageRenderer1_->GetTransform()->SetLocalPosition(float4(0.f, 0.f));

	ImageCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Monster));
	ImageCollision_->GetTransform()->SetLocalScaling(float4{ 70.f, 40.f, 1.f });
	ImageCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, -100.f });

	ZImageCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::None));
	ZImageCollision_->GetTransform()->SetLocalScaling(float4{ 1.f, ZPos, 1.f });
	ZImageCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 200.f, -100.f });

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void TestAirMonster::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(ImageCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushDebugRender(ZImageCollision_->GetTransform(), CollisionType::Rect);

	ImageCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Weapon)
		, std::bind(&TestAirMonster::Damege, this), CollisionCheck::Begin);
}

void TestAirMonster::Damege()
{
	if (GameManager::GetInst().GetPlayerZPos() <= 200.f - 150.f)
	{
		return;
	}

	Sound_->PlayOverLap("vo_shk.mp3", 0);

	//MonsterBase::Damage();

	HP_ -= 2;

	if (HP_ <= 0)
	{
		CurMap_->MonsterDeath();
		Death();
	}
}
