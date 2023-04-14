#include "Precompile.h"
#include "Weapon.h"
#include "GameEngine/GameEngineImageRenderer.h"

Weapon::Weapon()
	: FrontImageRenderer1(nullptr)
	, FrontImageRenderer2(nullptr)
	, BackImageRenderer1(nullptr)
	, BackImageRenderer2(nullptr)
	, Index_(0)
{
}

Weapon::~Weapon()
{
}

void Weapon::Start()
{
	FrontImageRenderer1 = CreateTransformComponent<GameEngineImageRenderer>();
	FrontImageRenderer1->GetTransform()->SetLocalPosition(float4(0.f,0.f, -0.1f));
	FrontImageRenderer1->CreateAnimationFolder("default", "sswd0000c", FLT_MAX, 0, 241);
	FrontImageRenderer1->CreateAnimationFolder("Weapon01", "beamswd00c1", FLT_MAX, 0, 241);
	FrontImageRenderer1->CreateAnimationFolder("Weapon02", "beamswd59c1", FLT_MAX, 0, 241);
	FrontImageRenderer1->SetChangeAnimation("default");

	FrontImageRenderer2 = CreateTransformComponent<GameEngineImageRenderer>();
	FrontImageRenderer2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.2f));
	FrontImageRenderer2->CreateAnimationFolder("Weapon01", "beamswd00c2", FLT_MAX, 0, 241);
	FrontImageRenderer2->CreateAnimationFolder("Weapon02", "beamswd59c2", FLT_MAX, 0, 241);
	FrontImageRenderer2->SetChangeAnimation("default");

	BackImageRenderer1 = CreateTransformComponent<GameEngineImageRenderer>();
	BackImageRenderer1->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.2f));
	BackImageRenderer1->CreateAnimationFolder("default", "sswd0000b", FLT_MAX, 0, 241);
	BackImageRenderer1->CreateAnimationFolder("Weapon01", "beamswd00b1", FLT_MAX, 0, 241);
	BackImageRenderer1->CreateAnimationFolder("Weapon02", "beamswd59b1", FLT_MAX, 0, 241);
	BackImageRenderer1->SetChangeAnimation("default");

	BackImageRenderer2 = CreateTransformComponent<GameEngineImageRenderer>(1);
	BackImageRenderer2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.1f));
	BackImageRenderer2->CreateAnimationFolder("Weapon01", "beamswd00b2", FLT_MAX, 0, 241);
	BackImageRenderer2->CreateAnimationFolder("Weapon02", "beamswd59b2", FLT_MAX, 0, 241);
	BackImageRenderer2->SetChangeAnimation("default");
}

void Weapon::Update(float _DeltaTime)
{
	FrontImageRenderer1->IsCurrentAnimationIndex(Index_);
	FrontImageRenderer2->IsCurrentAnimationIndex(Index_);
	BackImageRenderer1->IsCurrentAnimationIndex(Index_);
	BackImageRenderer2->IsCurrentAnimationIndex(Index_);
}

void Weapon::ChangeWeapon(std::string _Name)
{
	FrontImageRenderer1->SetChangeAnimation(_Name);
	FrontImageRenderer2->SetChangeAnimation(_Name);
	BackImageRenderer1->SetChangeAnimation(_Name);
	BackImageRenderer2->SetChangeAnimation(_Name);
}

void Weapon::SetReverse(bool _Reverse)
{
	FrontImageRenderer1->SetReverse(_Reverse);
	FrontImageRenderer2->SetReverse(_Reverse);
	BackImageRenderer1->SetReverse(_Reverse);
	BackImageRenderer2->SetReverse(_Reverse);

	float Pivot;

	if (true == _Reverse)
	{
		Pivot = -15.f;
	}
	else
	{
		Pivot = 15.f;
	}

	FrontImageRenderer1->GetTransform()->SetLocalPosition(float4(Pivot, 75.f));
	FrontImageRenderer2->GetTransform()->SetLocalPosition(float4(Pivot, 75.f));
	BackImageRenderer1->GetTransform()->SetLocalPosition(float4(Pivot, 75.f));
	BackImageRenderer2->GetTransform()->SetLocalPosition(float4(Pivot, 75.f));
}

void Weapon::SetRotation(float4 _Value)
{
	FrontImageRenderer1->GetTransform()->SetLocalRotationDegree(_Value);
	FrontImageRenderer2->GetTransform()->SetLocalRotationDegree(_Value);
	BackImageRenderer1->GetTransform()->SetLocalRotationDegree(_Value);
	BackImageRenderer2->GetTransform()->SetLocalRotationDegree(_Value);
}
