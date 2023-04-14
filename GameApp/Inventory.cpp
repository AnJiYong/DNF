#include "Precompile.h"
#include "Inventory.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GlobalEnum.h"
#include "InventoryButton.h"
#include "InventorySlot.h"
#include "GameManager.h"
#include "AvatarUI.h"

Inventory::Inventory()
	: TopBarCollision_(nullptr)
	, MoveVector_(float4::ZERO)
	, TopButton_()
	, MiddleButton_()
	, TopButtonCollision_(nullptr)
	, MiddleButtonCollision1_(nullptr)
	, MiddleButtonCollision2_(nullptr)
	, MouseCollision_(nullptr)
	, InvenSlotType_(InvenSlotType::Weapon)
	, IsMove_(false)
{
	for (size_t i = 0; i < 56; i++)
	{
		Slot_[i] = "None";
		Potion_[i] = "None";
	}	
}

Inventory::~Inventory()
{
}

void Inventory::Start()
{
	TopBarCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	TopBarCollision_->GetTransform()->SetLocalScaling(float4(258.f, 26.f, 1.f));
	TopBarCollision_->GetTransform()->SetLocalPosition(float4(0.f, -13.f, -10.f));
	
	TopButtonCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	TopButtonCollision_->GetTransform()->SetLocalScaling(float4(48.f, 20.f, 1.f));
	TopButtonCollision_->GetTransform()->SetLocalPosition(float4(-48.f, -40.f, -10.f));

	MiddleButtonCollision1_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	MiddleButtonCollision1_->GetTransform()->SetLocalScaling(float4(48.f, 20.f, 1.f));
	MiddleButtonCollision1_->GetTransform()->SetLocalPosition(float4(-96.f, -245.f, -10.f));

	MiddleButtonCollision2_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	MiddleButtonCollision2_->GetTransform()->SetLocalScaling(float4(48.f, 20.f, 1.f));
	MiddleButtonCollision2_->GetTransform()->SetLocalPosition(float4(-48.f, -245.f, -10.f));

	BackGround();
	TopButton();
	MiddleButton();
	ButtomBox();

	{
		std::vector<InventorySlot*>::iterator Iter = SlotList_.begin();

		for (; Iter != SlotList_.end(); Iter++)
		{
			(*Iter)->SetInvenType(InvenType::Inventory);
		}
	}

	{
		std::vector<InventorySlot*>::iterator Iter = EquipList_.begin();

		for (; Iter != EquipList_.end(); Iter++)
		{
			(*Iter)->SetInvenType(InvenType::Inventory);
		}
	}

	Avatar_ = GetLevel()->CreateActor<AvatarUI>();
	Avatar_->GetTransform()->AttachTransform(GetTransform());
	Avatar_->GetTransform()->SetLocalPosition(float4(20.f, -125.f, -10.f));
}

void Inventory::Update(float _Time)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(TopBarCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushUIDebugRender(TopButtonCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushUIDebugRender(MiddleButtonCollision1_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushUIDebugRender(MiddleButtonCollision2_->GetTransform(), CollisionType::Rect);
#endif

	TopBarCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&Inventory::TopBar, this, std::placeholders::_1));

	TopButtonCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&Inventory::ChangeInvenType, this, std::placeholders::_1));

	MiddleButtonCollision1_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&Inventory::ChangeWeaponSlot, this, std::placeholders::_1));

	MiddleButtonCollision2_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&Inventory::ChangePotionSlot, this, std::placeholders::_1));

	if (true == IsMove_)
	{
		if (true == GameEngineInput::GetInst().Press("MouseLButton"))
		{
			GetTransform()->SetWorldPosition(MouseCollision_->GetTransform()->GetWorldPosition() + MoveVector_);
		}
		else
		{
			IsMove_ = false;
		}
	}
}

