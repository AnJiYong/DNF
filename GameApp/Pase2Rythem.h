#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class Pase2Rythem : public GameEngineActor
{
public:
	Pase2Rythem();
	~Pase2Rythem();

private:
	GameEngineImageRenderer* ImageRenderer_;
	int Color_;

public:
	int GetColor()
	{
		return Color_;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SetAnimationIndex(int _Index);
	void Clear();
};