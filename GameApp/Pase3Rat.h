#pragma once
#include "MonsterBase.h"

class Pase3Rat : public MonsterBase
{
public:
	Pase3Rat();
	~Pase3Rat();

private:
	GameEngineImageRenderer* ImageRenderer_;
	class PeterThePiper* Peter_;
	float4 MoveVector_;
	bool RatW_;
	bool RatWDeath_;

public:
	void SetPeter(PeterThePiper* _Peter)
	{
		Peter_ = _Peter;
	}

	void SetRatW()
	{
		RatW_ = true;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Catch();
	void Boom();
	void RatWDeath();
};