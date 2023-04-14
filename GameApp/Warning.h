#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineImageRenderer;
class Warning : public GameEngineActor
{
public:
	Warning();
	~Warning();

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineImageRenderer* ImageRenderer_;
	GameEngineImageRenderer* RangeRenderer_;

	GameEngineImageRenderer* TopRenderer_;
	GameEngineImageRenderer* MiddleRenderer_;
	GameEngineImageRenderer* BottomRenderer_;

	float OffTime_;
	bool On_;
	bool Circle_;
	float CircleTime_;

public:
	void WarningAnimation(std::string _Name);
	void WarningIine(float4 _Start, float4 _End, float _Height);
	void WarningCircle(float4 _Pos);
};

