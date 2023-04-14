#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineCollision;
class GameEngineUIRenderer;
class NpcEvent : public GameEngineActor
{
public:
	NpcEvent();
	~NpcEvent();

private:
	GameEngineUIRenderer* ClickRenderer_;
	GameEngineUIRenderer* ButtonRenderer_;
	GameEngineCollision* ButtonCollision_;
	std::string NpcName_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void MouseOn(GameEngineCollision* _Other);

public:
	void SetNpcName(std::string _Name);
};

