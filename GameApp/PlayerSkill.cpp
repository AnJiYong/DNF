#include "Precompile.h"
#include "PlayerSkill.h"
#include "MonsterBase.h"
#include "Player.h"
#include "GameManager.h"
#include "MonsterBase.h"
#include "GameEngine/GameEngineImageRenderer.h"

PlayerSkill::PlayerSkill()
	: SkillEffect1Renderer_(nullptr)
	, SkillEffect2Renderer_(nullptr)
	, TargetMonster_(nullptr)
	, SkillOn_(false)
	, Pos_(float4::ZERO)
	, DeltaTIme_(0.f)
	, MindSwordTime_(0.f)
	, MindSwordCount_(0)
	, Info_(nullptr)
	, SkillCount_(0)
{
}

PlayerSkill::~PlayerSkill()
{
}

void PlayerSkill::Start()
{
	SkillEffect1Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	SkillEffect1Renderer_->SetRenderingPipeLine("TextureTrans");
	SkillEffect1Renderer_->CreateAnimationFolder("Dummy", "tripleslash", 0.2f, 28, 28, false);
	SkillEffect1Renderer_->SetChangeAnimation("Dummy");

	SkillEffect2Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	SkillEffect2Renderer_->SetRenderingPipeLine("TextureTrans");
	SkillEffect2Renderer_->CreateAnimationFolder("Dummy", "tripleslash", 0.2f, 28, 28, false);
	SkillEffect2Renderer_->SetChangeAnimation("Dummy");

	// ½É°Ë
	SkillStateManager_.CreateState("MindSword", std::bind(&PlayerSkill::MindSwordFinish, this));

	SkillEffect1Renderer_->CreateAnimationFolder("MindSwordFinish1", "MindSwordFinish1", 0.05f, 0, 9, false);
	SkillEffect1Renderer_->CreateAnimationFolder("MindSwordFinish2", "MindSwordFinish2", 0.05f, 0, 5, false);
	SkillEffect2Renderer_->CreateAnimationFolder("MindSwordFinish3", "MindSwordFinish3", 0.05f, 0, 3, false);
	SkillEffect1Renderer_->SetEndCallBack("MindSwordFinish1", [&]() { SkillEffect1Renderer_->SetChangeAnimation("MindSwordFinish2"); });
	SkillEffect1Renderer_->SetFrameCallBack("MindSwordFinish2", 3, [&]() { SkillEffect2Renderer_->SetChangeAnimation("MindSwordFinish3"); });
	SkillEffect2Renderer_->SetEndCallBack("MindSwordFinish3", [&]() { Death(); });
}

void PlayerSkill::Update(float _DeltaTime)
{
	DeltaTIme_ = _DeltaTime;
	SkillStateManager_.Update();
}

void PlayerSkill::MindSwordFinish()
{
	if (nullptr != TargetMonster_ && 0 < TargetMonster_->GetMonsterState().HP_)
	{
		Pos_ = TargetMonster_->GetTransform()->GetWorldPosition();

		MindSwordTime_ -= DeltaTIme_;

		if (MindSwordTime_ <= 0.f)
		{
			MindSwordTime_ = 0.05f;

			std::string Name = "MindSword";
			std::string Middle = std::to_string(SkillCount_);
			Name.append(Middle);

			int Damage = GameManager::GetInst().GetPlayer()->GetAttackInfo().GetDamage();

			++MindSwordCount_;

			AttackInfo_.SetAttackInfo(Name, Damage, MindSwordCount_);
			TargetMonster_->HitDamage(AttackInfo_, GameManager::GetInst().GetPlayer()->GetPlayerInfo());
		}
	}
	else
	{
		TargetMonster_ = nullptr;
	}

	GetTransform()->SetWorldPosition(Pos_ + float4(0.f, 90.f));

	if (false == SkillOn_)
	{
		SkillEffect1Renderer_->SetChangeAnimation("MindSwordFinish1");
		SkillOn_ = true;
	}
}

void PlayerSkill::SkillSetting(std::string _Name, int _Count, MonsterBase* _Monster, PlayerInfo* _Info)
{
	SkillStateManager_.ChangeState(_Name);
	SkillCount_ = _Count;
	TargetMonster_ = _Monster;
	Info_ = _Info;
}
