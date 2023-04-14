#include "Precompile.h"
#include "Pillasia.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "LiaRichter.h"
#include "AirshipScreen.h"
#include "Potal.h"

Pillasia::Pillasia()
	: ImageRenderer_(nullptr)
{
}

Pillasia::~Pillasia()
{
}

void Pillasia::Start()
{
	GameEngineImageRenderer* Image0 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image0->SetImage("Pillasia7.png");
	Image0->GetTransform()->SetLocalPosition(float4(-448.f, 0.f, 1.f));

	GameEngineImageRenderer* Image1 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image1->SetImage("Pillasia8.png");
	Image1->GetTransform()->SetLocalPosition(float4(-224.f, 0.f, 1.f));

	GameEngineImageRenderer* Image2 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image2->SetImage("Pillasia9.png");
	Image2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 1.f));

	GameEngineImageRenderer* Image3 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image3->SetImage("Pillasia10.png");
	Image3->GetTransform()->SetLocalPosition(float4(224.f, 0.f, 1.f));

	GameEngineImageRenderer* Image4 = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Image4->SetImage("Pillasia11.png");
	Image4->GetTransform()->SetLocalPosition(float4(448.f, 0.f, 1.f));

	LiaRichter* Actor = GetLevel()->CreateActor<LiaRichter>();
	Actor->GetTransform()->AttachTransform(GetTransform());
	Actor->GetTransform()->SetLocalPosition(float4(115.f, -50.f, 0.f));

	AirshipScreen* Actor1 = GetLevel()->CreateActor<AirshipScreen>();
	Actor1->GetTransform()->AttachTransform(GetTransform());
	Actor1->GetTransform()->SetLocalPosition(float4(-138.f, 78.f, 0.f));
}

void Pillasia::Update(float _DeltaTime)
{
}