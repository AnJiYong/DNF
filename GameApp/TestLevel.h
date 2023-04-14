#pragma once
#include "GameEngine/GameEngineLevel.h"

class TestLevel : public GameEngineLevel
{
public:
	TestLevel();
	~TestLevel();

private:
	class Player* CurPlayer_;
	class MapBase* CurMap_;

protected:
	void CreateActorLevel();
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	void MapChange();
};

