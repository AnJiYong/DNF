#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

enum class InvenSlotType
{
	None,
	Weapon,
	Potion
};

class GameEngineUIRenderer;
class GameEngineCollision;
class InventoryButton;
class InventorySlot;
class AvatarUI;
class Inventory : public GameEngineActor
{
public:
	Inventory();
	~Inventory();

protected:
	GameEngineCollision* TopBarCollision_;

	GameEngineCollision* TopButtonCollision_;
	GameEngineCollision* MiddleButtonCollision1_;
	GameEngineCollision* MiddleButtonCollision2_;

	bool IsMove_;
	GameEngineCollision* MouseCollision_;

	AvatarUI* Avatar_;

	InventoryButton* TopButton_[5];
	InventoryButton* MiddleButton_[5];
	std::vector<InventorySlot*> SlotList_;
	std::vector<InventorySlot*> EquipList_;
	std::unordered_map<ItemType, InventorySlot*> EquipEnumList_;
	float4 MoveVector_;

	std::string Slot_[56];
	std::string Potion_[56];
	InvenSlotType InvenSlotType_;

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

	std::string* GetSlotName()
	{
		return Slot_;
	}

	std::string* GetPotionName()
	{
		return Potion_;
	}

	void FirstInit();
	void SetItemSlot();
	void SlotNameSave();

private:
	void Start() override;
	void Update(float _Time) override;

	void BackGround();
	void TopButton();
	void MiddleButton();
	void ButtomBox();

	void TopBar(GameEngineCollision* _Other);
	void ChangeWeaponSlot(GameEngineCollision* _Other);
	void ChangePotionSlot(GameEngineCollision* _Other);
	void ChangeInvenType(GameEngineCollision* _Other);

public:
	void WeaponSlot();
	void PotionSlot();
};

