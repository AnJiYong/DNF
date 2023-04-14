#pragma once
#include "GameEngine/GameEngineActor.h"

class GameEngineUIRenderer;
class GameEngineCollision;
class GameEngineProgressBarRenderer;
class TopUI : public GameEngineActor
{
public:
	TopUI();
	~TopUI();

protected:
	GameEngineUIRenderer* TopRenderer;
	GameEngineUIRenderer* ItemRenderer;
	GameEngineUIRenderer* HPBarPercent_;
	GameEngineUIRenderer* MPBarPercent_;
	GameEngineCollision* InvenCollision_;
	GameEngineProgressBarRenderer* HPBar_;
	GameEngineProgressBarRenderer* MPBar_;
	GameEngineProgressBarRenderer* HPBarBack_;
	GameEngineProgressBarRenderer* MPBarBack_;
	class MonsterHpBar* MonsterHpBar_;
	class MiniMap* MiniMap_;
	float Time;
	bool Test;
	bool UIOn;
	float PrevHP_;
	float PrevMP_;
	float HPTime_;
	float MPTime_;
	std::map<std::string, class SkillSlot*> SkillSlot_;
	std::vector<class InventorySlot*> LeftSlotList_;

public:
	std::map<std::string, class SkillSlot*> GetSkillSlot()
	{
		return SkillSlot_;
	}

	std::vector<InventorySlot*> GetLeftSlotList()
	{
		return LeftSlotList_;
	}

	class MiniMap* GetMiniMap()
	{
		return MiniMap_;
	}

private:
	void Start() override;
	void Update(float _Time) override;	

	void SkillSlotSetting();
	void LeftSlotSetting();
	void Hud();

	void MyButton(GameEngineCollision* _Other);
	void InvenButton(GameEngineCollision* _Other);

public:
	void TownSet(bool _IsTown);
	void SetLeftSlot();
	void MonsterHPBarOn(class MonsterBase* _Monster, bool _Boss = false);
	void MonsterHPBarOff();
	void UIOnOff(bool _On);
};

