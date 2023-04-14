#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

class GameEngineImageRenderer;
class MapBase : public GameEngineActor
{
public:
	MapBase();
	~MapBase();

protected:
	int MonsterCount_;
	bool IsBossRoom_;
	bool BossDeath_;
	bool Clear_;
	bool FadeOutStart_;
	bool FadeInStart_;
	float FadeTime_;
	float FadeDeltaTime_;
	BossType Type_;

	std::vector<class Gate*> Gate_;
	GameEngineImageRenderer* FadeRenderer_;

public:
	BossType GetBossType()
	{
		return Type_;
	}

	void Clear()
	{
		Clear_ = true;
	}

	bool MapClear()
	{
		return true == BossDeath_ || 0 == MonsterCount_;
	}

	void MonsterCountPlus()
	{
		++MonsterCount_;
	}

	void MonsterDeath()
	{
		--MonsterCount_;
	}

	void IsBossRoom()
	{
		IsBossRoom_ = true;
	}

	void BossDeath()
	{
		BossDeath_ = true;
		CreateBossOutGate();
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	virtual void MapStart(int _MapCount = 0) {};

	void CreateGate(bool _Left, bool _Right, bool _Up, bool _Down);
	void CreateBossGate(BossType _Type);
	void CreateBossOutGate();
	void GateOpen();
	void GateClose();
	void FadeOut(float _Time);
};