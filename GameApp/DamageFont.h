#pragma once
#include "GameEngine/GameEngineActor.h"

class DamageFont : public GameEngineActor
{
public:
	DamageFont();
	~DamageFont();

private:
	int Damage_;
	float DeleteTime_;
	std::vector<int> DamageNumber_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void DamageSetting(int _Damage, bool _Critical);
};

