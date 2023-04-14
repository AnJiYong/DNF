
#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class Pillasia : public GameEngineActor
{
public:
	Pillasia();
	~Pillasia();

protected:
	GameEngineImageRenderer* ImageRenderer_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
};