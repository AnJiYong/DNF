#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineUIRenderer;
class AirshipScreenUI : public GameEngineActor
{
public:
	AirshipScreenUI();
	~AirshipScreenUI();

protected:
	GameEngineUIRenderer* MapRenderer_;
	GameEngineUIRenderer* TopRenderer_;
	GameEngineUIRenderer* SelectMapRenderer_;
	GameEngineUIRenderer* HexagonRenderer_;
	bool AniEnd_;
	int MapNumber_;

private:
	void Start() override;
	void Update(float _Time) override;

	void AnimationEnd();
};

