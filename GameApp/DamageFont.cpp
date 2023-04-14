#include "Precompile.h"
#include "DamageFont.h"
#include "GameEngine/GameEngineImageRenderer.h"

DamageFont::DamageFont()
	: Damage_(0)
	, DeleteTime_(1.f)
{
}

DamageFont::~DamageFont()
{
}

void DamageFont::Start()
{
}

void DamageFont::Update(float _DeltaTime)
{
	GetTransform()->SetWorldDeltaTimeMove(float4(0.f, 10.f));

	DeleteTime_ -= _DeltaTime;

	if (DeleteTime_ <= 0.f)
	{
		Death();
	}
}

void DamageFont::DamageSetting(int _Damage, bool _Critical)
{
	Damage_ = _Damage;

	std::stack<int>	Number;

	while (Damage_ > 0)
	{
		Number.push(Damage_ % 10);
		Damage_ /= 10;
	}

	while (false == Number.empty())
	{
		DamageNumber_.push_back(Number.top());
		Number.pop();
	}

	for (size_t i = 0; i < DamageNumber_.size(); i++)
	{
		GameEngineImageRenderer* Image = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		
		std::string Damage = "";

		if (true == _Critical)
		{
			Damage = "1";
		}

		std::string Middle = std::to_string(DamageNumber_[i]);
		Damage.append(Middle);
		Damage.append(".png");

		Image->SetImage(Damage);
		Image->GetTransform()->SetLocalPosition(float4(i * 15.f, 0.f, -100.f));
	}
}
