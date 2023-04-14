#pragma once
#include "GlobalEnum.h"

class Player;
class Item;
class SkillInfo;
class GameEngineSoundPlayer;
class GameManager
{
private:
	static GameManager* Inst;

public:
	static GameManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	GameManager();
	~GameManager();

private:
	GameEngineSoundPlayer* Sound_;
	Player* Player_;
	float4 PlayerPos_;
	float PlayerZPos_;
	class TopUI* UI_;
	class Mouse* Mouse_;
	class Inventory* Inven_;
	class AvatarInven* AvatarInven_;
	std::map<std::string, Item*> ItemList_;
	std::map<std::string, SkillInfo*> SkillList_;

	bool InvenOn_;
	bool AvatarInvenOn_;
	float4 UIPos_;

	std::vector<std::string> InvenEquipInfo_;
	std::vector<std::string> InvenInfo_;
	std::vector<std::string> InvenPortionInfo_;
	std::vector<std::string> AvatarEquipInfo_;
	std::vector<std::string> AvatarInfo_;
	std::vector<std::string> TopUISlotInfo_;

public:
	std::vector<std::string> GetInvenEquipInfo()
	{
		return InvenEquipInfo_;
	}

	std::vector<std::string> GetInvenInfo()
	{
		return InvenInfo_;
	}

	std::vector<std::string> GetInvenPortionInfo()
	{
		return InvenPortionInfo_;
	}

	std::vector<std::string> GetAvatarEquipInfo()
	{
		return AvatarEquipInfo_;
	}

	std::vector<std::string> GetAvatarInfo()
	{
		return AvatarInfo_;
	}

	std::vector<std::string> GetTopUISlotInfo()
	{
		return TopUISlotInfo_;
	}

	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

	Player* GetPlayer()
	{
		return Player_;
	}

	float4 GetPlayerPos()
	{
		return PlayerPos_;
	}

	void SetPlayerZPos(float _ZPos)
	{
		PlayerZPos_ = _ZPos;
	}

	float GetPlayerZPos()
	{
		return PlayerZPos_;
	}

	void SetUI(TopUI* _UI)
	{
		UI_ = _UI;
	}

	TopUI* GetUI()
	{
		return UI_;
	}

	void SetMouse(Mouse* _Mouse)
	{
		Mouse_ = _Mouse;
	}

	Mouse* GetMouse()
	{
		return Mouse_;
	}

	void SetInven(Inventory* _Inven)
	{
		Inven_ = _Inven;
	}

	Inventory* GetInven()
	{
		return Inven_;
	}

	void SetAvatarInven(AvatarInven* _AvatarInven)
	{
		AvatarInven_ = _AvatarInven;
	}

	AvatarInven* GetAvatarInven()
	{
		return AvatarInven_;
	}

private:
	void SetInvenInfo();
	void SetAvatarInfo();
	void SetTopUISlotInfo();
	void ItemList();
	void CreateKey();
	void SkillList();

public:
	void Init();
	void Update();
	void CreateItem(const std::string& _Name, ItemGrade _Grade, ItemType _Type);
	Item* FindItem(const std::string& _Name);
	void CreateSkill(const std::string& _Name, float _CoolTime, int _Damage, int _Mp, bool _EndCoolTime = false);
	SkillInfo* FindSkill(const std::string& _Name);
	void ChangeItem(const std::string& _Name, ItemType _Type);

	void AvatarUI();
	void InvenUI();
	void LevelChangeEndEvent();

	void ChangeSound(const std::string& _Name);
};


