#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class SeriaRoom : public GameEngineActor
{
public:
	SeriaRoom();
	~SeriaRoom();

protected:
	GameEngineImageRenderer* ImageRenderer_;
	float AlphaTime_;
	bool Check_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
};