#pragma once
#include "GlobalEnum.h"
#include "GameEngine/GameEngineActor.h"

class GameEngineCollision;
class GameEngineUIRenderer;
class GameEngineProgressBarRenderer;
class InventorySlot : public GameEngineActor
{
public:
	InventorySlot();
	~InventorySlot();

protected:
	GameEngineCollision* MainCollision_;
	GameEngineCollision* MoveCollision_;
	GameEngineCollision* MouseCollision_;
	GameEngineCollision* SlotCollision_;

	GameEngineUIRenderer* MainUIRenderer_;
	GameEngineUIRenderer* ItemIconRenderer_;
	GameEngineUIRenderer* GradeUIRenderer;
	GameEngineUIRenderer* SelectUIRenderer;

	GameEngineProgressBarRenderer* CoolTimeRenderer_;

	ItemType SlotType_;
	InvenType InvenType_;

	bool Block_;
	bool Empty_;
	bool GetItem_;
	float CoolTime_;
	float CoolTimeMax_;
	class Item* SlotItem_;
	class Item* PrevItem_;

public:
	bool GetEmpty()
	{
		return Empty_;
	}

	Item* GetSlotItem()
	{
		return SlotItem_;
	}

	std::string GetItemName();

	ItemType GetSlotType()
	{
		return SlotType_;
	}

	void SetSlotType(ItemType _SlotType)
	{
		SlotType_ = _SlotType;
	}

	void SetInvenType(InvenType _InvenType)
	{
		InvenType_ = _InvenType;
	}

private:
	void Start() override;
	void Update(float _Time) override;

public:
	void SetItem(const std::string& _Name);
	void GetItem(GameEngineCollision* _Other);
	void UseItem();
	void ChangeSlot(GameEngineCollision* _Other);
	void SetTopUISlot();
	void CoolTime(float _Time);
};

