#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineCollision;
class GameEngineUIRenderer;
class NpcDialog : public GameEngineActor
{
public:
	NpcDialog();
	~NpcDialog();

private:
	GameEngineUIRenderer* DialogRenderer_;
	GameEngineUIRenderer* ButtonRenderer_;
	GameEngineUIRenderer* ImageRenderer_;
	GameEngineUIRenderer* NameRenderer_;
	GameEngineCollision* ButtonCollision_;
	std::string NpcName_;
	float Alpha_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void MouseOn(GameEngineCollision* _Other);

public:
	void SetNpcName(std::string _Name);
};

