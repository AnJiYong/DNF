#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class Title : public GameEngineActor
{
public:
	Title();
	~Title();

protected:
	GameEngineImageRenderer* imageRenderer_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
};