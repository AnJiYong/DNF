#include "Precompile.h"
#include "MapBase.h"
#include "GameEngine/GameEngineCore.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "Gate.h"

MapBase::MapBase()
	: MonsterCount_(0)
	, IsBossRoom_(false)
	, BossDeath_(false)
	, Clear_(false)
	, FadeOutStart_(false)
	, FadeInStart_(false)
	, FadeTime_(0.f)
	, FadeDeltaTime_(0.f)
	, FadeRenderer_(nullptr)
	, Type_(BossType::None)
{
}

MapBase::~MapBase()
{
}

void MapBase::Start()
{
	FadeRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	FadeRenderer_->SetImage("BlackOut.png");
	FadeRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 60.f, 8.99f));
	FadeRenderer_->SetAlpha(0.f);
}

void MapBase::Update(float _DeltaTime)
{
	if (true == FadeOutStart_)
	{
		FadeDeltaTime_ -= _DeltaTime;

		float Alpha = FadeDeltaTime_ / FadeTime_;

		if (0.f <= FadeDeltaTime_)
		{
			FadeRenderer_->SetAlpha(Alpha);
		}
		else
		{
			FadeOutStart_ = false;
			FadeRenderer_->SetAlpha(0.f);
		}
	}
	
	if (true == FadeInStart_)
	{
		FadeDeltaTime_ -= _DeltaTime;

		float Alpha = FadeDeltaTime_ / FadeTime_;

		if (0.f <= FadeDeltaTime_)
		{
			FadeRenderer_->SetAlpha(1.f - Alpha);
		}
		else
		{
			FadeInStart_ = false;
			FadeRenderer_->SetAlpha(1.f);
		}
	}
}

void MapBase::CreateGate(bool _Left, bool _Right, bool _Up, bool _Down)
{
	if (true == _Left)
	{
		Gate* Gate1 = GetLevel()->CreateActor<Gate>();
		Gate1->GetTransform()->AttachTransform(GetTransform());
		Gate1->GetTransform()->SetLocalPosition(float4(-455.f, -65.f, -1.f));
		Gate1->SetGateType(GateType::Left);
		Gate_.push_back(Gate1);
	}

	if (true == _Right)
	{
		Gate* Gate1 = GetLevel()->CreateActor<Gate>();
		Gate1->GetTransform()->AttachTransform(GetTransform());
		Gate1->GetTransform()->SetLocalPosition(float4(445.f, -65.f, -1.f));
		Gate1->SetGateType(GateType::Right);
		Gate_.push_back(Gate1);
	}

	if (true == _Up)
	{
		Gate* Gate1 = GetLevel()->CreateActor<Gate>();
		Gate1->GetTransform()->AttachTransform(GetTransform());
		Gate1->GetTransform()->SetLocalPosition(float4(200.f, 60.f, -1.f));
		Gate1->SetGateType(GateType::Up);
		Gate_.push_back(Gate1);
	}

	if (true == _Down)
	{
		Gate* Gate1 = GetLevel()->CreateActor<Gate>();
		Gate1->GetTransform()->AttachTransform(GetTransform());
		Gate1->GetTransform()->SetLocalPosition(float4(-200.f, -180.f, -1.f));
		Gate1->SetGateType(GateType::Down);
		Gate_.push_back(Gate1);
	}
}

void MapBase::CreateBossGate(BossType _Type)
{
	Gate* Gate1 = GetLevel()->CreateActor<Gate>();
	Gate1->GetTransform()->AttachTransform(GetTransform());
	Gate1->GetTransform()->SetLocalPosition(float4(0.f, -60.f, -1.f));
	Gate1->SetGateType(GateType::BossIn);
	Gate_.push_back(Gate1);	
	Type_ = _Type;
}

void MapBase::CreateBossOutGate()
{
	Gate* Gate1 = GetLevel()->CreateActor<Gate>();
	Gate1->GetTransform()->AttachTransform(GetTransform());
	Gate1->GetTransform()->SetLocalPosition(float4(0.f, -60.f, -1.f));
	Gate1->SetGateType(GateType::BossOut);
	Gate_.push_back(Gate1);
}

void MapBase::GateOpen()
{
	std::vector<class Gate*>::iterator iter = Gate_.begin();

	for (; iter != Gate_.end(); ++iter)
	{
		(*iter)->Open();
	}
}

void MapBase::GateClose()
{
	std::vector<class Gate*>::iterator iter = Gate_.begin();

	for (; iter != Gate_.end(); ++iter)
	{
		(*iter)->Close();
	}
}

void MapBase::FadeOut(float _Time)
{
	FadeOutStart_ = true;
	FadeTime_ = _Time;
	FadeDeltaTime_ = _Time;
}
