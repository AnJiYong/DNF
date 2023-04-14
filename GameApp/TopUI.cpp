#include "PreCompile.h"
#include "TopUI.h"
#include "SkillSlot.h"
#include "GameManager.h"
#include "Player.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngine/GameEngineProgressBarRenderer.h"
#include "InventorySlot.h"
#include "MonsterHpBar.h"
#include "MiniMap.h"

TopUI::TopUI()
	: TopRenderer(nullptr)
	, ItemRenderer(nullptr)
	, Time(1.f)
	, Test(true)
	, InvenCollision_(nullptr)
	, HPBar_(nullptr)
	, MPBar_(nullptr)
	, UIOn(true)
	, MiniMap_(nullptr)
	, PrevHP_(0.f)
	, PrevMP_(0.f)
	, HPTime_(0.f)
	, MPTime_(0.f)
	, HPBarBack_(nullptr)
	, MPBarBack_(nullptr)
	, HPBarPercent_(nullptr)
	, MPBarPercent_(nullptr)
	, MonsterHpBar_(nullptr)
{
}

TopUI::~TopUI()
{
}

void TopUI::Start()
{
	//TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//TopRenderer->GetTransform()->SetLocalScaling({ 800.f, 100.f, 1.f });
	//TopRenderer->GetTransform()->SetLocalPosition({ 0.f, 300.f, 0.f });
	//TopRenderer->TextSetting("궁서", "테스트 중입니다", 50);

	TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TopRenderer->SetImage("UI0.png");
	TopRenderer->GetTransform()->SetWorldPosition(float4(0.f, -225.f - 37.5f));
	//TopRenderer->SetRenderingPipeLine("TextureUI");

	InvenCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));

	MonsterHpBar_ = GetLevel()->CreateActor<MonsterHpBar>();
	MonsterHpBar_->GetTransform()->AttachTransform(GetTransform());
	MonsterHpBar_->Off();

	SkillSlotSetting();
	LeftSlotSetting();
	Hud();
}

void TopUI::Update(float _Time)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(InvenCollision_->GetTransform(), CollisionType::Rect);
#endif

	InvenCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&TopUI::InvenButton, this, std::placeholders::_1));

	float HP = GameManager::GetInst().GetPlayer()->GetHPPercent();
	float MP = GameManager::GetInst().GetPlayer()->GetMPPercent();

	if (PrevHP_ != HP)
	{
		HPBar_->SetPercent(HP);

		HP *= 100.f;

		std::string HPPercent = std::to_string(static_cast<int>(HP));
		HPPercent.append("%");

		HPBarPercent_->TextSetting("gulim", HPPercent, 11, float4(201.f / 255.f, 113.f / 255.f, 73.f / 255.f), float4(0.f, 6.f));

		PrevHP_ = HP;
		HPTime_ = 0.15f;
	}

	HPTime_ -= _Time;

	if (HPTime_ <= 0.f)
	{
		//HPBarBack_->SetPercent(HP);
	}

	if (PrevMP_ != MP)
	{
		MPBar_->SetPercent(MP);

		MP *= 100.f;

		std::string MPPercent = std::to_string(static_cast<int>(MP));
		MPPercent.append("%");

		MPBarPercent_->TextSetting("gulim", MPPercent, 11, float4(60.f / 255.f, 155.f / 255.f, 199.f / 255.f), float4(0.f, 6.f));

		PrevMP_ = MP;
		MPTime_ = 0.15f;
	}

	MPTime_ -= _Time;

	if (MPTime_ <= 0.f)
	{
		//MPBarBack_->SetPercent(MP);
	}

	if (true == GameEngineInput::GetInst().Down("1"))
	{
		LeftSlotList_[0]->UseItem();
	}

	if (true == GameEngineInput::GetInst().Down("2"))
	{
		LeftSlotList_[1]->UseItem();
	}
	if (true == GameEngineInput::GetInst().Down("3"))
	{
		LeftSlotList_[2]->UseItem();
	}
	if (true == GameEngineInput::GetInst().Down("4"))
	{
		LeftSlotList_[3]->UseItem();
	}
	if (true == GameEngineInput::GetInst().Down("5"))
	{
		LeftSlotList_[4]->UseItem();
	}
	if (true == GameEngineInput::GetInst().Down("6"))
	{
		LeftSlotList_[5]->UseItem();
	}
}

