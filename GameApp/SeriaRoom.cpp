#include "Precompile.h"
#include "SeriaRoom.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "SeriaKirmin.h"
#include "Potal.h"

SeriaRoom::SeriaRoom()
	: ImageRenderer_(nullptr)
	, AlphaTime_(0.f)
	, Check_(true)
{
}

SeriaRoom::~SeriaRoom()
{
}

void SeriaRoom::Start()
{
	GameEngineImageRenderer* Image0 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image0->SetImage("SeriaRoom0.png");
	Image0->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 2.f));

	GameEngineImageRenderer* Image3 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image3->SetImage("SeriaRoom3.png");
	Image3->GetTransform()->SetLocalPosition(float4(-435.f, 77.f, 1.f));

	GameEngineImageRenderer* Image1 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image1->SetImage("SeriaRoom1.png");
	Image1->GetTransform()->SetLocalPosition(float4(0.f, 67.5f, 1.f));

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->SetImage("SeriaRoom2.png");
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 20.f, 1.f));
	ImageRenderer_->SetAlpha(1.f);

	SeriaKirmin* Actor = GetLevel()->CreateActor<SeriaKirmin>();
	Actor->GetTransform()->AttachTransform(GetTransform());
	Actor->GetTransform()->SetLocalPosition(float4(0.f, 50.f, 0.f));

	Potal* potal = GetLevel()->CreateActor<Potal>();
	potal->GetTransform()->AttachTransform(GetTransform());
	potal->GetTransform()->SetLocalPosition(float4(380.f, -80.f, 0.f));
}

void SeriaRoom::Update(float _DeltaTime)
{
	if (true == Check_)
	{
		AlphaTime_ += _DeltaTime;
	}
	else
	{
		AlphaTime_ -= _DeltaTime;
	}

	if (AlphaTime_ >= 4.f)
	{
		Check_ = false;
	}

	if (AlphaTime_ <= 0.f)
	{
		Check_ = true;
	}

	ImageRenderer_->SetAlpha(AlphaTime_ * 0.25f);
}