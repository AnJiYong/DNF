#include "Precompile.h"
#include "Pase2Rythem.h"
#include "GlobalEnum.h"

#include "GameEngine\GameEngineImageRenderer.h"

Pase2Rythem::Pase2Rythem()
	: ImageRenderer_(nullptr)
	, Color_(0)
{
}

Pase2Rythem::~Pase2Rythem()
{
}

void Pase2Rythem::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));
	ImageRenderer_->CreateAnimationFolder("BlueRythem", "PeterThePiperEffect2", 0.1f, 12, 16);
	ImageRenderer_->CreateAnimationFolder("RedRythem", "PeterThePiperEffect2", 0.1f, 17, 21);
	ImageRenderer_->CreateAnimationFolder("GreenRythem", "PeterThePiperEffect2", 0.1f, 22, 26);
	ImageRenderer_->SetChangeAnimation("BlueRythem");
}

void Pase2Rythem::Update(float _DeltaTime)
{
}

void Pase2Rythem::SetAnimationIndex(int _Index)
{
	if (_Index == 0)
	{
		ImageRenderer_->SetChangeAnimation("BlueRythem");
		Color_ = _Index;
	}
	else if (_Index == 1)
	{
		ImageRenderer_->SetChangeAnimation("RedRythem");
		Color_ = _Index;
	}
}

void Pase2Rythem::Clear()
{
	ImageRenderer_->SetChangeAnimation("GreenRythem");
}
