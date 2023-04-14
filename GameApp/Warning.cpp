#include "Precompile.h"
#include "Warning.h"
#include "GlobalEnum.h"
#include "GameEngine\GameEngineImageRenderer.h"

Warning::Warning()
	: ImageRenderer_(nullptr)
	, RangeRenderer_(nullptr)
	, TopRenderer_(nullptr)
	, MiddleRenderer_(nullptr)
	, BottomRenderer_(nullptr)
	, On_(false)
	, OffTime_(1.f)
	, Circle_(false)
	, CircleTime_(0.f)
{
}

Warning::~Warning()
{
}

void Warning::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("GreenLine", "GreenLine", 0.1f, 0, 11);
	ImageRenderer_->CreateAnimationFolder("RedLine", "RedLine", 0.1f, 0, 11);
	ImageRenderer_->CreateAnimationFolder("VioletLine", "VioletLine", 0.1f, 0, 11);
	ImageRenderer_->CreateAnimationFolder("RedCircle", "RedCircle", 0.075f, 0, 11, false);
	ImageRenderer_->SetChangeAnimation("GreenLine");

	ImageRenderer_->SetEndCallBack("RedCircle", [&]() {Death(); });

	RangeRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	RangeRenderer_->SetImage("GreenLine.png");
	RangeRenderer_->SetAlpha(0.5f);

	MiddleRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	MiddleRenderer_->SetImage("RedLineMiddle.png");
	MiddleRenderer_->SetAlpha(0.5f);
	MiddleRenderer_->Off();

	TopRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	TopRenderer_->SetImage("RedLineTop.png");
	TopRenderer_->Off();

	BottomRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	BottomRenderer_->SetImage("RedLineTop.png");
	BottomRenderer_->Off();
}

void Warning::Update(float _DeltaTime)
{
	GetTransform()->SetWorldPositionZ(static_cast<float>(ZOrder::Warnimg));

	if (true == On_)
	{
		OffTime_ -= _DeltaTime;

		if (OffTime_ <= 0.f)
		{
			Death();
		}
	}

	if (true == Circle_)
	{
		CircleTime_ -= _DeltaTime;
		//RangeRenderer_->SetAlpha(CircleTime_ * 2.f);

		if (CircleTime_ >= 0.5f)
		{
			//RangeRenderer_->SetAlpha(1.f);
		}
	}
}

void Warning::WarningAnimation(std::string _Name)
{
	ImageRenderer_->SetChangeAnimation(_Name);

	std::string Name = _Name;
	Name.append(".png");
	RangeRenderer_->SetImage(Name);
	RangeRenderer_->GetTransform()->SetLocalPosition(float4(0.f, -43.5f));
	RangeRenderer_->GetTransform()->SetLocalScaling(float4(838.f, 1722.f));
}

void Warning::WarningIine(float4 _Start, float4 _End, float _Height)
{
	On_ = true;
	ImageRenderer_->Off();
	RangeRenderer_->Off();

	MiddleRenderer_->On();
	TopRenderer_->On();
	BottomRenderer_->On();

	float X = _Start.x - _End.x;
	float Y = _Start.y - _End.y;

	double Distance = sqrt((X * X) + (Y * Y));
	float Width = static_cast<float>(Distance);

	double Degree = atan2(static_cast<double>(Y), static_cast<double>(X));
	Degree *= GameEngineMath::RadianToDegree;
	GetTransform()->SetWorldRotationDegree(float4(0.f, 0.f, static_cast<float>(Degree)));

	float4 Center = _End + float4(X * 0.5f, Y * 0.5f);
	GetTransform()->SetWorldPosition(Center);
	
	TopRenderer_->GetTransform()->SetLocalScaling(float4(Width, 25.f));
	TopRenderer_->GetTransform()->SetLocalPosition(float4(0, _Height * 0.5f));

	BottomRenderer_->GetTransform()->SetLocalScaling(float4(Width, 25.f));
	BottomRenderer_->GetTransform()->SetLocalPosition(float4(0, _Height * -0.5f));

	MiddleRenderer_->GetTransform()->SetLocalScaling(float4(Width, _Height));

	GetTransform()->SetWorldPositionZ(static_cast<float>(ZOrder::Warnimg));
}

void Warning::WarningCircle(float4 _Pos)
{
	Circle_ = true;

	GetTransform()->SetWorldPosition(_Pos);
	ImageRenderer_->SetChangeAnimation("RedCircle");
	RangeRenderer_->SetImage("RedCircle.png");
}
