#include "Precompile.h"
#include "GameManager.h"
#include "Player.h"
#include "Item.h"
#include "Inventory.h"
#include "AvatarInven.h"
#include "InventorySlot.h"
#include "TopUI.h"
#include "SkillInfo.h"

#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

GameManager* GameManager::Inst = new GameManager();

GameManager::GameManager()
	: Player_(nullptr)
	, PlayerPos_(float4::ZERO)
	, PlayerZPos_(0.f)
	, Inven_(nullptr)
	, AvatarInven_(nullptr)
	, InvenOn_(false)
	, Sound_(nullptr)
	, AvatarInvenOn_(false)
	, UIPos_(float4(250.f, 275.f, -10.f))
{
}

GameManager::~GameManager()
{
	for (const std::pair<std::string, Item*>& Res : ItemList_)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ItemList_.clear();

	for (const std::pair<std::string, SkillInfo*>& Res : SkillList_)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	SkillList_.clear();
}

void GameManager::SetInvenInfo()
{
	Inven_->SlotNameSave();
	InvenEquipInfo_.clear();
	InvenInfo_.clear();
	InvenPortionInfo_.clear();

	std::vector<InventorySlot*> Equip = Inven_->GetEquipList();
	std::vector<InventorySlot*>::iterator EquipIter = Equip.begin();
	std::vector<InventorySlot*>::iterator EquipIterEnd = Equip.end();

	for (; EquipIter != EquipIterEnd; ++EquipIter)
	{
		if (true == (*EquipIter)->GetEmpty())
		{
			InvenEquipInfo_.push_back("None");
		}
		else
		{
			InvenEquipInfo_.push_back((*EquipIter)->GetItemName());
		}
	}

	std::string* SlotName = Inven_->GetSlotName();
	std::string* PortionName = Inven_->GetPotionName();

	for (size_t i = 0; i < 56; i++)
	{
		InvenInfo_.push_back(SlotName[i]);
		InvenPortionInfo_.push_back(PortionName[i]);
	}
}

void GameManager::SetAvatarInfo()
{
	std::vector<InventorySlot*> Equip = AvatarInven_->GetEquipList();
	std::vector<InventorySlot*>::iterator EquipIter = Equip.begin();
	std::vector<InventorySlot*>::iterator EquipIterEnd = Equip.end();

	for (; EquipIter != EquipIterEnd; ++EquipIter)
	{
		if (true == (*EquipIter)->GetEmpty())
		{
			AvatarEquipInfo_.push_back("None");
		}
		else
		{
			AvatarEquipInfo_.push_back((*EquipIter)->GetItemName());
		}
	}

	std::vector<InventorySlot*> Slot = AvatarInven_->GetSlotList();
	std::vector<InventorySlot*>::iterator SlotIter = Slot.begin();
	std::vector<InventorySlot*>::iterator SlotIterEnd = Slot.end();

	for (; SlotIter != SlotIterEnd; ++SlotIter)
	{
		if (true == (*SlotIter)->GetEmpty())
		{
			AvatarInfo_.push_back("None");
		}
		else
		{
			AvatarInfo_.push_back((*SlotIter)->GetItemName());
		}
	}
}

void GameManager::SetTopUISlotInfo()
{
	std::vector<InventorySlot*> Slot = UI_->GetLeftSlotList();
	std::vector<InventorySlot*>::iterator SlotIter = Slot.begin();
	std::vector<InventorySlot*>::iterator SlotIterEnd = Slot.end();

	for (; SlotIter != SlotIterEnd; ++SlotIter)
	{
		if (true == (*SlotIter)->GetEmpty())
		{
			TopUISlotInfo_.push_back("None");
		}
		else
		{
			TopUISlotInfo_.push_back((*SlotIter)->GetItemName());
		}
	}
}

