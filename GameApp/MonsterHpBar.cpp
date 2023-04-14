#include "Precompile.h"
#include "MonsterHpBar.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineProgressBarRenderer.h"
#include "DamageFont.h"
#include "MonsterBase.h"

MonsterHpBar::MonsterHpBar()
	: BaseRenderer_(nullptr)
	, Monster_(nullptr)
	, PrevMonster_(nullptr)
	, MonsterHP_(2325)
	, PrevHP_(0)
	, IsBoss_(false)
	, BossHPBarBackRenderer_(nullptr)
	, BossHPBarRenderer_(nullptr)
	, MonsterIconRenderer_(nullptr)
	, HPBarFrontRenderer_(nullptr)
	, HPBarBackRenderer_(nullptr)
	, HPBarRenderer_(nullptr)
	, HPBarCountRenderer_()
	, InvincibilityRenderer_(nullptr)
	, InvincibilityIconRenderer_(nullptr)
	, HitDamageOn_(false)
	, HitDamageUpTime_(0.05f)
	, HitDamageDownTime_(0.05f)
	, MonsterDeathRenderer_(nullptr)
	, MarkRenderer1_(nullptr)
	, MarkRenderer2_(nullptr)
	, HPBarOffTime_(1.5f)
{
}

MonsterHpBar::~MonsterHpBar()
{
}

void MonsterHpBar::Start()
{
	BaseRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	BaseRenderer_->SetImage("HPBar_Nomal.png");

	HPBarBackRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	HPBarBackRenderer_->SetImage("HPBar_Nomal00.png");

	HPBarRenderer_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
	HPBarRenderer_->SetImage("HPBar_Nomal01.png");
	HPBarRenderer_->SetProgressBarDirect(2);

	HPBarFrontRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	HPBarFrontRenderer_->SetImage("HPBar_Nomal05.png");
	HPBarFrontRenderer_->Off();

	for (size_t i = 0; i < 5; i++)
	{
		HPBarCountRenderer_[i] = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		HPBarCountRenderer_[i]->Off();
	}

	BossHPBarBackRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	BossHPBarBackRenderer_->SetImage("HPBar_BossDown.png");
	BossHPBarBackRenderer_->Off();

	BossHPBarRenderer_ = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform());
	BossHPBarRenderer_->SetImage("HPBar_BossDown00.png");
	BossHPBarRenderer_->SetProgressBarDirect(2);
	BossHPBarRenderer_->Off();

	MarkRenderer1_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MarkRenderer1_->SetImage("HPBar_BossDown01.png");
	MarkRenderer1_->Off();
	
	MarkRenderer2_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MarkRenderer2_->SetImage("HPBar_BossDown01.png");
	MarkRenderer2_->Off();

	MonsterIconRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MonsterIconRenderer_->SetImage("HPBar_Icon01.png");
	MonsterIconRenderer_->GetTransform()->SetLocalPosition(float4(-378.f, 227.f, static_cast<float>(HPBarZOrder::MonsterIcon)));
	MonsterIconRenderer_->SetOutLine(float4::BLACK);
	MonsterIconRenderer_->TextSetting("gulim", "Lv110 일반 몬스터", 11, float4::WHITE, float4(25.f, 15.f), FW1_LEFT);

	MonsterDeathRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MonsterDeathRenderer_->SetImage("HPBar_Icon00.png");
	MonsterDeathRenderer_->GetTransform()->SetLocalPosition(float4(-378.f, 227.f, static_cast<float>(HPBarZOrder::MonsterDeath)));
	MonsterDeathRenderer_->Off();

	InvincibilityRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	InvincibilityRenderer_->SetRenderingPipeLine("TextureTrans");
	InvincibilityRenderer_->CreateAnimationFolder("Invincibility", "Invincibility", 0.1f, 0, 12);
	InvincibilityRenderer_->SetChangeAnimation("Invincibility");
	InvincibilityRenderer_->Off();

	InvincibilityIconRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	InvincibilityIconRenderer_->SetImage("InvincibilityIcon.png");
	InvincibilityIconRenderer_->Off();
}

