#include "Precompile.h"
#include "AvatarInven.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GlobalEnum.h"
#include "InventoryButton.h"
#include "InventorySlot.h"
#include "GameManager.h"
#include "AvatarUI.h"

AvatarInven::AvatarInven()
	: TopBarCollision_(nullptr)
	, TopButtonCollision_(nullptr)
	, MouseCollision_(nullptr)
	, MoveVector_(float4::ZERO)
	, TopButton_()
	, MiddleButton_()
	, IsMove_(false)
{
}

AvatarInven::~AvatarInven()
{
}

void AvatarInven::FirstInit()
{
	SlotList_[0]->SetItem("Avatar_Hair01");
	SlotList_[1]->SetItem("Avatar_Cap01");
	SlotList_[2]->SetItem("Avatar_Face01");
	SlotList_[8]->SetItem("Avatar_Neck01");
	SlotList_[9]->SetItem("Avatar_Coat01");
	SlotList_[16]->SetItem("Avatar_Belt01");
	SlotList_[17]->SetItem("Avatar_Pants01");
	SlotList_[18]->SetItem("Avatar_Shoes01");

	SlotList_[32]->SetItem("Avatar_Skin01");
	SlotList_[33]->SetItem("Avatar_Skin02");
	SlotList_[34]->SetItem("Avatar_Aura00");

	SlotList_[5]->SetItem("Avatar_Hair02");
	SlotList_[6]->SetItem("Avatar_Cap02");
	SlotList_[7]->SetItem("Avatar_Face02");
	SlotList_[13]->SetItem("Avatar_Neck02");
	SlotList_[14]->SetItem("Avatar_Coat02");
	SlotList_[21]->SetItem("Avatar_Belt02");
	SlotList_[22]->SetItem("Avatar_Pants02");
	SlotList_[23]->SetItem("Avatar_Shoes02");
}

void AvatarInven::SetItemSlot()
{
	std::vector<std::string> Equip = GameManager::GetInst().GetAvatarEquipInfo();

	for (size_t i = 0; i < EquipList_.size(); i++)
	{
		EquipList_[i]->SetItem(Equip[i]);
	}

	std::vector<std::string> Slot = GameManager::GetInst().GetAvatarInfo();

	for (size_t i = 0; i < SlotList_.size(); i++)
	{
		SlotList_[i]->SetItem(Slot[i]);
	}
}

void AvatarInven::Start()
{
	TopBarCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	TopBarCollision_->GetTransform()->SetLocalScaling(float4(258.f, 26.f, 1.f));
	TopBarCollision_->GetTransform()->SetLocalPosition(float4(0.f, -13.f, -10.f));

	TopButtonCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	TopButtonCollision_->GetTransform()->SetLocalScaling(float4(48.f, 20.f, 1.f));
	TopButtonCollision_->GetTransform()->SetLocalPosition(float4(-96.f, -40.f, -10.f));

	BackGround();
	TopButton();
	MiddleButton();
	ButtomBox();
	
	{
		std::vector<InventorySlot*>::iterator Iter = SlotList_.begin();

		for (; Iter != SlotList_.end(); Iter++)
		{
			(*Iter)->SetInvenType(InvenType::Avatar);
		}
	}

	{
		std::vector<InventorySlot*>::iterator Iter = EquipList_.begin();

		for (; Iter != EquipList_.end(); Iter++)
		{
			(*Iter)->SetInvenType(InvenType::Avatar);
		}
	}

	Avatar_ = GetLevel()->CreateActor<AvatarUI>();
	Avatar_->GetTransform()->AttachTransform(GetTransform());
	Avatar_->GetTransform()->SetLocalPosition(float4(-50.f, -125.f, -10.f));
}

void AvatarInven::Update(float _Time)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(TopBarCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushUIDebugRender(TopButtonCollision_->GetTransform(), CollisionType::Rect);
#endif

	TopBarCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&AvatarInven::TopBar, this, std::placeholders::_1));

	TopButtonCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&AvatarInven::ChangeInvenType, this, std::placeholders::_1));

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

void AvatarInven::BackGround()
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
		UIRneder2->TextSetting("gulim", "아바타 인벤토리(U)", 11, float4(110.f / 255.f, 108.f / 255.f, 85.f / 255.f), float4(-100.f, 7.f));
	}

	{
		// 착용 장비 창

		GameEngineUIRenderer* UIRneder0 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder0->SetImage("Inven13.png");
		UIRneder0->GetTransform()->SetLocalPosition(float4(0.f, -51.f - 88.5f, -1.f));

		GameEngineUIRenderer* UIRneder1 = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder1->SetImage("Inven01.png");
		UIRneder1->GetTransform()->SetLocalPosition(float4(0.f, -51.f - 88.5f, 0.f));

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 15.f, -81.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Weapon);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Weapon, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 48.f, -81.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Hair);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Hair, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 81.f, -81.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Cap);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Cap, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 114.f, -81.f - 15.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Face);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Face, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 15.f, -81.f - 48.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Aura);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Aura, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 48.f, -81.f - 48.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Neck);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Neck, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 81.f, -81.f - 48.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Coat);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Coat, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 114.f, -81.f - 48.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Skin);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Skin, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 48.f, -81.f - 81.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Belt);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Belt, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 81.f, -81.f - 81.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Pants);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Pants, Slot));
		}

		{
			InventorySlot* Slot = GetLevel()->CreateActor<InventorySlot>();
			Slot->GetTransform()->AttachTransform(GetTransform());
			Slot->GetTransform()->SetLocalPosition(float4(-11.f + 114.f, -81.f - 81.f, -1.f));
			Slot->SetSlotType(ItemType::Avatar_Shoes);
			EquipList_.push_back(Slot);
			EquipEnumList_.insert(std::make_pair(ItemType::Avatar_Shoes, Slot));
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

void AvatarInven::TopButton()
{
	for (size_t i = 0; i < 5; i++)
	{
		TopButton_[i] = GetLevel()->CreateActor<InventoryButton>();
		TopButton_[i]->GetTransform()->AttachTransform(GetTransform());
		TopButton_[i]->GetTransform()->SetLocalPosition(float4(-96.f + (i * 48.f), -47.f, -1.f));
	}

	TopButton_[1]->ButtonOn();
	TopButton_[0]->SetText("인벤토리");
	TopButton_[1]->SetText("아바타");
	TopButton_[2]->SetText("크리쳐");
	TopButton_[3]->SetText("탈리스만");
	TopButton_[4]->SetText("휘장");
}

void AvatarInven::MiddleButton()
{
	for (size_t i = 0; i < 2; i++)
	{
		MiddleButton_[i] = GetLevel()->CreateActor<InventoryButton>();
		MiddleButton_[i]->GetTransform()->AttachTransform(GetTransform());
		MiddleButton_[i]->GetTransform()->SetLocalPosition(float4(-96.f + (i * 48.f), -253.f, -1.f));
	}

	MiddleButton_[0]->ButtonOn();
	MiddleButton_[0]->SetText("아바타");
	MiddleButton_[1]->SetText("엠블렘");

	for (size_t y = 0; y < 8; y++)
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

void AvatarInven::ButtomBox()
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

void AvatarInven::TopBar(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		MoveVector_ = GetTransform()->GetWorldPosition() - _Other->GetTransform()->GetWorldPosition();
		MouseCollision_ = _Other;
		IsMove_ = true;
	}
}

void AvatarInven::ChangeInvenType(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		GameManager::GetInst().InvenUI();
	}
}
