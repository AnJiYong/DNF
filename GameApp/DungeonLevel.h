#pragma once
#include "GameEngine/GameEngineLevel.h"
#include "GlobalEnum.h"

class DungeonLevel : public GameEngineLevel
{
public:
	DungeonLevel();
	~DungeonLevel();

private:
	class Player* CurPlayer_;
	class MapBase* CurMap_;
	class MiniMap* MiniMap_;
	int DungeonNumber_;
	bool FadeOn_;
	float FadeTime_;

	std::vector<class MapBase*> MapList_;
	float4 MapPivot_;
	float4 PlayerPos_;
	bool IsBossRoom_;
	bool BossRoomIn_;

public:
	class MapBase* GetCurMap()
	{
		return CurMap_;
	}

protected:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:
	void PlayerPosChange();
	void CreateBossRoom(BossType _Type);

public:
	void MapChange(GateType _GateType);
	void FadeOut(float _Time);
};