void Inventory::BackGround()
{
	{
		// 백그라운드

		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("Inven03.png");
		UIRneder->GetTransform()->SetLocalPosition(float4(0.f, -277.f, 0.f));
		UIRneder->SetAlpha(0.75f);
	}

	{
		// 상단 바

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven09.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(-129.f + 49.f, -13.f, -1.f));
		
		for (size_t i = 0; i < 8; i++)
		{
			GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			UIRneder1->SetImage("Inven10.png");
			UIRneder1->GetTransform()->SetLocalPosition(float4(-25.f + (12.f * i), -13.f, -1.f));
		}

		GameEngineUIRenderer* UIRneder2 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder2->SetImage("Inven11.png");
		UIRneder2->GetTransform()->SetLocalPosition(float4(129.f - 32.f, -13.f, -1.f));
		UIRneder2->SetOutLine(float4::BLACK);
		UIRneder2->TextSetting("gulim", "장비 인벤토리(I)", 11, float4(110.f / 255.f, 108.f / 255.f, 85.f / 255.f), float4(-100.f, 7.f));
	}

	{
		// 착용 장비 창

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven00.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(0.f, -51.f - 88.5f, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven01.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(0.f, -51.f - 88.5f, 0.f));

		GameEngineUIRenderer* UIRneder2 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder2->SetImage("Inven12.png");
		UIRneder2->GetTransform()->SetLocalPosition(float4(0.f, -59.f - 47.f, -1.f));

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-118.f + 15.f, -59.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Shoulder);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Shoulder, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-118.f + 47.f, -59.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Coat);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Coat, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-118.f + 15.f, -59.f - 47.f, -1.f));
			Slot->SetSlotType(ItemType::Pants);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Pants, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-118.f + 47.f, -59.f - 47.f, -1.f));
			Slot->SetSlotType(ItemType::Belt);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Belt, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-118.f + 15.f, -59.f - 79.f, -1.f));
			Slot->SetSlotType(ItemType::Shoes);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Shoes, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(118.f - 15.f, -59.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Title);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Title, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(118.f - 47.f, -59.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Weapon);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Weapon, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(118.f - 15.f, -59.f - 47.f, -1.f));
			Slot->SetSlotType(ItemType::Necklace);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Necklace, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(118.f - 47.f, -59.f - 47.f, -1.f));
			Slot->SetSlotType(ItemType::Bracelet);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Bracelet, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(118.f - 15.f, -59.f - 79.f, -1.f));
			Slot->SetSlotType(ItemType::Ring);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Ring, Slot));
		}
	}

	{
		// 보유 아이템 창

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven04.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(0.f, -257.f - 121.f, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven05.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(0.f, -258.f - 120.f, -1.f));
	}

	{
		// 구분 선
		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven02.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(0.f, -48.f, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven02.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(0.f, -254.f, -1.f));
	}
}

void Inventory::TopButton()
{
	for (size_t i = 0; i < 5; i++)
	{
		TopButton_[i] = GetLevel()->CreateActor<InventoryButton>();
		TopButton_[i]->GetTransform()->AttachTransform(GetTransform());
		TopButton_[i]->GetTransform()->SetLocalPosition(float4(-96.f + (i * 48.f), -47.f, -1.f));
	}

	TopButton_[0]->ButtonOn();
	TopButton_[0]->SetText("인벤토리");
	TopButton_[1]->SetText("아바타");
	TopButton_[2]->SetText("크리쳐");
	TopButton_[3]->SetText("탈리스만");
	TopButton_[4]->SetText("휘장");

}

void Inventory::MiddleButton()
{
	for (size_t i = 0; i < 5; i++)
	{
		MiddleButton_[i] = GetLevel()->CreateActor<InventoryButton>();
		MiddleButton_[i]->GetTransform()->AttachTransform(GetTransform());
		MiddleButton_[i]->GetTransform()->SetLocalPosition(float4(-96.f + (i * 48.f), -253.f, -1.f));
	}

	MiddleButton_[0]->ButtonOn();
	MiddleButton_[0]->SetText("장비");
	MiddleButton_[1]->SetText("소모품");
	MiddleButton_[2]->SetText("재료");
	MiddleButton_[3]->SetText("전문재료");
	MiddleButton_[4]->SetText("퀘스트");

	for (size_t y = 0; y < 7; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-105.f + (x * 30.f), -275.f - (y * 30.f), -1.f));
			SlotList_.push_back(Slot);
		}
	}
}

void Inventory::ButtomBox()
{
	{
		// 코인 박스
		float XPivot = 1.f;
		float YPivot = 501.f;

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven06.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(-61.5f - XPivot, -11.5f - YPivot, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven07.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(-61.5f - XPivot, -11.5f - YPivot, -1.f));
	}

	{
		// 세라 박스
		float XPivot = 1.f;
		float YPivot = 526.f;

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven06.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(-61.5f - XPivot, -11.5f - YPivot, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven07.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(-61.5f - XPivot, -11.5f - YPivot, -1.f));
	}

	{
		// 결투장 박스
		float XPivot = 1.f;
		float YPivot = 501.f;

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven06.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(61.5f + XPivot, -11.5f - YPivot, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven07.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(61.5f + XPivot, -11.5f - YPivot, -1.f));
	}

	{
		// 골드 박스
		float XPivot = 1.f;
		float YPivot = 526.f;

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven06.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(61.5f + XPivot, -11.5f - YPivot, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven07.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(61.5f + XPivot, -11.5f - YPivot, -1.f));
	}
}

