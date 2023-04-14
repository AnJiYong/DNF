#include "Precompile.h"
#include "InventoryButton.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GlobalEnum.h"

InventoryButton::InventoryButton()
	: LeftImage_(nullptr)
	, MiddleImage_()
	, RightImage_(nullptr)
	, ButtonOn_(false)
	, Text_("")
{
}

InventoryButton::~InventoryButton()
{
}

void InventoryButton::Start()
{
	LeftImage_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	LeftImage_->SetImage("Button169.png");
	LeftImage_->GetTransform()->SetLocalPosition(float4(-13.5f, 9.5f, -1.f));

	for (size_t i = 0; i < 5; i++)
	{
		MiddleImage_[i] = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		MiddleImage_[i]->SetImage("Button170.png");
		MiddleImage_[i]->GetTransform()->SetLocalPosition(float4(-2.5f + (1.f * i), 9.5f, -1.f));
	}

	RightImage_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	RightImage_->SetImage("Button171.png");
	RightImage_->GetTransform()->SetLocalPosition(float4(13.f, 9.5f, -1.f));
}

void InventoryButton::Update(float _Time)
{
}

void InventoryButton::SetText(std::string _Text)
{
	Text_ = _Text;

	float4 Pivot = float4(-12.f, 7.f);
	float4 Color = float4(120.f / 255.f, 113.f / 255.f, 105.f / 255.f);

	if (true == ButtonOn_)
	{
		Pivot.y = 8.f;
		Color = float4(184.f / 255.f, 173.f / 255.f, 151.f / 255.f);
	}

	RightImage_->TextSetting("gulim", Text_, 11, Color, Pivot);
}

void InventoryButton::ButtonOn()
{
	ButtonOn_ = true;

	LeftImage_->SetImage("Button166.png");
	LeftImage_->GetTransform()->SetLocalPosition(float4(-13.5f, 10.5f, -1.f));

	for (size_t i = 0; i < 5; i++)
	{
		MiddleImage_[i]->SetImage("Button167.png");
		MiddleImage_[i]->GetTransform()->SetLocalPosition(float4(-2.5f + (1.f * i), 10.5f, -1.f));
	}

	RightImage_->SetImage("Button168.png");
	RightImage_->GetTransform()->SetLocalPosition(float4(13.f, 10.5f, -1.f));

	float4 Pivot = float4(-12.f, 8.f);
	float4 Color = float4(184.f / 255.f, 173.f / 255.f, 151.f / 255.f);
	RightImage_->TextSetting("gulim", Text_, 11, Color, Pivot);
}

void InventoryButton::ButtonOff()
{
	LeftImage_->SetImage("Button169.png");
	LeftImage_->GetTransform()->SetLocalPosition(float4(-13.5f, 9.5f, -1.f));

	for (size_t i = 0; i < 5; i++)
	{
		MiddleImage_[i]->SetImage("Button170.png");
		MiddleImage_[i]->GetTransform()->SetLocalPosition(float4(-2.5f + (1.f * i), 9.5f, -1.f));
	}

	RightImage_->SetImage("Button171.png");
	RightImage_->GetTransform()->SetLocalPosition(float4(13.f, 9.5f, -1.f));

	float4 Pivot = float4(-12.f, 7.f);
	float4 Color = float4(120.f / 255.f, 113.f / 255.f, 105.f / 255.f);
	RightImage_->TextSetting("gulim", Text_, 11, Color, Pivot);
}
