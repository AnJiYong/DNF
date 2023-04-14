#pragma once
#include "GlobalEnum.h"

class Item
{
public:
	Item();
	~Item();

public:
	std::string Name_;
	ItemGrade Grade_;
	ItemType Type_;
	float CoolTime_;

public:
	void SetName(const std::string& _Name)
	{
		Name_ = _Name;
	}

	void SetGrade(ItemGrade _Grade)
	{
		Grade_ = _Grade;
	}

	void SetType(ItemType _Type)
	{
		Type_ = _Type;

		if (_Type == ItemType::Potion)
		{
			CoolTime_ = 2.f;
		}
	}

	ItemType GetType()
	{
		return Type_;
	}

	std::string GetName()
	{
		return Name_;
	}

	std::string GetGrade()
	{
		switch (Grade_)
		{
		case ItemGrade::None:
			return "None";
		case ItemGrade::Common:
			return "Common";
		case ItemGrade::UnCommon:
			return "UnCommon";
		case ItemGrade::Rare:
			return "Rare";
		case ItemGrade::Chronicle:
			return "Chronicle";
		case ItemGrade::Unique:
			return "Unique";
		case ItemGrade::Legendary:
			return "Legendary";
		case ItemGrade::Epic:
			return "Epic";
		default:
			return "";
		}
	}
};

