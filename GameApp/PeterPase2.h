#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class PeterPase2 : public GameEngineActor
{
public:
	PeterPase2();
	~PeterPase2();

private:
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* Renderer_;
	GameEngineSoundPlayer* Sound_;
	GameEngineCollision* BodyCollision_;
	GameEngineCollision* TriggerCollision_;
	class PeterThePiper* Peter_;
	bool IsUp_;
	int Color_;

public:
	void SetPeter(PeterThePiper* _Peter)
	{
		Peter_ = _Peter;
	}

	bool GetIsUp()
	{
		return IsUp_;
	}

	int GetColor()
	{
		return Color_;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Catch();
	void MonsterDeath();

public:
	void Up();
	void Gray();
	void White();
};