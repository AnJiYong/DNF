#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineUIRenderer;
class MiniMap : public GameEngineActor
{
public:
	MiniMap();
	~MiniMap();

private:
	GameEngineUIRenderer* MiniMapRenderer_;

	GameEngineUIRenderer* RoomSubRenderer_[25];
	GameEngineUIRenderer* RoomRenderer_[25];
	GameEngineUIRenderer* BossMapRenderer_;
	GameEngineUIRenderer* PlayerRenderer_;
	
	bool MapClear_;
	bool Check_;
	float Time_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void RoomOn(int _Index);
	void SubRoomOff();

	void MapClear(bool _Value)
	{
		MapClear_ = _Value;
	}
};

