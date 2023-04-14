#pragma once
#include "GameEngine/GameEngineLevel.h"

class TownLevel : public GameEngineLevel
{
public:
	TownLevel();
	~TownLevel();

private:
	class Mouse* Mouse_;
	class Player* CurPlayer_;
	bool StartCheck_;

private:
	void CreateActorLevel();

	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	void Teleport();
};