void TopUI::SkillSlotSetting()
{
	SkillSlot* SkillSlot0 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot0->GetTransform()->SetLocalPosition(float4(-102.f + 15.f, -232.f - 15.f, -1.f));
	SkillSlot0->SetKey("q.png", "tripleslash");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("q", SkillSlot0));

	SkillSlot* SkillSlot1 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot1->GetTransform()->SetLocalPosition(float4(-102.f + 15.f, -263.f - 15.f, -1.f));
	SkillSlot1->SetKey("a.png", "flowmindone");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("a", SkillSlot1));

	SkillSlot* SkillSlot2 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot2->GetTransform()->SetLocalPosition(float4(-71.f + 15.f, -232.f - 15.f, -1.f));
	SkillSlot2->SetKey("w.png", "chargecrash");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("w", SkillSlot2));

	SkillSlot* SkillSlot3 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot3->GetTransform()->SetLocalPosition(float4(-71.f + 15.f, -263.f - 15.f, -1.f));
	SkillSlot3->SetKey("s.png", "flowmindtwo");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("s", SkillSlot3));

	SkillSlot* SkillSlot4 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot4->GetTransform()->SetLocalPosition(float4(-40.f + 15.f, -232.f - 15.f, -1.f));
	SkillSlot4->SetKey("e.png", "momentaryslash");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("e", SkillSlot4));

	SkillSlot* SkillSlot5 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot5->GetTransform()->SetLocalPosition(float4(-40.f + 15.f, -263.f - 15.f, -1.f));
	SkillSlot5->SetKey("d.png", "flowmindthree");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("d", SkillSlot5));

	SkillSlot* SkillSlot6 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot6->GetTransform()->SetLocalPosition(float4(-9.f + 15.f, -232.f - 15.f, -1.f));
	SkillSlot6->SetKey("r.png", "guard");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("r", SkillSlot6));

	//SkillSlot* SkillSlot7 = GetLevel()->CreateActor<SkillSlot>();
	//SkillSlot7->GetTransform()->SetLocalPosition(float4(-9.f + 15.f, -263.f - 15.f, -1.f));
	//SkillSlot7->SetKey("f.png", "ChargeBurst");
	//SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("f", SkillSlot7));

	SkillSlot* SkillSlot8 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot8->GetTransform()->SetLocalPosition(float4(22.f + 15.f, -232.f - 15.f, -1.f));
	SkillSlot8->SetKey("t.png", "Gorecross");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("t", SkillSlot8));

	//SkillSlot* SkillSlot9 = GetLevel()->CreateActor<SkillSlot>();
	//SkillSlot9->GetTransform()->SetLocalPosition(float4(22.f + 15.f, -263.f - 15.f, -1.f));
	//SkillSlot9->SetKey("g.png", "EtherealSlash");
	//SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("g", SkillSlot9));

	SkillSlot* SkillSlot10 = GetLevel()->CreateActor<SkillSlot>();
	SkillSlot10->GetTransform()->SetLocalPosition(float4(53.f + 15.f, -232.f - 15.f, -1.f));
	SkillSlot10->SetKey("y.png", "MindSword");
	SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("y", SkillSlot10));

	//SkillSlot* SkillSlot11 = GetLevel()->CreateActor<SkillSlot>();
	//SkillSlot11->GetTransform()->SetLocalPosition(float4(53.f + 15.f, -263.f - 15.f, -1.f));
	//SkillSlot11->SetKey("h.png", "Lightning");
	//SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("h", SkillSlot11));

	//SkillSlot* SkillSlot12 = GetLevel()->CreateActor<SkillSlot>();
	//SkillSlot12->GetTransform()->SetLocalPosition(float4(84.f + 15.f, -232.f - 15.f, -1.f));
	//SkillSlot12->SetKey("shift.png", "TelekineticSwords");
	//SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("shift", SkillSlot12));

	//SkillSlot* SkillSlot13 = GetLevel()->CreateActor<SkillSlot>();
	//SkillSlot13->GetTransform()->SetLocalPosition(float4(84.f + 15.f, -263.f - 15.f, -1.f));
	//SkillSlot13->SetKey("ctrl.png", "Pentastrike");
	//SkillSlot_.insert(std::map<std::string, SkillSlot*>::value_type("ctrl", SkillSlot13));
}

void TopUI::LeftSlotSetting()
{
	for (size_t i = 0; i < 6; i++)
	{
		InventorySlot* LeftSlot_ = GetLevel()->CreateActor<InventorySlot>();
		LeftSlot_->GetTransform()->AttachTransform(GetTransform());
		LeftSlot_->GetTransform()->SetWorldPosition(float4(-383.f + (i * 31.f), -283.f, -1.f));
		LeftSlot_->SetTopUISlot();
		LeftSlotList_.push_back(LeftSlot_);

		GameEngineUIRenderer* LeftSlotKey = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		std::string Name = "Key";
		std::string Middle = std::to_string(i + 1);
		Name.append(Middle);
		Name.append(".png");
		LeftSlotKey->SetImage(Name);
		LeftSlotKey->GetTransform()->SetWorldPosition(float4(-383.f + (i * 31.f), -283.f, -10.f));
	}
}

