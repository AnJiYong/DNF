#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GameEngineBase\GameEngineRandom.h"

class GameEngineImageRenderer;
class Map : public GameEngineActor
{
public:
	Map();
	~Map();

private:
	static Map* CurrentMap;

public:
	static float4 GetColor(GameEngineTransform* _Ptr, bool YRevers = true);
	static float4 GetColor(float4 _Postion);

protected:
	void Start() override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

	GameEngineRandom Random;
	float4 RandomStart;

	void RandomRoad(int Count);
	void RandomRoad();

private:
	GameEngineImageRenderer* ImageRenderer;

public:
	GameEngineImageRenderer* GetImageRenderer()
	{
		return ImageRenderer;
	}
};