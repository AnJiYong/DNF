#pragma once
#include "GameEngine/GameEngineLevel.h"

class TitleLevel : public GameEngineLevel
{
public:
	TitleLevel();
	~TitleLevel();

protected:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	void ResourcesLoad();
	void LevelLoad();

private:
	bool ResourcesLoad_;
	bool LoadEnd_;
};

