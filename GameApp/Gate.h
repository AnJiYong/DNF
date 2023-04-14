#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

class Gate : public GameEngineActor
{
public:
	Gate();
	~Gate();

protected:
	class GameEngineImageRenderer* ImageRenderer_;
	class GameEngineImageRenderer* UpImageRenderer_;
	class GameEngineCollision* ImageCollision;

	bool Open_;
	GateType Type_;
	float ChangeTime_;

public:
	void SetGateType(GateType _GateType);

	void Open()
	{
		Open_ = true;
	}

	void Close()
	{
		Open_ = false;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void NextDungeon();
};

