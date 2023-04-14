#pragma once

class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class GameEngineCollision;
class AirshipScreen : public GameEngineActor
{
public:
	AirshipScreen();
	~AirshipScreen();

private:
	GameEngineSoundPlayer* Sound_;
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* MainImageRenderer_;
	GameEngineImageRenderer* OutlineRenderer_[9];
	GameEngineCollision* MainCollision_;

	class AirshipScreenUI* MainUI_;

	bool MainUIOn;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void UIOnPlayer(GameEngineCollision* _Other);
	void UIOnMouse(GameEngineCollision* _Other);
};