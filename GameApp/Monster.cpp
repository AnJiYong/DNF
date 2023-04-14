#include "PreCompile.h"
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include "Monster.h"
#include "Bullet.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

void Monster::TestStartCallBack()
{
	int a = 0;
}

void Monster::TestEndCallBack() 
{
	Death();
}

void Monster::Start()
{
	{
		ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		ImageRenderer_->CreateAnimationFolder("Seria", "Seria", 0.5f);
		ImageRenderer_->SetChangeAnimation("Seria");
		//playerImageRenderer_->GetTransform()->SetWorldPosition(float4(0.f, 0.f));
	}

	{
		GameEngineCollision* Collision = CreateTransformComponent<GameEngineCollision>(20);
		Collision->GetTransform()->SetLocalScaling(float4{ 100.0f, 100.0f, 1.0f });
	}
}

void Monster::Update(float _DeltaTime)
{

}