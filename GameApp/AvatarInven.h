#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

class GameEngineUIRenderer;
class GameEngineCollision;
class InventoryButton;
class InventorySlot;
class AvatarUI;
class AvatarInven : public GameEngineActor
{
public:
	AvatarInven();
	~AvatarInven();

protected:
	GameEngineCollision* TopBarCollision_;

	GameEngineCollision* TopButtonCollision_;

	bool IsMove_;
	GameEngineCollision* MouseCollision_;

	InventoryButton* TopButton_[5];
	InventoryButton* MiddleButton_[2];
	std::vector<InventorySlot*> SlotList_;
	std::vector<InventorySlot*> EquipList_;
	std::unordered_map<ItemType, InventorySlot*> EquipEnumList_;
	float4 MoveVector_;
	AvatarUI* Avatar_;

public:
	AvatarUI* GetAvatarUI()
	{
		return Avatar_;
	}

	std::vector<InventorySlot*> GetSlotList()
	{
		return SlotList_;
	}

	std::vector<InventorySlot*> GetEquipList()
	{
		return EquipList_;
	}

	std::unordered_map<ItemType, InventorySlot*> GetEquipEnumList()
	{
		return EquipEnumList_;
	}

	void FirstInit();
	void SetItemSlot();

private:
	void Start() override;
	void Update(float _Time) override;

	void BackGround();
	void TopButton();
	void MiddleButton();
	void ButtomBox();

	void TopBar(GameEngineCollision* _Other);
	void ChangeInvenType(GameEngineCollision* _Other);
};

