#include "Precompile.h"
#include "InventorySlot.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngine/GameEngineProgressBarRenderer.h"
#include "GlobalEnum.h"
#include "GameManager.h"
#include "Item.h"
#include "Inventory.h"
#include "AvatarInven.h"
#include "Player.h"
#include "Mouse.h"

InventorySlot::InventorySlot()
	: MainCollision_(nullptr)
	, MoveCollision_(nullptr)
	, MouseCollision_(nullptr)
	, SlotCollision_(nullptr)
	, MainUIRenderer_(nullptr)
	, ItemIconRenderer_(nullptr)
	, GradeUIRenderer(nullptr)
	, SelectUIRenderer(nullptr)
	, CoolTimeRenderer_(nullptr)
	, SlotType_(ItemType::None)
	, InvenType_(InvenType::Inventory)
	, Empty_(true)
	, GetItem_(false)
	, Block_(false)
	, SlotItem_(nullptr)
	, PrevItem_(nullptr)
	, CoolTime_(0.f)
	, CoolTimeMax_(0.f)
{
}

InventorySlot::~InventorySlot()
{
}

std::string InventorySlot::GetItemName()
{
	return SlotItem_->GetName();
}

void InventorySlot::Start()
{
	MainCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Slot));
	MainCollision_->GetTransform()->SetLocalScaling(float4(28.f, 28.f, 1.f));
	MainCollision_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));

	MoveCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	MoveCollision_->GetTransform()->SetLocalScaling(float4(1.f, 1.f, 1.f));
	MoveCollision_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));

	MainUIRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MainUIRenderer_->SetImage("Inven08.png");
	MainUIRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));

	SelectUIRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	SelectUIRenderer->SetImage("Inven14.png");
	SelectUIRenderer->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));
	SelectUIRenderer->Off();

	CoolTimeRenderer_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
	CoolTimeRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -9.f));
	CoolTimeRenderer_->SetImage("SkillIcon.png");
	CoolTimeRenderer_->SetProgressBarDirect(0);
	CoolTimeRenderer_->Off();
}

void InventorySlot::Update(float _Time)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(MainCollision_->GetTransform(), CollisionType::Rect);
#endif

	SlotCollision_ = MainCollision_;
	SelectUIRenderer->Off();

	MainCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&InventorySlot::GetItem, this, std::placeholders::_1));

	MoveCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Slot)
		, std::bind(&InventorySlot::ChangeSlot, this, std::placeholders::_1));

	if (SlotType_ != ItemType::None && SlotItem_ != PrevItem_)
	{
		PrevItem_ = SlotItem_;

		if (nullptr != SlotItem_)
		{
			GameManager::GetInst().ChangeItem(SlotItem_->GetName(), SlotType_);
		}
		else
		{
			GameManager::GetInst().ChangeItem("default", SlotType_);
		}
	}

	if (true == GetItem_ && true == GameEngineInput::GetInst().Press("MouseLButton"))
	{
		MoveCollision_->GetTransform()->SetWorldPosition(MouseCollision_->GetTransform()->GetWorldPosition());
		ItemIconRenderer_->GetTransform()->SetWorldPosition(MouseCollision_->GetTransform()->GetWorldPosition());
		ItemIconRenderer_->GetTransform()->SetWorldPositionZ(-1000.f);

		if ("None" != SlotItem_->GetGrade())
		{
			GradeUIRenderer->GetTransform()->SetWorldPosition(MouseCollision_->GetTransform()->GetWorldPosition());
		}
	}

	if (true == GameEngineInput::GetInst().Up("MouseLButton"))
	{
		if (true == GetItem_)
		{
			GameManager::GetInst().GetMouse()->SetCatchItemType(ItemType::None);
			GetItem_ = false;
			MouseCollision_ = nullptr;

			if (SlotCollision_ == MainCollision_)
			{
				MoveCollision_->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());
				ItemIconRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));

				if ("None" != SlotItem_->GetGrade())
				{
					GradeUIRenderer->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());
				}
			}
			else
			{
				MoveCollision_->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());
				GetItem_ = false;

				InventorySlot* Slot = (InventorySlot*)SlotCollision_->GetActor();

				if (ItemType::None == Slot->GetSlotType() || SlotItem_->GetType() == Slot->GetSlotType())
				{
					ItemIconRenderer_->Off();

					if ("None" != SlotItem_->GetGrade())
					{
						GradeUIRenderer->Off();
					}

					if (true == Slot->GetEmpty())
					{
						Slot->SetItem(SlotItem_->GetName());
						Empty_ = true;
						SlotItem_ = nullptr;
					}
					else
					{
						std::string Name = Slot->GetSlotItem()->GetName();
						Slot->SetItem(SlotItem_->GetName());
						SetItem(Name);
					}
				}
				else
				{
					ItemIconRenderer_->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());

					if ("None" != SlotItem_->GetGrade())
					{
						GradeUIRenderer->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());
					}
				}
			}
		}
	}

	if (true == Block_)
	{
		CoolTime_ -= _Time;
		CoolTimeRenderer_->SetPercent(CoolTime_ / CoolTimeMax_);

		if (CoolTime_ <= 0.f)
		{
			CoolTimeRenderer_->Off();
			Block_ = false;
		}
	}
}

