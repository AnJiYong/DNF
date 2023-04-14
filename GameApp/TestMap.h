#pragma once
#include "MapBase.h"

class GameEngineImageRenderer;
class TestMap : public MapBase
{
public:
	TestMap();
	~TestMap();

protected:
	GameEngineImageRenderer* BackRenderer_;
	GameEngineImageRenderer* FrontRenderer_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void MapStart(int _MapCount = 0) override;
	void SetMap(const std::string& _Name);
};