void GameManager::Init()
{
	ItemList();
	CreateKey();
	SkillList();
	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void GameManager::Update()
{
	PlayerPos_ = Player_->GetBodyCollision()->GetTransform()->GetWorldPosition();

	if (true == GameEngineInput::GetInst().Down("i"))
	{
		InvenUI();
	}

	if (true == GameEngineInput::GetInst().Down("u"))
	{
		AvatarUI();
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		InvenOn_ = false;
		Inven_->GetTransform()->SetWorldPosition(float4(10000.f, 10000.f, 10.f));

		AvatarInvenOn_ = false;
		AvatarInven_->GetTransform()->SetWorldPosition(float4(10000.f, 10000.f, 10.f));
	}
}

void GameManager::ItemList()
{
	CreateItem("Potion", ItemGrade::Unique, ItemType::Potion);

	CreateItem("Weapon01", ItemGrade::Rare, ItemType::Weapon);
	CreateItem("Weapon02", ItemGrade::Epic, ItemType::Weapon);
	CreateItem("Title00", ItemGrade::Rare, ItemType::Title);
	CreateItem("Title01", ItemGrade::Rare, ItemType::Title);

	CreateItem("Shirt01", ItemGrade::Common, ItemType::Coat);
	CreateItem("Shirt02", ItemGrade::UnCommon, ItemType::Coat);
	CreateItem("Pants01", ItemGrade::Common, ItemType::Pants);
	CreateItem("Pants02", ItemGrade::UnCommon, ItemType::Pants);
	CreateItem("Shoulder01", ItemGrade::Common, ItemType::Shoulder);
	CreateItem("Shoulder02", ItemGrade::UnCommon, ItemType::Shoulder);
	CreateItem("Belt01", ItemGrade::Common, ItemType::Belt);
	CreateItem("Belt02", ItemGrade::UnCommon, ItemType::Belt);
	CreateItem("Shoes01", ItemGrade::Common, ItemType::Shoes);
	CreateItem("Shoes02", ItemGrade::UnCommon, ItemType::Shoes);

	CreateItem("Bracelet01", ItemGrade::Rare, ItemType::Bracelet);
	CreateItem("Bracelet02", ItemGrade::Unique, ItemType::Bracelet);
	CreateItem("Necklace01", ItemGrade::Rare, ItemType::Necklace);
	CreateItem("Necklace02", ItemGrade::UnCommon, ItemType::Necklace);
	CreateItem("Ring01", ItemGrade::Rare, ItemType::Ring);
	CreateItem("Ring02", ItemGrade::Unique, ItemType::Ring);

	//CreateItem("Weapon01", ItemGrade::Epic, ItemType::Support);
	//CreateItem("Weapon02", ItemGrade::Rare, ItemType::Support);
	//CreateItem("Weapon01", ItemGrade::Epic, ItemType::MagicStone);
	//CreateItem("Weapon02", ItemGrade::Rare, ItemType::MagicStone);
	//CreateItem("Weapon01", ItemGrade::Epic, ItemType::Earring);
	//CreateItem("Weapon02", ItemGrade::Rare, ItemType::Earring);

	//CreateItem("Ring01", ItemGrade::None, ItemType::Avatar_Aura);
	//CreateItem("Ring02", ItemGrade::None, ItemType::Avatar_Aura);
	//CreateItem("Ring01", ItemGrade::None, ItemType::Avatar_Weapon);
	//CreateItem("Ring02", ItemGrade::None, ItemType::Avatar_Weapon);

	CreateItem("Avatar_Aura00", ItemGrade::None, ItemType::Avatar_Aura);

	CreateItem("Avatar_Skin01", ItemGrade::None, ItemType::Avatar_Skin);
	CreateItem("Avatar_Skin02", ItemGrade::None, ItemType::Avatar_Skin);

	CreateItem("Avatar_Hair01", ItemGrade::None, ItemType::Avatar_Hair);
	CreateItem("Avatar_Hair02", ItemGrade::None, ItemType::Avatar_Hair);
	CreateItem("Avatar_Cap01", ItemGrade::None, ItemType::Avatar_Cap);
	CreateItem("Avatar_Cap02", ItemGrade::None, ItemType::Avatar_Cap);

	CreateItem("Avatar_Face01", ItemGrade::None, ItemType::Avatar_Face);
	CreateItem("Avatar_Face02", ItemGrade::None, ItemType::Avatar_Face);
	CreateItem("Avatar_Neck01", ItemGrade::None, ItemType::Avatar_Neck);
	CreateItem("Avatar_Neck02", ItemGrade::None, ItemType::Avatar_Neck);

	CreateItem("Avatar_Coat01", ItemGrade::None, ItemType::Avatar_Coat);
	CreateItem("Avatar_Coat02", ItemGrade::None, ItemType::Avatar_Coat);
	CreateItem("Avatar_Pants01", ItemGrade::None, ItemType::Avatar_Pants);
	CreateItem("Avatar_Pants02", ItemGrade::None, ItemType::Avatar_Pants);

	CreateItem("Avatar_Belt01", ItemGrade::None, ItemType::Avatar_Belt);
	CreateItem("Avatar_Belt02", ItemGrade::None, ItemType::Avatar_Belt);
	CreateItem("Avatar_Shoes01", ItemGrade::None, ItemType::Avatar_Shoes);
	CreateItem("Avatar_Shoes02", ItemGrade::None, ItemType::Avatar_Shoes);
}

void GameManager::CreateKey()
{
	GameEngineInput::GetInst().CreateKey("Space", VK_SPACE);
	GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);
	GameEngineInput::GetInst().CreateKey("MouseLButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("MouseRButton", VK_RBUTTON);
	GameEngineInput::GetInst().CreateKey("Insert", VK_INSERT);
	GameEngineInput::GetInst().CreateKey("MoveLeft", VK_LEFT);
	GameEngineInput::GetInst().CreateKey("MoveRight", VK_RIGHT);
	GameEngineInput::GetInst().CreateKey("MoveUp", VK_UP);
	GameEngineInput::GetInst().CreateKey("MoveDown", VK_DOWN);
	GameEngineInput::GetInst().CreateKey("Attack", 'x');
	GameEngineInput::GetInst().CreateKey("Jump", 'c');
	GameEngineInput::GetInst().CreateKey("LevelControl", 'i');
	GameEngineInput::GetInst().CreateKey("MOn", 'o');
	GameEngineInput::GetInst().CreateKey("MOff", 'p');
	GameEngineInput::GetInst().CreateKey("Enter", VK_RETURN);
	GameEngineInput::GetInst().CreateKey("1", 0x31);
	GameEngineInput::GetInst().CreateKey("2", 0x32);
	GameEngineInput::GetInst().CreateKey("3", 0x33);
	GameEngineInput::GetInst().CreateKey("4", 0x34);
	GameEngineInput::GetInst().CreateKey("5", 0x35);
	GameEngineInput::GetInst().CreateKey("6", 0x36);
	GameEngineInput::GetInst().CreateKey("u", 'u');
	GameEngineInput::GetInst().CreateKey("q", 'q');
	GameEngineInput::GetInst().CreateKey("w", 'w');
	GameEngineInput::GetInst().CreateKey("e", 'e');
	GameEngineInput::GetInst().CreateKey("a", 'a');
	GameEngineInput::GetInst().CreateKey("r", 'r');
	GameEngineInput::GetInst().CreateKey("f", 'f');
	GameEngineInput::GetInst().CreateKey("s", 's');
	GameEngineInput::GetInst().CreateKey("d", 'd');
	GameEngineInput::GetInst().CreateKey("x", 'x');
	GameEngineInput::GetInst().CreateKey("i", 'i');
	GameEngineInput::GetInst().CreateKey("m", 'm');
	GameEngineInput::GetInst().CreateKey("t", 't');
	GameEngineInput::GetInst().CreateKey("y", 'y');
	GameEngineInput::GetInst().CreateKey("g", 'g');
	GameEngineInput::GetInst().CreateKey("h", 'h');
	GameEngineInput::GetInst().CreateKey("ctrl", VK_CONTROL);
	GameEngineInput::GetInst().CreateKey("shift", VK_SHIFT);
	GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
}

void GameManager::SkillList()
{
	CreateSkill("flowmindone",		2.f,	3,	1111);
	CreateSkill("flowmindtwo",		3.f,	4,	2222);
	CreateSkill("flowmindthree",	4.f,	5,	3333);
	CreateSkill("tripleslash",		5.f,	3,	4444,	true);
	CreateSkill("chargecrash",		6.f,	8,	6666);
	CreateSkill("momentaryslash",	7.f,	10,	9999);
	CreateSkill("guard",			3.f,	0,	1111);
	CreateSkill("MindSword",		3.f,	3,	8888);
	CreateSkill("Gorecross",		3.f,	5,	2222);

	CreateSkill("TelekineticSwords", 3.f, 0, 1000); 
	CreateSkill("EtherealSlash", 3.f, 0, 1000);
	CreateSkill("Pentastrike", 3.f, 0, 1000);
	CreateSkill("ChargeBurst", 3.f, 0, 1000);
	CreateSkill("Lightning", 3.f, 0, 1000);	
}

void GameManager::CreateItem(const std::string& _Name, ItemGrade _Grade, ItemType _Type)
{
	Item* NewItem = new Item;
	NewItem->SetName(_Name);
	NewItem->SetGrade(_Grade);
	NewItem->SetType(_Type);
	ItemList_.insert(std::map<std::string, Item*>::value_type(_Name, NewItem));
}

Item* GameManager::FindItem(const std::string& _Name)
{
	std::map<std::string, Item*>::iterator FindIter = ItemList_.find(_Name);

	if (FindIter != ItemList_.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

void GameManager::CreateSkill(const std::string& _Name, float _CoolTime, int _Damage, int _Mp, bool _EndCoolTime)
{
	SkillInfo* NewSkill = new SkillInfo;
	NewSkill->SetName(_Name);
	NewSkill->SetCoolTime(_CoolTime, _EndCoolTime);
	NewSkill->SetMp(_Mp);
	NewSkill->SetDamage(_Damage);
	SkillList_.insert(std::map<std::string, SkillInfo*>::value_type(_Name, NewSkill));
}

SkillInfo* GameManager::FindSkill(const std::string& _Name)
{
	std::map<std::string, SkillInfo*>::iterator FindIter = SkillList_.find(_Name);

	if (FindIter != SkillList_.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

void GameManager::ChangeItem(const std::string& _Name, ItemType _Type)
{
	Player_->ChangeItemImage(_Name, _Type);
}

void GameManager::AvatarUI()
{
	Sound_->PlayOverLap("click1.ogg", 0);

	if (false == AvatarInvenOn_)
	{
		if (true == InvenOn_)
		{
			InvenOn_ = false;
			UIPos_ = Inven_->GetTransform()->GetWorldPosition();
		}

		AvatarInvenOn_ = true;
		AvatarInven_->GetTransform()->SetWorldPosition(UIPos_);

		Inven_->GetTransform()->SetWorldPosition(float4(10000.f, 10000.f, -10.f));
	}
	else
	{
		AvatarInvenOn_ = false;
		UIPos_ = AvatarInven_->GetTransform()->GetWorldPosition();
		AvatarInven_->GetTransform()->SetWorldPosition(float4(10000.f, 10000.f, -10.f));
	}
}

void GameManager::InvenUI()
{
	Sound_->PlayOverLap("click1.ogg", 0);

	if (false == InvenOn_)
	{
		if (true == AvatarInvenOn_)
		{
			AvatarInvenOn_ = false;
			UIPos_ = AvatarInven_->GetTransform()->GetWorldPosition();
		}

		InvenOn_ = true;
		Inven_->GetTransform()->SetWorldPosition(UIPos_);

		AvatarInven_->GetTransform()->SetWorldPosition(float4(10000.f, 10000.f, -10.f));
	}
	else
	{
		InvenOn_ = false;
		UIPos_ = Inven_->GetTransform()->GetWorldPosition();
		Inven_->GetTransform()->SetWorldPosition(float4(10000.f, 10000.f, -10.f));
	}
}

void GameManager::LevelChangeEndEvent()
{
	SetInvenInfo();
	SetAvatarInfo();
	SetTopUISlotInfo();
}

void GameManager::ChangeSound(const std::string& _Name)
{
	Sound_->Stop();

	if ("x" == _Name)
	{
		return;
	}

	Sound_->PlayOverLap(_Name, 100);
	Sound_->SetVolume(0.2f);
}
