#pragma once
#include "MapBase.h"

class GameEngineImageRenderer;
class TestBossMap : public MapBase
{
public:
	TestBossMap();
	~TestBossMap();

protected:
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* Map1Renderer_;
	GameEngineImageRenderer* Map2Renderer_;
	BossType Type_;

public:
	void SetBossType(BossType _Type)
	{
		Type_ = _Type;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void MapStart(int _MapCount = 0) override;
	void MapColorReverse(bool _On);
};