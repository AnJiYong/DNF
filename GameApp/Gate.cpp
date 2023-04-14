#include "Precompile.h"
#include "Gate.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GlobalEnum.h"
#include "DungeonLevel.h"

Gate::Gate()
	: ImageRenderer_(nullptr)
	, UpImageRenderer_(nullptr)
	, ImageCollision(nullptr)
	, Open_(false)
	, Type_(GateType::None)
	, ChangeTime_(0.f)
{
}

Gate::~Gate()
{
}

void Gate::SetGateType(GateType _GateType)
{
	Type_ = _GateType;

	if (_GateType == GateType::BossIn || _GateType == GateType::BossOut)
	{
		UpImageRenderer_->SetChangeAnimation("Boss");
		UpImageRenderer_->Off();
		ImageRenderer_->Off();
	}
}

void Gate::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Potal", "Gatedown", 0.1f);
	ImageRenderer_->SetChangeAnimation("Potal");

	UpImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	UpImageRenderer_->CreateAnimationFolder("Boss", "Potal", 0.1f);
	UpImageRenderer_->CreateAnimationFolder("Potal", "Gateup", 0.1f);
	UpImageRenderer_->SetChangeAnimation("Potal");
	UpImageRenderer_->Off();

	ImageCollision = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Potal));
	ImageCollision->GetTransform()->SetLocalScaling(float4(20.f, 10.f, 1.f));
	ImageCollision->GetTransform()->SetLocalPosition(float4(10.f, -65.f));
}

void Gate::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(ImageCollision->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	ImageCollision->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&Gate::NextDungeon, this), CollisionCheck::Begin);

	if (true == Open_)
	{
		ChangeTime_ -= _DeltaTime;
		UpImageRenderer_->On();
	}
}

void Gate::NextDungeon()
{
	if (true == Open_)
	{
		if (ChangeTime_ <= 0.f)
		{
			DungeonLevel* level = (DungeonLevel*)GetLevel();
			level->MapChange(Type_);
			ChangeTime_ = 0.5f;
		}

		if (Type_ == GateType::BossIn)
		{
			Off();
		}

		if (Type_ == GateType::BossOut)
		{
			GetLevel()->FadeIn(1.f);
		}
	}
}