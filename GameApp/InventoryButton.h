#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineUIRenderer;
class GameEngineCollision;
class InventoryButton : public GameEngineActor
{
public:
	InventoryButton();
	~InventoryButton();

protected:
	GameEngineUIRenderer* LeftImage_;
	GameEngineUIRenderer* MiddleImage_[5];
	GameEngineUIRenderer* RightImage_;
	bool ButtonOn_;
	std::string Text_;

private:
	void Start() override;
	void Update(float _Time) override;

public:
	void SetText(std::string _Text);
	void ButtonOn();
	void ButtonOff();
};

