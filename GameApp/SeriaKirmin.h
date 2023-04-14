#pragma once
#include "GameEngine/GameEngineActor.h"

class NpcEvent;
class GameEngineCollision;
class GameEngineUIRenderer;
class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class SeriaKirmin : public GameEngineActor
{
public:
	SeriaKirmin();
	~SeriaKirmin();

protected:
	GameEngineUIRenderer* TextRenderer_;
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* OutlineRenderer_[9];
	GameEngineCollision* ImageCollision_;

	NpcEvent* Event_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void OutLine(GameEngineCollision* _Other);
};