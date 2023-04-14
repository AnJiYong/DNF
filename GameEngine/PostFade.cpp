#include "PreCompile.h"
#include "PostFade.h"


PostFade::PostFade()
	: FadeTime_(0.f)
	, Auto_(false)
	, Option(LIGHT)
{
	Data.Ratio = 1.f;
	Data.Clip = 0;
}

PostFade::~PostFade()
{
}

void PostFade::Effect(float _DeltaTime)
{
	if (true == Auto_ && Data.Ratio == 0.f)
	{
		SetData(FadeTime_, FadeOption::LIGHT);
	}

	Data.CurTime += _DeltaTime;

	if (Data.CurTime <= 0)
	{
		Data.CurTime = 0.f;
	}

	float Inter = Data.CurTime / Data.Time;

	switch (Option)
	{
	case LIGHT:
		Data.Ratio = Inter;

		if (1 <= Data.Ratio)
		{
			Data.Ratio = 1.f;
		}

		break;
	case DARK:
		Data.Ratio = 1.f - Inter;

		if (0 >= Data.Ratio)
		{
			Data.Ratio = 0.f;
		}

		break;
	default:
		break;
	}

	Result_->Clear();
	Result_->Setting();
	Res_.SettingTexture("Target", Target_->GetTexture(0));
	Res_.Setting();
	Effect_->Rendering();
	Effect_->Reset();
	Res_.ReSet();

	Target_->Clear(false);
	Target_->Copy(Result_);
}

void PostFade::Initialize() 
{
	CreateResultTarget();
	SetEffect("Fade");
	Res_.SettingConstantBufferLink("FadeData", Data);
}

void PostFade::SetData(float _Time, FadeOption _Option, bool _Auto)
{
	FadeTime_ = _Time;

	Data.Ratio = 1.f;

	Data.Time = FadeTime_;
	Data.CurTime = 0.f;
	Data.Clip = 0;

	Option = _Option;
	Auto_ = _Auto;
}