void MonsterHpBar::Update(float _Time)
{
	if (nullptr != Monster_)
	{
		MonsterHP_ = Monster_->State_.HP_;

		if (PrevHP_ != MonsterHP_)
		{
			PrevHP_ = MonsterHP_;
			HitDamageOn_ = true;

			float Percent = static_cast<float>(MonsterHP_ % 100);
			Percent *= 0.01f;

			HPBarRenderer_->SetPercent(Percent);

			if (MonsterHP_ <= 0)
			{
				HPBarCount(0);
			}
			else
			{
				HPBarCount(MonsterHP_ / 100);
			}

			if (true == IsBoss_)
			{
				int MonsterHPMax = Monster_->State_.HpMax_;

				float HPPercent = static_cast<float>(MonsterHP_) / static_cast<float>(MonsterHPMax);

				BossHPBarRenderer_->SetPercent(HPPercent);
			}
		}

		if (true == IsBoss_)
		{
			if (true == Monster_->GetInvincibility() && MonsterHP_ > 0)
			{
				InvincibilityRenderer_->On();
				InvincibilityIconRenderer_->On();

				InvincibilityRenderer_->GetTransform()->SetLocalPosition(float4(-90.f, 227.f, static_cast<float>(HPBarZOrder::Invincibility)));
				InvincibilityIconRenderer_->GetTransform()->SetLocalPosition(float4(-90.f, 227.f, static_cast<float>(HPBarZOrder::InvincibilityIcon)));
			}
			else
			{
				InvincibilityRenderer_->Off();
				InvincibilityIconRenderer_->Off();
			}
		}

		if (MonsterHP_ <= 0)
		{
			MonsterDeathRenderer_->On();
			HPBarOffTime_ -= _Time;

			if (HPBarOffTime_ <= 0.f)
			{
				Off();
			}
		}

		HitDamageShake(_Time);
	}
}

void MonsterHpBar::SetMonster(MonsterBase* _Monster)
{
	if (PrevMonster_ == _Monster)
	{
		return;
	}

	if (true == IsBoss_ && MonsterHP_ > 0)
	{
		return;
	}

	IsBoss_ = false;
	Monster_ = _Monster;
	PrevMonster_ = Monster_;
	HPBarOffTime_ = 1.5f;
	MonsterDeathRenderer_->Off();

	BaseRenderer_->SetImage("HPBar_Nomal.png");
	BaseRenderer_->GetTransform()->SetLocalPosition(float4(-77.5f, 225.f, static_cast<float>(HPBarZOrder::Base)));
	HPBarBackRenderer_->GetTransform()->SetLocalPosition(float4(-64.5f, 219.f, static_cast<float>(HPBarZOrder::HPBarBack)));
	HPBarRenderer_->GetTransform()->SetLocalPosition(float4(-64.5f, 219.f, static_cast<float>(HPBarZOrder::HPBar)));

	HPBarFrontRenderer_->SetImage("HPBar_Nomal05.png");
	HPBarFrontRenderer_->GetTransform()->SetLocalPosition(float4(-64.5f, 219.f, static_cast<float>(HPBarZOrder::HPBarFront)));

	for (size_t i = 0; i < 5; i++)
	{
		HPBarCountRenderer_[i]->GetTransform()->SetLocalPosition(float4(225.f + i * -15.f, 240.f, static_cast<float>(HPBarZOrder::HPBarCount)));
		HPBarCountRenderer_[i]->Off();
	}

	BossHPBarBackRenderer_->Off();
	BossHPBarRenderer_->Off();
	MarkRenderer1_->Off();
	MarkRenderer2_->Off();

	MonsterIconRenderer_->SetImage("HPBar_Icon01.png");
	MonsterIconRenderer_->TextSetting("gulim", "Lv110 " + Monster_->GetMonsterName(), 11, float4::WHITE, float4(25.f, 15.f), FW1_LEFT);
}

void MonsterHpBar::SetIsBoss(MonsterBase* _Monster)
{
	if (PrevMonster_ == _Monster)
	{
		return;
	}

	IsBoss_ = true;
	Monster_ = _Monster;
	PrevMonster_ = Monster_;
	HPBarOffTime_ = 1.5f;
	MonsterDeathRenderer_->Off();

	BaseRenderer_->SetImage("HPBar_Boss.png");
	BaseRenderer_->GetTransform()->SetLocalPosition(float4(-107.5f + 2.5f, 225.f, static_cast<float>(HPBarZOrder::Base)));
	HPBarBackRenderer_->GetTransform()->SetLocalPosition(float4(-90.f, 227.f, static_cast<float>(HPBarZOrder::HPBarBack)));
	HPBarRenderer_->GetTransform()->SetLocalPosition(float4(-90.f, 227.f, static_cast<float>(HPBarZOrder::HPBar)));

	HPBarFrontRenderer_->SetImage("HPBar_Boss05.png");
	HPBarFrontRenderer_->GetTransform()->SetLocalPosition(float4(-90.f, 227.f, static_cast<float>(HPBarZOrder::HPBarFront)));

	for (size_t i = 0; i < 5; i++)
	{
		HPBarCountRenderer_[i]->GetTransform()->SetLocalPosition(float4(160.f + i * -15.f, 228.f, static_cast<float>(HPBarZOrder::HPBarCount)));
		HPBarCountRenderer_[i]->Off();
	}

	BossHPBarBackRenderer_->On();
	BossHPBarRenderer_->On();

	BossHPBarBackRenderer_->GetTransform()->SetLocalPosition(float4(-89.f, 211.f, static_cast<float>(HPBarZOrder::BossHPBarBack)));
	BossHPBarRenderer_->GetTransform()->SetLocalPosition(float4(-89.5f, 211.f, static_cast<float>(HPBarZOrder::BossHPBar)));

	MonsterIconRenderer_->SetImage("HPBar_Icon01.png");
	MonsterIconRenderer_->TextSetting("gulim", "Lv110 " + Monster_->GetMonsterName(), 11, float4::WHITE, float4(25.f, 8.f), FW1_LEFT);

	MarkRenderer1_->On();
	MarkRenderer2_->On();

	MarkRenderer1_->GetTransform()->SetLocalPosition(float4(-89.5f + 103.f, 216.f, static_cast<float>(HPBarZOrder::BossPatternMark)));
	MarkRenderer2_->GetTransform()->SetLocalPosition(float4(-89.5f - 59.f, 216.f, static_cast<float>(HPBarZOrder::BossPatternMark)));
}

