#pragma once
#include "GameEngine/GameEngineActor.h"

class NpcEvent;
class GameEngineCollision;
class GameEngineUIRenderer;
class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class LiaRichter : public GameEngineActor
{
public:
	LiaRichter();
	~LiaRichter();

protected:
	GameEngineUIRenderer* TextRenderer_;
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* DungeonStartBackRenderer_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* OutlineRenderer_[9];
	GameEngineCollision* ImageCollision_;
	GameEngineCollision* DungeonStartCollision_;

	NpcEvent* Event_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void OutLine(GameEngineCollision* _Other);
	void DungeonStartMouseOn(GameEngineCollision* _Other);
};