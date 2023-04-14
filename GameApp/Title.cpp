#include "Precompile.h"
#include "Title.h"
#include "GameEngine/GameEngineImageRenderer.h"

Title::Title()
	: imageRenderer_(nullptr)
{
}

Title::~Title()
{
}

void Title::Start()
{
	GameEngineImageRenderer* Image0 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image0->SetImage("Title0.png");

	GameEngineImageRenderer* Image1 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image1->SetImage("Title1.png");
	Image1->GetTransform()->SetWorldPosition(float4(-5.f, -150.f, -1.f));

	GameEngineImageRenderer* Image2 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image2->SetImage("Title2.png");
	Image2->GetTransform()->SetWorldPosition(float4(0.f, -283.f, -2.f));

	imageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	imageRenderer_->SetImage("Title3.png");
	imageRenderer_->GetTransform()->SetWorldPosition(float4(0.f, -171.f, -3.f));
}

void Title::Update(float _DeltaTime)
{
	imageRenderer_->GetTransform()->AddLocalDeltaTimeRotation(float4(0.f, 0.f, -10000.f) * _DeltaTime);
}