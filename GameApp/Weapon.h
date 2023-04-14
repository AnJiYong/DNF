#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class Weapon : public GameEngineActor
{
public:
	Weapon();
	~Weapon();

private:
	GameEngineImageRenderer* FrontImageRenderer1;
	GameEngineImageRenderer* FrontImageRenderer2;
	GameEngineImageRenderer* BackImageRenderer1;
	GameEngineImageRenderer* BackImageRenderer2;

	int Index_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SetIndex(int _Index)
	{
		Index_ = _Index;
	}

	void ChangeWeapon(std::string _Name);
	void SetReverse(bool _Reverse);
	void SetRotation(float4 _Value);
};

