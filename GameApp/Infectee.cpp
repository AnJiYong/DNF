#include "Precompile.h"
#include "Infectee.h"
#include "MapBase.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

Infectee::Infectee()
	: ImageRenderer_(nullptr)
{
}

Infectee::~Infectee()
{
}

void Infectee::Start()
{
	MonsterBase::Start();

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Attack", "Infectee", 0.1f, 0, 6);
	ImageRenderer_->CreateAnimationFolder("Move", "Infectee", 0.1f, 7, 11);
	ImageRenderer_->CreateAnimationFolder("Idle", "Infectee", 0.1f, 12, 13);
	ImageRenderer_->CreateAnimationFolder("Hit", "Infectee", 0.1f, 14, 15);
	ImageRenderer_->CreateAnimationFolder("Death", "Infectee", 0.1f, 14, 18);
	ImageRenderer_->SetChangeAnimation("Attack");

	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 140.0f, 70.0f, 1.0f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.0f, 0.0f, -100.0f });

	State_.HpMax_ = 30000;
	State_.HP_ = State_.HpMax_;
}

void Infectee::Update(float _DeltaTime)
{
	MonsterBase::Update(_DeltaTime);

	if (false == DirLock_)
	{
		if (PlayerPos_.x >= BodyCollision_->GetTransform()->GetWorldPosition().x)
		{
			ImageRenderer_->SetReverse(false);
			ImageRenderer_->GetTransform()->SetLocalPosition(float4(-20.f, 55.f));
		}
		else
		{
			ImageRenderer_->SetReverse(true);
			ImageRenderer_->GetTransform()->SetLocalPosition(float4(20.f, 55.f));
		}
	}
}

void Infectee::MonsterDeath()
{
	CurMap_->MonsterDeath();
	Death();
}