void TopUI::Hud()
{
	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_skilllistX.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(-117.f + 8.5f, -231.f - 17.f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_skillstyle.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(-117.f + 8.5f, -262.f - 17.f, -1.f));
	}

	{
		HPBar_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
		HPBar_->CreateAnimationFolder("hp", "hp", 0.02f, 0, 120);
		HPBar_->SetChangeAnimation("hp");
		HPBar_->GetTransform()->SetWorldPosition(float4(-173.f + 27.f, -239.f - 27.f, -1.f));
		HPBar_->SetProgressBarDirect(1);

		//HPBarBack_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
		//HPBarBack_->CreateAnimationFolder("hp", "hp", 0.02f, 0, 120);
		//HPBarBack_->SetChangeAnimation("hp");
		//HPBarBack_->GetTransform()->SetWorldPosition(float4(-173.f + 27.f, -239.f - 27.f, -1.f));
		//HPBarBack_->SetPlusColor(float4::WHITE);
		//HPBarBack_->SetProgressBarDirect(1);

		HPBarPercent_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		HPBarPercent_->SetImage("Dummy.png");
		HPBarPercent_->GetTransform()->SetWorldPosition(float4(-173.f + 27.f, -239.f - 27.f, -1.f));
		HPBarPercent_->SetOutLine(float4::BLACK);
		HPBarPercent_->TextSetting("gulim", "100%", 11, float4(201.f / 255.f, 113.f / 255.f, 73.f / 255.f), float4(0.f, 6.f));
	}

	{
		MPBar_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
		MPBar_->CreateAnimationFolder("mp", "mp", 0.02f, 0, 120);
		MPBar_->SetChangeAnimation("mp");
		MPBar_->GetTransform()->SetWorldPosition(float4(173.f - 27.f, -239.f - 27.f, -1.f));
		MPBar_->SetProgressBarDirect(1);

		//MPBarBack_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
		//MPBarBack_->CreateAnimationFolder("mp", "mp", 0.02f, 0, 120);
		//MPBarBack_->SetChangeAnimation("mp");
		//MPBarBack_->GetTransform()->SetWorldPosition(float4(173.f - 27.f, -239.f - 27.f, -1.f));
		//MPBarBack_->SetPlusColor(float4::WHITE);
		//MPBarBack_->SetProgressBarDirect(1);

		MPBarPercent_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		MPBarPercent_->SetImage("Dummy.png");
		MPBarPercent_->GetTransform()->SetWorldPosition(float4(173.f - 27.f, -239.f - 27.f, -1.f));
		MPBarPercent_->SetOutLine(float4::BLACK);
		MPBarPercent_->TextSetting("gulim", "100%", 11, float4(60.f / 255.f, 155.f / 255.f, 199.f / 255.f), float4(0.f, 6.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_level.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(0.f - 5.5f, -300.f + 8.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_levelbar.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(0.f, -295.f - 2.f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_my.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(311.f + 10.f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_inven.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(328.f + 10.f, -253.f - 13.5f, -1.f));

		InvenCollision_->GetTransform()->SetLocalScaling(float4(18.f, 25.f, 1.f));
		InvenCollision_->GetTransform()->SetWorldPosition(float4(328.f + 10.f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_x2.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(345.f + 10.f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_x1.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(362.f + 10.f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_x3.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(379.f + 10.f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_cashshop.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(222.f + 32.f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_list.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(285.f + 13.5f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_alarm.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(196.f + 13.5f, -253.f - 13.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_alarmicon.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(209.5f, -265.f, -2.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_fatigue.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(230.f + 11.5f, -278.f - 5.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_fatiguebar.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(256.f + 70.5f, -282.f - 2.5f, -1.f));
	}

	{
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_pp.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(215.f + 91.5f, -288.f - 6.f, -1.f));
	}
}

void TopUI::MyButton(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Up("MouseLButton"))
	{
		GameManager::GetInst().AvatarUI();
	}
}

void TopUI::InvenButton(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Up("MouseLButton"))
	{
		GameManager::GetInst().InvenUI();
	}
}

void TopUI::TownSet(bool _IsTown)
{
	if (true == _IsTown)
	{		
		GameEngineUIRenderer* UIRneder = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		UIRneder->SetImage("UI_map.png");
		UIRneder->GetTransform()->SetWorldPosition(float4(400.f - 17.5f, 300.f - 17.5f, -1.f));
	}
	else
	{
		MiniMap_ = GetLevel()->CreateActor<MiniMap>();
		MiniMap_->GetTransform()->SetWorldPosition(float4(320.f, 209.f));
	}
}

void TopUI::SetLeftSlot()
{
	std::vector<std::string> Slot = GameManager::GetInst().GetTopUISlotInfo();

	for (size_t i = 0; i < LeftSlotList_.size(); i++)
	{
		LeftSlotList_[i]->SetItem(Slot[i]);
	}
}

void TopUI::MonsterHPBarOn(MonsterBase* _Monster, bool _Boss)
{
	if (false == UIOn)
	{
		return;
	}

	MonsterHpBar_->On();

	if (true == _Boss)
	{
		MonsterHpBar_->SetIsBoss(_Monster);
	}
	else
	{
		MonsterHpBar_->SetMonster(_Monster);
	}
}

void TopUI::MonsterHPBarOff()
{
	MonsterHpBar_->Off();
}

void TopUI::UIOnOff(bool _On)
{
	UIOn = _On;

	if (false == UIOn)
	{
		MiniMap_->Off();
		MonsterHpBar_->Off();
	}
	else
	{
		MiniMap_->On();
	}
}