void MonsterHpBar::HPBarCount(int _Value)
{
	switch (_Value % 10)
	{
	case 0:
	case 5:
		if (true == IsBoss_)
		{
			HPBarBackRenderer_->SetImage("HPBar_Boss04.png");
			HPBarRenderer_->SetImage("HPBar_Boss00.png");
		}
		else
		{
			HPBarBackRenderer_->SetImage("HPBar_Nomal04.png");
			HPBarRenderer_->SetImage("HPBar_Nomal00.png");
		}

		HPBarBackRenderer_->On();

		if (_Value == 0)
		{
			HPBarBackRenderer_->Off();
		}
		break;
	case 1:
	case 6:
		if (true == IsBoss_)
		{
			HPBarBackRenderer_->SetImage("HPBar_Boss00.png");
			HPBarRenderer_->SetImage("HPBar_Boss01.png");
		}
		else
		{
			HPBarBackRenderer_->SetImage("HPBar_Nomal00.png");
			HPBarRenderer_->SetImage("HPBar_Nomal01.png");
		}

		HPBarBackRenderer_->On();
		break;
	case 2:
	case 7:
		if (true == IsBoss_)
		{
			HPBarBackRenderer_->SetImage("HPBar_Boss01.png");
			HPBarRenderer_->SetImage("HPBar_Boss02.png");
		}
		else
		{
			HPBarBackRenderer_->SetImage("HPBar_Nomal01.png");
			HPBarRenderer_->SetImage("HPBar_Nomal02.png");
		}

		HPBarBackRenderer_->On();
		break;
	case 3:
	case 8:
		if (true == IsBoss_)
		{
			HPBarBackRenderer_->SetImage("HPBar_Boss02.png");
			HPBarRenderer_->SetImage("HPBar_Boss03.png");
		}
		else
		{
			HPBarBackRenderer_->SetImage("HPBar_Nomal02.png");
			HPBarRenderer_->SetImage("HPBar_Nomal03.png");
		}

		HPBarBackRenderer_->On();
		break;
	case 4:
	case 9:
		if (true == IsBoss_)
		{
			HPBarBackRenderer_->SetImage("HPBar_Boss03.png");
			HPBarRenderer_->SetImage("HPBar_Boss04.png");
		}
		else
		{
			HPBarBackRenderer_->SetImage("HPBar_Nomal03.png");
			HPBarRenderer_->SetImage("HPBar_Nomal04.png");
		}

		HPBarBackRenderer_->On();
		break;
	}

	std::vector<int> HPBarCount_;

	if (_Value == 0)
	{
		HPBarCount_.push_back(0);
	}

	while (_Value > 0)
	{
		HPBarCount_.push_back(_Value % 10);
		_Value /= 10;
	}

	HPBarCount_.push_back(99);

	for (size_t i = 0; i < 5; i++)
	{
		HPBarCountRenderer_[i]->Off();
	}

	for (size_t i = 0; i < HPBarCount_.size(); i++)
	{
		HPBarCountRenderer_[i]->On();
		std::string Name = "MonsterHp";
		std::string Middle = std::to_string(HPBarCount_[i]);
		Name.append(Middle);
		Name.append(".png");
		HPBarCountRenderer_[i]->SetImage(Name);
	}
}

void MonsterHpBar::HitDamageShake(float _Time)
{
	if (false == HitDamageOn_)
	{
		return;
	}

	HitDamageUpTime_ -= _Time;
	HPBarFrontRenderer_->On();

	if (HitDamageUpTime_ >= 0.f)
	{
		GetTransform()->SetWorldDeltaTimeMove(float4(-100.f, 100.f, 0.f));
	}
	else
	{
		HitDamageDownTime_ -= _Time;

		if (HitDamageDownTime_ >= 0.f)
		{
			GetTransform()->SetWorldDeltaTimeMove(float4(100.f, -100.f, 0.f));
		}
		else
		{
			HPBarFrontRenderer_->Off();
			HitDamageUpTime_ = 0.05f;
			HitDamageDownTime_ = 0.05f;
			HitDamageOn_ = false;
		}
	}
}