#pragma once

enum class CollisionOrder
{
	None,
	Player,
	Weapon,
	NPC,
	Potal,
	Monster,
	MonsterTrace,
	MonsterAttack,
	Mouse,
	MouseSub,
	UI,
	Slot,
	Trigger,
	Fire
};

enum class ItemGrade
{
	None,
	Common,
	UnCommon,
	Rare,
	Chronicle,
	Unique,
	Legendary,
	Epic
};

enum class ItemType
{
	None,
	Potion,
	Weapon,
	Title,
	Coat,
	Pants,
	Shoulder,
	Belt,
	Shoes,
	Bracelet,
	Necklace,
	Ring,
	Support,
	MagicStone,
	Earring,
	Avatar_Weapon,
	Avatar_Hair,
	Avatar_Cap,
	Avatar_Face,
	Avatar_Aura,
	Avatar_Neck,
	Avatar_Coat,
	Avatar_Skin,
	Avatar_Pants,
	Avatar_Belt,
	Avatar_Shoes,
};

enum class InvenType
{
	None,
	Inventory,
	Avatar,
	Creature,
	Talisman,
	Insignia
};

enum class GateType
{
	None,
	Left,
	Right,
	Up,
	Down,
	BossIn,
	BossOut
};

enum class BossType
{
	None,
	PeterThePiper,
	IsangomaNazaloh
};

enum class ZOrder
{
	GroundEffectUp = 100,
	BackGroundUp,
	Warnimg,
	GroundEffect,
	BackGroundDown
};

class AttackInfo
{
private:
	std::string Name_;
	int Damage_;
	int Count_;

public:
	void SetAttackInfo(const std::string& _Name, int _Damage, int _Count)
	{
		Name_ = _Name;
		Damage_ = _Damage;
		Count_ = _Count;
	}

	std::string GetName()
	{
		return Name_;
	}

	int GetDamage()
	{
		return Damage_;
	}

	void MulDamage(float _Value)
	{
		float Mul = static_cast<float>(Damage_)* _Value;
		Damage_ = static_cast<int>(Mul);
	}

	int GetCount()
	{
		return Count_;
	}

	int GetPlusCount()
	{
		++Count_;
		return Count_;
	}
};