void Inventory::SlotNameSave()
{
	if (InvenSlotType_ == InvenSlotType::Weapon)
	{
		for (size_t i = 0; i < 56; i++)
		{
			if (true == SlotList_[i]->GetEmpty())
			{
				Slot_[i] = "None";
			}
			else
			{
				Slot_[i] = SlotList_[i]->GetItemName();
			}
		}
	}
	else if (InvenSlotType_ == InvenSlotType::Potion)
	{
		for (size_t i = 0; i < 56; i++)
		{
			if (true == SlotList_[i]->GetEmpty())
			{
				Potion_[i] = "None";
			}
			else
			{
				Potion_[i] = SlotList_[i]->GetItemName();
			}
		}
	}
}

void Inventory::TopBar(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		MoveVector_ = GetTransform()->GetWorldPosition() - _Other->GetTransform()->GetWorldPosition();
		MouseCollision_ = _Other;
		IsMove_ = true;
	}
}

void Inventory::ChangeWeaponSlot(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		WeaponSlot();
	}
}

void Inventory::ChangePotionSlot(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		PotionSlot();
	}
}

void Inventory::ChangeInvenType(GameEngineCollision* _Other)
{		
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		GameManager::GetInst().AvatarUI();
	}
}

void Inventory::WeaponSlot()
{
	if (InvenSlotType_ == InvenSlotType::Weapon)
	{
		return;
	}

	InvenSlotType_ = InvenSlotType::Weapon;
	MiddleButton_[0]->ButtonOn();
	MiddleButton_[1]->ButtonOff();

	for (size_t i = 0; i < 56; i++)
	{
		if (true == SlotList_[i]->GetEmpty())
		{
			Potion_[i] = "None";
		}
		else
		{
			Potion_[i] = SlotList_[i]->GetItemName();
		}
	}

	for (size_t i = 0; i < SlotList_.size(); i++)
	{
		SlotList_[i]->SetItem(Slot_[i]);
	}
}

void Inventory::PotionSlot()
{
	if (InvenSlotType_ == InvenSlotType::Potion)
	{
		return;
	}

	InvenSlotType_ = InvenSlotType::Potion;
	MiddleButton_[0]->ButtonOff();
	MiddleButton_[1]->ButtonOn();

	for (size_t i = 0; i < 56; i++)
	{
		if (true == SlotList_[i]->GetEmpty())
		{
			Slot_[i] = "None";
		}
		else
		{
			Slot_[i] = SlotList_[i]->GetItemName();
		}
	}

	for (size_t i = 0; i < SlotList_.size(); i++)
	{
		SlotList_[i]->SetItem(Potion_[i]);
	}
}

void Inventory::FirstInit()
{
	SlotList_[0]->SetItem("Weapon01");
	SlotList_[1]->SetItem("Weapon02");
	SlotList_[2]->SetItem("Shirt01");
	SlotList_[3]->SetItem("Shirt02");
	SlotList_[4]->SetItem("Pants01");
	SlotList_[5]->SetItem("Pants02");
	SlotList_[6]->SetItem("Shoulder01");
	SlotList_[7]->SetItem("Shoulder02");
	SlotList_[8]->SetItem("Belt01");
	SlotList_[9]->SetItem("Belt02");
	SlotList_[10]->SetItem("Shoes01");
	SlotList_[11]->SetItem("Shoes02");
	SlotList_[12]->SetItem("Bracelet01");
	SlotList_[13]->SetItem("Bracelet02");
	SlotList_[14]->SetItem("Necklace01");
	SlotList_[15]->SetItem("Necklace02");
	SlotList_[16]->SetItem("Ring01");
	SlotList_[17]->SetItem("Ring02");
	SlotList_[18]->SetItem("Title00");
	SlotList_[19]->SetItem("Title01");

	for (size_t i = 0; i < 56; i++)
	{
		if (true == SlotList_[i]->GetEmpty())
		{
			Slot_[i] = "None";
		}
		else
		{
			Slot_[i] = SlotList_[i]->GetItemName();
		}
	}

	Potion_[3] = "Potion";
}

void Inventory::SetItemSlot()
{
	std::vector<std::string> Equip = GameManager::GetInst().GetInvenEquipInfo();

	for (size_t i = 0; i < EquipList_.size(); i++)
	{
		EquipList_[i]->SetItem(Equip[i]);
	}

	std::vector<std::string> Slot = GameManager::GetInst().GetInvenInfo();
	std::vector<std::string> Portion = GameManager::GetInst().GetInvenPortionInfo();

	for (size_t i = 0; i < 56; i++)
	{
		Slot_[i] = Slot[i];
		Potion_[i] = Portion[i];
	}

	for (size_t i = 0; i < SlotList_.size(); i++)
	{
		SlotList_[i]->SetItem(Slot_[i]);
	}
}
