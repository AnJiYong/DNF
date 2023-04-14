#include "Precompile.h"
#include "SkillSlot.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngine/GameEngineProgressBarRenderer.h"
#include "GlobalEnum.h"
#include "GameManager.h"
#include "Item.h"

SkillSlot::SkillSlot()
	: MainUIRenderer_(nullptr)
	, IconRenderer_(nullptr)
	, KeyRenderer_(nullptr)
	, SelectUIRenderer_(nullptr)
	, MainCollision_(nullptr)
	, MoveCollision_(nullptr)
	, MouseCollision_(nullptr)
	, SlotCollision_(nullptr)
	, CoolTimeRenderer_(nullptr)
	, TextRenderer_(nullptr)
	, Empty_(true)
	, GetSkill_(false)
	, SkillName_("")
	, Block_(false)
	, CoolTime_(0.f)
	, CoolTimeMax_(0.f)
{
}

SkillSlot::~SkillSlot()
{
}

void SkillSlot::Start()
{
	MainUIRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MainUIRenderer_->SetImage("SkillIcon.png");
	MainUIRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));

	IconRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	IconRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));

	KeyRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	KeyRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -5.f));

	MainCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Slot));
	MainCollision_->GetTransform()->SetLocalScaling(float4(28.f, 28.f, 1.f));
	MainCollision_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));

	MoveCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	MoveCollision_->GetTransform()->SetLocalScaling(float4(1.f, 1.f, 1.f));
	MoveCollision_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));

	SelectUIRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	SelectUIRenderer_->SetImage("Inven14.png");
	SelectUIRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));
	SelectUIRenderer_->Off();

	CoolTimeRenderer_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
	CoolTimeRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));
	CoolTimeRenderer_->SetImage("SkillIcon.png");
	CoolTimeRenderer_->SetProgressBarDirect(0);
	CoolTimeRenderer_->Off();

	TextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TextRenderer_->SetImage("Dummy.png");
	TextRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -10.f));
	TextRenderer_->Off();
}

void SkillSlot::SetKey(const std::string& _Key, const std::string& _Skill)
{
	KeyRenderer_->SetImage(_Key);
	IconRenderer_->SetImage(_Skill + ".png");
	SkillName_ = _Skill;
	Empty_ = false;
}

void SkillSlot::Update(float _Time)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(MainCollision_->GetTransform(), CollisionType::Rect);
#endif

	SlotCollision_ = MainCollision_;
	SelectUIRenderer_->Off();

	MainCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&SkillSlot::GetSkill, this, std::placeholders::_1));

	MoveCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Slot)
		, std::bind(&SkillSlot::ChangeSlot, this, std::placeholders::_1));

	if (false == Empty_ && true == GetSkill_ &&
		true == GameEngineInput::GetInst().Press("MouseLButton"))
	{
		MoveCollision_->GetTransform()->SetWorldPosition(MouseCollision_->GetTransform()->GetWorldPosition());
		IconRenderer_->GetTransform()->SetWorldPosition(MouseCollision_->GetTransform()->GetWorldPosition());
		IconRenderer_->GetTransform()->SetWorldPositionZ(-10.f);
	}

	if (true == GameEngineInput::GetInst().Up("MouseLButton"))
	{
		if (false == Empty_ && true == GetSkill_)
		{
			GetSkill_ = false;
			MouseCollision_ = nullptr;

			if (SlotCollision_ == MainCollision_)
			{
				MoveCollision_->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());
				IconRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));
			}
			else
			{
				MoveCollision_->GetTransform()->SetWorldPosition(MainCollision_->GetTransform()->GetWorldPosition());
				GetSkill_ = false;

				SkillSlot* Slot = (SkillSlot*)SlotCollision_->GetActor();
								
				IconRenderer_->Off();

				if (true == Slot->GetEmpty())
				{
					Slot->SetSkill(SkillName_);
					Empty_ = true;
					SkillName_ = "";
				}
				else
				{
					std::string Name = Slot->GetSkillName();
					Slot->SetSkill(SkillName_);
					SetSkill(Name);
				}
			}
		}
	}

	if (true == Block_)
	{
		CoolTime_ -= _Time;
		CoolTimeRenderer_->SetPercent(CoolTime_ / CoolTimeMax_);

		if (CoolTime_ >= 1.f)
		{
			std::string Time = std::to_string(static_cast<int>(CoolTime_));
			TextRenderer_->TextSetting("gulim", Time, 12, float4::WHITE, float4(-1.f, 7.f));
		}
		else
		{
			float RoundTime = round(CoolTime_ * 10.f);

			if (RoundTime == 10.f)
			{
				return;
			}

			std::string Time = std::to_string(static_cast<int>(RoundTime));
			TextRenderer_->TextSetting("gulim", "0." + Time, 12, float4::WHITE, float4(-1.f, 7.f));
		}
		if (CoolTime_ <= 0.f)
		{
			IconRenderer_->SetImage(SkillName_ + ".png");
			TextRenderer_->Off();
			CoolTimeRenderer_->Off();
			Block_ = false;
		}
	}
}

void SkillSlot::SetSkill(const std::string& _Name)
{
	if (false == Empty_)
	{
		IconRenderer_->Off();
		SkillName_ = "";
	}

	Empty_ = false;

	SkillName_ = _Name;

	IconRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	IconRenderer_->SetImage(SkillName_ + ".png");
	IconRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));
}

void SkillSlot::GetSkill(GameEngineCollision* _Other)
{
	SelectUIRenderer_->On();

	if (true == Block_)
	{
		return;
	}

	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		GetSkill_ = true;
		MouseCollision_ = _Other;
	}
}

void SkillSlot::ChangeSlot(GameEngineCollision* _Other)
{
	SlotCollision_ = _Other;
}

void SkillSlot::CoolTime(float _Time)
{
	Block_ = true;
	CoolTime_ = _Time;
	CoolTimeMax_ = _Time;
	IconRenderer_->SetImage(SkillName_ + "1.png");
	IconRenderer_->SetOutLine(float4::BLACK);
	CoolTimeRenderer_->On();
	TextRenderer_->On();
}