void InventorySlot::SetItem(const std::string& _Name)
{
	if ("None" == _Name)
	{
		if (nullptr != SlotItem_)
		{
			ItemIconRenderer_->Off();

			if ("None" != SlotItem_->GetGrade())
			{
				GradeUIRenderer->Off();
			}

			SlotItem_ = nullptr;
		}

		Empty_ = true;
		return;
	}

	if (false == Empty_)
	{
		ItemIconRenderer_->Off();

		if ("None" != SlotItem_->GetGrade())
		{
			GradeUIRenderer->Off();
		}

		SlotItem_ = nullptr;
	}

	Empty_ = false;

	SlotItem_ = GameManager::GetInst().FindItem(_Name);

	ItemIconRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	ItemIconRenderer_->SetImage(SlotItem_->GetName() + ".png");
	ItemIconRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));

	if ("None" != SlotItem_->GetGrade())
	{
		GradeUIRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		GradeUIRenderer->SetImage(SlotItem_->GetGrade() + ".png");
		GradeUIRenderer->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));
	}
}

void InventorySlot::GetItem(GameEngineCollision* _Other)
{
	SelectUIRenderer->On();

	if (true == Block_)
	{
		return;
	}

	if (InvenType_ == InvenType::Avatar)
	{
		//GameManager::GetInst().InvenUI();
	}

	if (InvenType_ == InvenType::Inventory)
	{
		if (GameManager::GetInst().GetMouse()->GetCatchItemType() == ItemType::Potion)
		{
			GameManager::GetInst().GetInven()->PotionSlot();
		}
		else if (GameManager::GetInst().GetMouse()->GetCatchItemType() != ItemType::None)
		{
			GameManager::GetInst().GetInven()->WeaponSlot();
		}
	}

	if (false == Empty_ && true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		GameManager::GetInst().GetMouse()->SetCatchItemType(SlotItem_->GetType());

		GetItem_ = true;
		MouseCollision_ = _Other;
	}

	if (true == GameEngineInput::GetInst().Down("MouseRButton"))
	{
		UseItem();
	}
}

void InventorySlot::UseItem()
{
	if (true == Empty_)
	{
		return;
	}

	if (true == Block_)
	{
		return;
	}

	if (SlotItem_->GetType() == ItemType::Potion)
	{
		GameManager::GetInst().GetPlayer()->HPPercentUp(0.3f);
		GameManager::GetInst().GetPlayer()->MPPercentUp(0.3f);
		CoolTime(2.f);
		return;
	}

	Inventory* Inven = GameManager::GetInst().GetInst().GetInven();
	AvatarInven* Avatar = GameManager::GetInst().GetInst().GetAvatarInven();

	if (ItemType::None == SlotType_)
	{
		if (InvenType::Inventory == InvenType_)
		{
			std::unordered_map<ItemType, InventorySlot*> map = Inven->GetEquipEnumList();

			InventorySlot* EquipSlot = map[SlotItem_->GetType()];

			ItemIconRenderer_->Off();

			if ("None" != SlotItem_->GetGrade())
			{
				GradeUIRenderer->Off();
			}

			if (true == EquipSlot->GetEmpty())
			{
				EquipSlot->SetItem(SlotItem_->GetName());
				Empty_ = true;
				SlotItem_ = nullptr;
			}
			else
			{
				std::string Name = EquipSlot->GetSlotItem()->GetName();
				EquipSlot->SetItem(SlotItem_->GetName());
				SetItem(Name);
			}
		}
		else if (InvenType::Avatar == InvenType_)
		{
			std::vector<InventorySlot*> EquipList;
			EquipList = Avatar->GetEquipList();

			std::vector<InventorySlot*>::iterator Iter = EquipList.begin();

			for (; Iter != EquipList.end(); Iter++)
			{
				if (SlotItem_->GetType() == (*Iter)->GetSlotType())
				{
					InventorySlot* EquipSlot = (*Iter);

					ItemIconRenderer_->Off();

					if ("None" != SlotItem_->GetGrade())
					{
						GradeUIRenderer->Off();
					}

					if (true == EquipSlot->GetEmpty())
					{
						EquipSlot->SetItem(SlotItem_->GetName());
						Empty_ = true;
						SlotItem_ = nullptr;
					}
					else
					{
						std::string Name = EquipSlot->GetSlotItem()->GetName();
						EquipSlot->SetItem(SlotItem_->GetName());
						SetItem(Name);
					}
				}
			}
		}
	}
	else
	{
		std::vector<InventorySlot*> List;

		if (InvenType::Inventory == InvenType_)
		{
			List = Inven->GetSlotList();
		}
		else if (InvenType::Avatar == InvenType_)
		{
			List = Avatar->GetSlotList();
		}

		std::vector<InventorySlot*>::iterator Iter = List.begin();

		for (; Iter != List.end(); Iter++)
		{
			if (true == (*Iter)->GetEmpty())
			{
				ItemIconRenderer_->Off();

				if ("None" != SlotItem_->GetGrade())
				{
					GradeUIRenderer->Off();
				}

				(*Iter)->SetItem(SlotItem_->GetName());
				Empty_ = true;
				SlotItem_ = nullptr;

				return;
			}
		}
	}
}

void InventorySlot::ChangeSlot(GameEngineCollision* _Other)
{
	SlotCollision_ = _Other;
}

void InventorySlot::SetTopUISlot()
{
	MainUIRenderer_->SetImage("UI1.png");
	InvenType_ = InvenType::None;
}

void InventorySlot::CoolTime(float _Time)
{
	Block_ = true;
	CoolTime_ = _Time;
	CoolTimeMax_ = _Time;
	CoolTimeRenderer_->On();
}
