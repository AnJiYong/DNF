#include "Precompile.h"
#include "Player.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "Weapon.h"
#include "Map.h"
#include "GlobalEnum.h"
#include "GameManager.h"
#include "Inventory.h"
#include "Avatar.h"
#include "TopUI.h"
#include "SkillSlot.h"
#include "MonsterBase.h"
#include "SkillInfo.h"
#include "DamageFont.h"
#include "AvatarUI.h"
#include "AvatarInven.h"
#include "PlayerSkill.h"
#include "DungeonLevel.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"
#include "GameEngine/GameEngineUIRenderer.h"

Player::Player()
	: PlayerImageRenderer(nullptr)
	, SkillEffect1Renderer_(nullptr)
	, SkillEffect2Renderer_(nullptr)
	, SkillEffect3Renderer_(nullptr)
	, SkillEffect4Renderer_(nullptr)
	, SkillEffect5Renderer_(nullptr)
	, BuffEffectRenderer_(nullptr)
	, TextRenderer_(nullptr)
	, BodyCollision_(nullptr)
	, AttackCollision_(nullptr)
	, PrevPos_(float4::ZERO)
	, PrevKey_(PrevKey::None)
	, PrevKeyTime_(0.f)
	, DeltaTime_(0.f)
	, Speed(250.f)
	, JumpPower_(200.f)
	, ZPos_(0.f)
	, Acceleration_(0.5f)
	, DebuffTime_(0.f)
	, DebuffValue_(1.f)
	, DebuffOn_(false)
	, KeyLock_(false)
	, DirLeft_(true)
	, MoveLeft_(true)
	, MoveRight_(true)
	, MoveUp_(true)
	, MoveDown_(true)
	, DashLeft_(false)
	, DashRight_(false)
	, JumpUp_(true)
	, CollisionMonsterCheck_(false)
	, IsDungeon_(false)
	, NextAttack_(false)
	, SuperArmor_(false)
	, HitAnimationChange_(false)
	, Dirf_(0.f)
	, Damage_(100)
	, ChargecrashTime_(0.1f)
	, IsSkill_(true)
	, CurAvatar_(nullptr)
	, HitTime_(0.4f)
	, Sound_(nullptr)
	, Pivot_(float4::ZERO)
	, TripleslashCount_(0)
	, TripleslashNext_(false)
	, TripleslashTime_(0.5f)
	, MomentaryslashNext_(false)
	, GuardOn_(false)
	, GuardTime_(0.f)
	, MindSwordChargeTime_(3.f)
	, MindSwordNext_(false)
	, GorecrossNext_(false)
	, AttackInfo_()
	, AttackCount_(0)
	, SkillKey_("")
	, Info_()
{

}

Player::~Player()
{
}

bool Player::GetPlayerDirection()
{
	if (BodyCollision_->GetTransform()->GetWorldPosition().x <= 0.f)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Player::Start()
{	
	PlayerAnimation();
	Collision();
	FSM();

	//TextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//TextRenderer_->SetImage("Dummy.png");
	//TextRenderer_->TextSetting("gulim", "안지용", 11, float4::WHITE);
	//TextRenderer_->SetOutLine(float4::BLACK);

	CurAvatar_ = GetLevel()->CreateActor<Avatar>(-2);
	CurAvatar_->GetTransform()->AttachTransform(GetTransform());

	SkillAnimation();

	BuffEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	BuffEffectRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 50.f));
	BuffEffectRenderer_->CreateAnimationFolder("Debuff", "Debuff", 0.1f, 0, 5);
	BuffEffectRenderer_->AnimaOff();

	AttackInfo_.SetAttackInfo("aaa", 25, AttackCount_);

	Info_.HPMax = 100;
	Info_.HP = 100;
	Info_.MPMax = 100000;
	Info_.MP = Info_.MPMax;
	Info_.Critical = 50.f;

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void Player::Update(float _DeltaTime)
{
	if (true == IsDungeon_)
	{
		DungeonStateManager_.Update();
		CommandCheck(_DeltaTime);
	}
	else
	{
		TownStateManager_.Update();
	}

#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushDebugRender(AttackCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	GameManager::GetInst().SetPlayerZPos(ZPos_);
	DeltaTime_ = _DeltaTime;

	GetTransform()->SetWorldPositionZ(BodyCollision_->GetTransform()->GetWorldPosition().y);

	AttackCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Monster)
		, std::bind(&Player::Damage, this, std::placeholders::_1));

	CurAvatar_->SetIndex(PlayerImageRenderer->GetCurrentAnimation()->CurFrame_);

	SkillCheck();
	DirCheck();
	DebuffCheck(_DeltaTime);
}

void Player::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	if (std::string::npos != _NextLevel->GetName().find("Title"))
	{
		return;
	}

	GetLevel()->SetLevelActorMove(_NextLevel, this);
	_NextLevel->GetMainCameraActor()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4(0.f, 0.f, -100.f));
}

void Player::PlayerAnimation()
{
	PlayerImageRenderer = CreateTransformComponent<GameEngineImageRenderer>();
	PlayerImageRenderer->GetTransform()->SetLocalPosition(float4(0.f, 75.f));
	PlayerImageRenderer->CreateAnimationFolder("TownIdle", "Character", 0.2f, 176, 179);
	PlayerImageRenderer->CreateAnimationFolder("Idle", "Character", 0.2f, 90, 95);
	PlayerImageRenderer->CreateAnimationFolder("Move", "Character", 0.15f, 180, 187);
	PlayerImageRenderer->CreateAnimationFolder("Attack1", "Character", 0.05f, 0, 9, false);
	PlayerImageRenderer->CreateAnimationFolder("Attack2", "Character", 0.05f, 10, 20, false);
	PlayerImageRenderer->CreateAnimationFolder("Attack3", "Character", 0.05f, 33, 41, false);
	PlayerImageRenderer->CreateAnimationFolder("Dash", "Character", 0.1f, 105, 112);
	PlayerImageRenderer->CreateAnimationFolder("DashAttack", "Character", 0.05f, 65, 74, false);
	PlayerImageRenderer->CreateAnimationFolder("JumpUp", "Character", 0.1f, 125, 129, false);
	PlayerImageRenderer->CreateAnimationFolder("JumpDown", "Character", 0.3f, 130, 131, false);
	PlayerImageRenderer->CreateAnimationFolder("JumpAttack", "Character", 0.075f, 133, 138, false);
	PlayerImageRenderer->CreateAnimationFolder("Hit1", "Character", 0.1f, 99, 99, false);
	PlayerImageRenderer->CreateAnimationFolder("Hit2", "Character", 0.1f, 98, 98, false);
	PlayerImageRenderer->CreateAnimationFolder("HitDown", "Character", 0.1f, 100, 103, false);

	PlayerImageRenderer->SetChangeAnimation("Idle");

	PlayerImageRenderer->SetFrameCallBack("Attack1", 9, std::bind(&Player::AttackFrame, this));
	PlayerImageRenderer->SetFrameCallBack("Attack2", 20, std::bind(&Player::AttackFrame, this));
	PlayerImageRenderer->SetFrameCallBack("Attack3", 41, std::bind(&Player::AttackFrame, this));
	PlayerImageRenderer->SetFrameCallBack("JumpAttack", 135, std::bind(&Player::JumpAttackFrame, this));

	PlayerImageRenderer->SetEndCallBack("Attack1", std::bind(&Player::AttackEnd, this));
	PlayerImageRenderer->SetEndCallBack("Attack2", std::bind(&Player::AttackEnd, this));
	PlayerImageRenderer->SetEndCallBack("Attack3", std::bind(&Player::AttackEnd, this));
	PlayerImageRenderer->SetEndCallBack("DashAttack", std::bind(&Player::DashAttackEnd, this));
	PlayerImageRenderer->SetEndCallBack("JumpAttack", std::bind(&Player::JumpAttackEnd, this));
}

void Player::SkillAnimation()
{
	SkillEffect1Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	SkillEffect1Renderer_->SetRenderingPipeLine("TextureTrans");

	SkillEffect2Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	SkillEffect2Renderer_->SetRenderingPipeLine("TextureTrans");

	SkillEffect3Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();

	SkillEffect4Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();

	SkillEffect5Renderer_ = CreateTransformComponent<GameEngineImageRenderer>();

	// 단공참
	DungeonStateManager_.CreateState("tripleslash", std::bind(&Player::Tripleslash, this));

	PlayerImageRenderer->CreateAnimationFolder("tripleslash0", "Character", 0.1f, 0, 9, false);
	PlayerImageRenderer->CreateAnimationFolder("tripleslash1", "Character", 0.1f, 188, 193, false);
	PlayerImageRenderer->CreateAnimationFolder("tripleslash2", "Character", 0.1f, 194, 198, false);
	PlayerImageRenderer->CreateAnimationFolder("tripleslash3", "Character", 0.1f, 10, 20, false);
	PlayerImageRenderer->CreateAnimationFolder("tripleslash4", "Character", 0.1f, 199, 203, false);

	SkillEffect1Renderer_->CreateAnimationFolder("tripleslash0", "tripleslash", 0.2f, 0, 3, false);
	SkillEffect1Renderer_->CreateAnimationFolder("tripleslash1", "tripleslash", 0.2f, 4, 7, false);
	SkillEffect1Renderer_->CreateAnimationFolder("tripleslash2", "tripleslash", 0.2f, 8, 11, false);
	SkillEffect1Renderer_->CreateAnimationFolder("tripleslash3", "tripleslash", 0.2f, 12, 15, false);
	SkillEffect1Renderer_->CreateAnimationFolder("tripleslash4", "tripleslash", 0.2f, 16, 19, false);
	SkillEffect2Renderer_->CreateAnimationFolder("tripleslash0", "tripleslash", 0.1f, 20, 23, false);
	SkillEffect2Renderer_->CreateAnimationFolder("tripleslash1", "tripleslash", 0.1f, 24, 27, false);

	// 차지크래시
	DungeonStateManager_.CreateState("chargecrash", std::bind(&Player::Chargecrash, this));

	PlayerImageRenderer->CreateAnimationFolder("chargecrash0", "Character", 0.1f, 143, 146, false);
	PlayerImageRenderer->CreateAnimationFolder("chargecrash1", "Character", 0.1f, 199, 203, false);
	PlayerImageRenderer->SetEndCallBack("chargecrash0", [&]() {	PlayerImageRenderer->SetChangeAnimation("chargecrash1"); });
	PlayerImageRenderer->SetEndCallBack("chargecrash1", [&]() {	DungeonStateManager_.ChangeState("Idle"); });

	SkillEffect1Renderer_->CreateAnimationFolder("chargecrash0", "chargecrash", 0.1f, 0, 3, false);
	SkillEffect1Renderer_->CreateAnimationFolder("chargecrash1", "chargecrash", 0.1f, 4, 8, false);

	// 발도
	DungeonStateManager_.CreateState("momentaryslash", std::bind(&Player::Momentaryslash, this));

	PlayerImageRenderer->CreateAnimationFolder("momentaryslash0", "Character", 0.025f, 10, 20, false);
	PlayerImageRenderer->CreateAnimationFolder("momentaryslash1", "Character", 0.025f, 10, 20, false);
	PlayerImageRenderer->SetEndCallBack("momentaryslash0", std::bind(&Player::MomentaryslashEnd, this));
	PlayerImageRenderer->SetEndCallBack("momentaryslash1", [&]() {	DungeonStateManager_.ChangeState("Idle"); });

	SkillEffect3Renderer_->CreateAnimationFolder("momentaryslashBB", "momentaryslash", 0.05f, 0, 4, false);
	SkillEffect3Renderer_->CreateAnimationFolder("momentaryslashRB", "momentaryslash", 0.05f, 10, 14, false);
	SkillEffect3Renderer_->CreateAnimationFolder("momentaryslashWB", "momentaryslash", 0.05f, 20, 24, false);
	SkillEffect4Renderer_->CreateAnimationFolder("momentaryslashBF", "momentaryslash", 0.05f, 5, 9, false);
	SkillEffect4Renderer_->CreateAnimationFolder("momentaryslashRF", "momentaryslash", 0.05f, 15, 19, false);
	SkillEffect4Renderer_->CreateAnimationFolder("momentaryslashWF", "momentaryslash", 0.05f, 25, 29, false);

	// 류심 충
	DungeonStateManager_.CreateState("flowmindone", std::bind(&Player::Flowmindone, this));

	PlayerImageRenderer->CreateAnimationFolder("flowmindone", "Character", 0.05f, 65, 74, false);
	PlayerImageRenderer->SetEndCallBack("flowmindone", [&]() {	DungeonStateManager_.ChangeState("Idle"); });

	SkillEffect1Renderer_->CreateAnimationFolder("flowmindone", "flowmindone", 0.1f, 0, 5, false);

	// 류심 쾌
	DungeonStateManager_.CreateState("flowmindtwo", std::bind(&Player::Flowmindtwo, this));

	PlayerImageRenderer->CreateAnimationFolder("flowmindtwo0", "Character", 0.05f, 133, 133, false);
	PlayerImageRenderer->CreateAnimationFolder("flowmindtwo1", "Character", 0.1f, 133, 138, false);
	PlayerImageRenderer->SetEndCallBack("flowmindtwo1", std::bind(&Player::FlowmindtwoEnd, this));

	SkillEffect1Renderer_->CreateAnimationFolder("flowmindtwo", "flowmindtwo", 0.15f, 0, 3, false);

	// 류심 승
	DungeonStateManager_.CreateState("flowmindthree", std::bind(&Player::Flowmindthree, this));

	PlayerImageRenderer->CreateAnimationFolder("flowmindthree", "Character", 0.08f, 54, 59, false);

	SkillEffect1Renderer_->CreateAnimationFolder("flowmindthree0", "flowmindthree", 0.15f, 0, 3, false);
	SkillEffect2Renderer_->CreateAnimationFolder("flowmindthree1", "flowmindthree", 0.12f, 4, 8, false);
	SkillEffect1Renderer_->SetEndCallBack("flowmindthree0", [&]() { SkillEffect1Renderer_->AnimaOff(); });
	SkillEffect2Renderer_->SetEndCallBack("flowmindthree1", [&]() { SkillEffect2Renderer_->AnimaOff(); });

	// 가드
	DungeonStateManager_.CreateState("guard", std::bind(&Player::Guard, this));

	PlayerImageRenderer->CreateAnimationFolder("guard", "Character", 0.1f, 123, 124, false);
	
	SkillEffect3Renderer_->CreateAnimationFolder("guard", "guard", 0.04f, 0, 4, false);
	SkillEffect4Renderer_->CreateAnimationFolder("guarddust", "guarddust", 0.03f, 0, 6, false);
	SkillEffect3Renderer_->SetEndCallBack("guard", [&]() { SkillEffect3Renderer_->AnimaOff(); });
	SkillEffect4Renderer_->SetEndCallBack("guarddust", [&]() { SkillEffect4Renderer_->AnimaOff(); });

	// 심검
	DungeonStateManager_.CreateState("MindSword", std::bind(&Player::MindSword, this));

	PlayerImageRenderer->CreateAnimationFolder("MindSword1", "Character", 0.1f, 9, 9, false);
	PlayerImageRenderer->CreateAnimationFolder("MindSword2", "Character", 0.1f, 199, 200, false);

	SkillEffect1Renderer_->CreateAnimationFolder("MindSwordCharge1", "MindSwordCharge1", 0.05f, 0, 5, false);
	SkillEffect1Renderer_->CreateAnimationFolder("MindSwordCharge2", "MindSwordCharge2", 0.05f, 0, 9);	
	SkillEffect1Renderer_->SetEndCallBack("MindSwordCharge1", [&]() { SkillEffect1Renderer_->SetChangeAnimation("MindSwordCharge2"); });

	SkillEffect3Renderer_->CreateAnimationFolder("MindSwordDust1", "MindSwordDust1", 0.05f, 0, 3, true);
	SkillEffect5Renderer_->CreateAnimationFolder("MindSwordHit1", "MindSwordHit1", 0.05f, 0, 4, false);
	SkillEffect4Renderer_->CreateAnimationFolder("MindSwordHit2", "MindSwordHit2", 0.05f, 0, 3, false);
	SkillEffect5Renderer_->CreateAnimationFolder("MindSwordHit3", "MindSwordHit3", 0.1f, 0, 1, false);
	SkillEffect5Renderer_->SetEndCallBack("MindSwordHit3", [&]() {	MindSwordNext_ = true; });

	SkillEffect3Renderer_->CreateAnimationFolder("MindSwordDustUp", "MindSwordDustUp", 0.05f, 0, 8, false);
	SkillEffect4Renderer_->CreateAnimationFolder("MindSwordDustDown", "MindSwordDustDown", 0.05f, 0, 8, false);
	SkillEffect4Renderer_->SetEndCallBack("MindSwordDustDown", [&]() {	DungeonStateManager_.ChangeState("Idle"); });

	// 차지버스트
	DungeonStateManager_.CreateState("ChargeBurst", std::bind(&Player::ChargeBurst, this));

	// 섬단
	DungeonStateManager_.CreateState("Lightning", std::bind(&Player::Lightning, this));

	// 고어 크로스
	DungeonStateManager_.CreateState("Gorecross", std::bind(&Player::Gorecross, this));

	PlayerImageRenderer->CreateAnimationFolder("Gorecross1", "Character", 0.02f, 10, 19, false);
	PlayerImageRenderer->CreateAnimationFolder("Gorecross2", "Character", 0.05f, 201, 203, false);
	PlayerImageRenderer->SetEndCallBack("Gorecross1", [&]() { PlayerImageRenderer->SetChangeAnimation("Gorecross2"); });

	SkillEffect3Renderer_->CreateAnimationFolder("Gorecross", "Gorecross", 0.05f, 0, 10, true);
	SkillEffect3Renderer_->SetFrameCallBack("Gorecross", 4, [&]() { PlayerImageRenderer->SetChangeAnimation("Gorecross2"); });
	SkillEffect3Renderer_->SetEndCallBack("Gorecross", [&]() { DungeonStateManager_.ChangeState("Idle"); });

	// 이기어검술
	DungeonStateManager_.CreateState("TelekineticSwords", std::bind(&Player::TelekineticSwords, this));

	// 무형참
	DungeonStateManager_.CreateState("EtherealSlash", std::bind(&Player::EtherealSlash, this));

	// 천제극섬
	DungeonStateManager_.CreateState("Pentastrike", std::bind(&Player::Pentastrike, this));

}

void Player::Collision()
{
	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Player));
	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 50.f, 30.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, -100.f });

	AttackCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Weapon));
	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 0.f, 0.f, 1.f });
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, -100.f });
}

void Player::FSM()
{
	TownStateManager_.CreateState("TownIdle", std::bind(&Player::TownIdle, this));
	TownStateManager_.CreateState("TownMove", std::bind(&Player::TownMove, this));
	TownStateManager_.ChangeState("TownIdle");

	DungeonStateManager_.CreateState("Idle", std::bind(&Player::Idle, this));
	DungeonStateManager_.CreateState("Move", std::bind(&Player::Move, this));
	DungeonStateManager_.CreateState("Attack", std::bind(&Player::Attack, this));
	DungeonStateManager_.CreateState("Dash", std::bind(&Player::Dash, this));
	DungeonStateManager_.CreateState("DashAttack", std::bind(&Player::DashAttack, this));
	DungeonStateManager_.CreateState("Jump", std::bind(&Player::Jump, this));
	DungeonStateManager_.CreateState("JumpAttack", std::bind(&Player::JumpAttack, this));
	DungeonStateManager_.CreateState("Hit", std::bind(&Player::HitAnimation, this));
	DungeonStateManager_.CreateState("HitAir", std::bind(&Player::HitAnimationAir, this));
	DungeonStateManager_.ChangeState("Idle");
}

void Player::CommandCheck(float _DeltaTime)
{
	if (PrevKey_ != PrevKey::None)
	{
		PrevKeyTime_ -= _DeltaTime;

		if (PrevKeyTime_ <= 0.f)
		{
			PrevKey_ = PrevKey::None;
		}
	}

	if (true == GameEngineInput::GetInst().Down("MoveLeft"))
	{
		if (PrevKey_ == PrevKey::MoveLeft)
		{
			DashLeft_ = true;
		}
		else
		{
			PrevKey_ = PrevKey::MoveLeft;
			PrevKeyTime_ = 0.25f;
		}
	}

	if (true == GameEngineInput::GetInst().Down("MoveRight"))
	{
		if (PrevKey_ == PrevKey::MoveRight)
		{
			DashRight_ = true;
		}
		else
		{
			PrevKey_ = PrevKey::MoveRight;
			PrevKeyTime_ = 0.25f;
		}
	}
}

void Player::SkillCheck()
{
	if (ZPos_ == 0.f && false == IsSkill_)
	{
		if (true == GameEngineInput::GetInst().Down("q"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("q")->second, "q");
		}

		if (true == GameEngineInput::GetInst().Down("w"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("w")->second, "w");
		}

		if (true == GameEngineInput::GetInst().Down("e"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("e")->second, "e");
		}

		if (true == GameEngineInput::GetInst().Down("r"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("r")->second, "r");
		}

		if (true == GameEngineInput::GetInst().Down("t"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("t")->second, "t");
		}

		if (true == GameEngineInput::GetInst().Down("y"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("y")->second, "y");
		}

		if (true == GameEngineInput::GetInst().Down("a"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("a")->second, "a");
		}

		if (true == GameEngineInput::GetInst().Down("s"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("s")->second, "s");
		}

		if (true == GameEngineInput::GetInst().Down("d"))
		{
			std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
			SkillStart(Skill.find("d")->second, "d");
		}

		//if (true == GameEngineInput::GetInst().Down("f"))
		//{
		//	std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
		//	SkillStart(Skill.find("f")->second, "f");
		//}

		//if (true == GameEngineInput::GetInst().Down("g"))
		//{
		//	std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
		//	SkillStart(Skill.find("g")->second, "g");
		//}

		//if (true == GameEngineInput::GetInst().Down("h"))
		//{
		//	std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
		//	SkillStart(Skill.find("h")->second, "h");
		//}

		//if (true == GameEngineInput::GetInst().Down("ctrl"))
		//{
		//	std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
		//	SkillStart(Skill.find("ctrl")->second, "ctrl");
		//}

		//if (true == GameEngineInput::GetInst().Down("shift"))
		//{
		//	std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
		//	SkillStart(Skill.find("shift")->second, "shift");
		//}
	}
}

void Player::DirCheck()
{
	if (DirLeft_)
	{
		Dirf_ = -1.f;

		PlayerImageRenderer->GetTransform()->SetLocalPosition(float4(-15.f, 75.f + ZPos_) + Pivot_);
		PlayerImageRenderer->SetReverse(true);
		SkillEffect1Renderer_->SetReverse(true);
		SkillEffect2Renderer_->SetReverse(true);
		SkillEffect3Renderer_->SetReverse(true);
		SkillEffect4Renderer_->SetReverse(true);
		SkillEffect5Renderer_->SetReverse(true);

		CurAvatar_->GetTransform()->SetLocalPosition(float4(0.f, ZPos_) + Pivot_);
		CurAvatar_->SetReverse(true);
	}
	else
	{
		Dirf_ = 1.f;

		PlayerImageRenderer->GetTransform()->SetLocalPosition(float4(15.f, 75.f + ZPos_) + Pivot_);
		PlayerImageRenderer->SetReverse(false);
		SkillEffect1Renderer_->SetReverse(false);
		SkillEffect2Renderer_->SetReverse(false);
		SkillEffect3Renderer_->SetReverse(false);
		SkillEffect4Renderer_->SetReverse(false);
		SkillEffect5Renderer_->SetReverse(false);

		CurAvatar_->GetTransform()->SetLocalPosition(float4(0.f, ZPos_) + Pivot_);
		CurAvatar_->SetReverse(false);
	}
}

void Player::DebuffCheck(float _DeltaTime)
{
	if (true == DebuffOn_)
	{
		DebuffTime_ -= _DeltaTime;

		if (DebuffTime_ <= 0.f)
		{
			DebuffOn_ = false;
			DebuffTime_ = 0.f;
			DebuffValue_ = 1.f;
			BuffEffectRenderer_->AnimaOff();
		}
	}
}

void Player::TownIdle()
{
	PlayerImageRenderer->SetChangeAnimation("TownIdle");

	if (
		true == GameEngineInput::GetInst().Press("MoveLeft") ||
		true == GameEngineInput::GetInst().Press("MoveRight") ||
		true == GameEngineInput::GetInst().Press("MoveUp") ||
		true == GameEngineInput::GetInst().Press("MoveDown")
		)
	{
		TownStateManager_.ChangeState("TownMove");
	}
}

void Player::TownMove()
{
	Speed = 400.f;

	if (
		false == GameEngineInput::GetInst().Press("MoveLeft") &&
		false == GameEngineInput::GetInst().Press("MoveRight") &&
		false == GameEngineInput::GetInst().Press("MoveUp") &&
		false == GameEngineInput::GetInst().Press("MoveDown")
		)
	{
		TownStateManager_.ChangeState("TownIdle");
	}

	PlayerImageRenderer->SetChangeAnimation("Move");

	MoveBase();
}

void Player::MoveBase()
{
	PrevPos_ = GetTransform()->GetLocalPosition();

	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		if (false == KeyLock_)
		{
			DirLeft_ = true;
		}

		GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * Speed);
		ColorCheckX();
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		if (false == KeyLock_)
		{
			DirLeft_ = false;
		}

		GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * Speed);
		ColorCheckX();
	}
	if (true == GameEngineInput::GetInst().Press("MoveUp"))
	{
		GetTransform()->SetLocalDeltaTimeMove(float4::UP * Speed * 0.5f);
		ColorCheckY();
	}
	if (true == GameEngineInput::GetInst().Press("MoveDown"))
	{
		GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * Speed * 0.5f);
		ColorCheckY();
	}
}

void Player::ColorCheckX()
{
	float4 Color = Map::GetColor(GetTransform());

	if (Color == float4::BLACK)
	{
		float4 Pos;
		Pos.x = PrevPos_.x;
		Pos.y = GetTransform()->GetLocalPosition().y;
		GetTransform()->SetLocalPosition(Pos);
	}
}

void Player::ColorCheckY()
{
	float4 Color = Map::GetColor(GetTransform());

	if (Color == float4::BLACK)
	{
		float4 Pos;
		Pos.x = GetTransform()->GetLocalPosition().x;
		Pos.y = PrevPos_.y;
		GetTransform()->SetLocalPosition(Pos);
	}
}

void Player::Idle()
{
	PlayerImageRenderer->SetChangeAnimation("Idle");
	SkillEffect1Renderer_->AnimaOff();
	SkillEffect2Renderer_->AnimaOff();
	SkillEffect3Renderer_->AnimaOff();
	SkillEffect4Renderer_->AnimaOff();
	SkillEffect5Renderer_->AnimaOff();
	ZPos_ = 0.f;
	Damage_ = 1000;
	KeyLock_ = false;
	IsSkill_ = false;
	SuperArmor_ = false;
	GuardTime_ = 0.f;
	GuardOn_ = false;
	GorecrossNext_ = false;
	CollisionMonsterCheck_ = false;

	if (0 != MonsterList_.size())
	{
		MonsterListClear();
	}

	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, -100.f });

	if (
		true == GameEngineInput::GetInst().Press("MoveLeft") ||
		true == GameEngineInput::GetInst().Press("MoveRight") ||
		true == GameEngineInput::GetInst().Press("MoveUp") ||
		true == GameEngineInput::GetInst().Press("MoveDown")
		)
	{
		DungeonStateManager_.ChangeState("Move");
	}

	if (true == GameEngineInput::GetInst().Down("Attack"))
	{
		++AttackCount_;
		AttackInfo_.SetAttackInfo("Attack", 100, AttackCount_);
		DungeonStateManager_.ChangeState("Attack");
	}

	if (true == GameEngineInput::GetInst().Down("Jump"))
	{
		Sound_->Stop();
		Sound_->PlayOverLap("sm_jump.ogg", 0);
		DungeonStateManager_.ChangeState("Jump");
	}
}

void Player::Move()
{
	if (true == GameEngineInput::GetInst().Up("MoveLeft") ||
		true == GameEngineInput::GetInst().Up("MoveRight"))
	{
		MoveLeft_ = true;
		MoveRight_ = true;
	}

	if (true == GameEngineInput::GetInst().Up("MoveUp") ||
		true == GameEngineInput::GetInst().Up("MoveDown"))
	{
		MoveUp_ = true;
		MoveDown_ = true;
	}

	if (
		false == GameEngineInput::GetInst().Press("MoveLeft") &&
		false == GameEngineInput::GetInst().Press("MoveRight") &&
		false == GameEngineInput::GetInst().Press("MoveUp") &&
		false == GameEngineInput::GetInst().Press("MoveDown")
		)
	{
		DungeonStateManager_.ChangeState("Idle");
	}

	PlayerImageRenderer->SetChangeAnimation("Move");
	PrevPos_ = GetTransform()->GetLocalPosition();
	Speed = 250.f * DebuffValue_;

	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		if (true == DashLeft_)
		{
			DungeonStateManager_.ChangeState("Dash");
		}

		if (true == GameEngineInput::GetInst().Down("MoveRight"))
		{
			MoveLeft_ = false;
			MoveRight_ = true;
		}

		if (true == MoveLeft_)
		{
			DirLeft_ = true;

			GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * Speed);
			ColorCheckX();
		}
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		if (true == DashRight_)
		{
			DungeonStateManager_.ChangeState("Dash");
		}

		if (true == GameEngineInput::GetInst().Down("MoveLeft"))
		{
			MoveLeft_ = true;
			MoveRight_ = false;
		}

		if (true == MoveRight_)
		{
			DirLeft_ = false;

			GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * Speed);
			ColorCheckX();
		}
	}
	if (true == GameEngineInput::GetInst().Press("MoveUp"))
	{
		if (true == GameEngineInput::GetInst().Down("MoveDown"))
		{
			MoveUp_ = false;
			MoveDown_ = true;
		}

		if (true == MoveUp_)
		{
			GetTransform()->SetLocalDeltaTimeMove(float4::UP * Speed * 0.8f);
			ColorCheckY();
		}
	}
	if (true == GameEngineInput::GetInst().Press("MoveDown"))
	{
		if (true == GameEngineInput::GetInst().Down("MoveUp"))
		{
			MoveUp_ = true;
			MoveDown_ = false;
		}

		if (true == MoveDown_)
		{
			GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * Speed * 0.8f);
			ColorCheckY();
		}
	}

	if (true == GameEngineInput::GetInst().Down("Attack"))
	{
		DungeonStateManager_.ChangeState("Attack");
	}

	if (true == GameEngineInput::GetInst().Down("Jump"))
	{
		DungeonStateManager_.ChangeState("Jump");
	}
}

void Player::Attack()
{
	if ("Attack2" != PlayerImageRenderer->GetCurrentAnimationName() &&
		"Attack3" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		if ("Attack1" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_atk_01.ogg", 0);

			++AttackCount_;
			AttackInfo_.SetAttackInfo("Attack", 25, AttackCount_);
		}

		PlayerImageRenderer->SetChangeAnimation("Attack1");

		AttackCollision_->GetTransform()->SetLocalScaling(float4{ 200.f, 50.f, 1.f });

		if (true == DirLeft_)
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -50.f, 0.f, -100.f });
		}
		else
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 50.f, 0.f, -100.f });
		}
	}

	if (true == GameEngineInput::GetInst().Down("Attack") &&
		"Attack3" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		NextAttack_ = true;
	}
}

void Player::AttackFrame()
{
	if (true == NextAttack_)
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, -100.f });
	}
}

void Player::AttackEnd()
{
	if (true == NextAttack_)
	{
		if ("Attack2" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			if ("Attack2" != PlayerImageRenderer->GetCurrentAnimationName())
			{
				Sound_->Stop();
				Sound_->PlayOverLap("sm_atk_02.ogg", 0);

				++AttackCount_;
				AttackInfo_.SetAttackInfo("Attack", 25, AttackCount_);
			}

			PlayerImageRenderer->SetChangeAnimation("Attack2");
			AttackCollision_->GetTransform()->SetLocalScaling(float4{ 250.f, 50.f, 1.f });

			if (true == DirLeft_)
			{
				AttackCollision_->GetTransform()->SetLocalPosition(float4{ -30.f, 0.f, -100.f });
			}
			else
			{
				AttackCollision_->GetTransform()->SetLocalPosition(float4{ 30.f, 0.f, -100.f });
			}
		}
		else
		{
			if ("Attack3" != PlayerImageRenderer->GetCurrentAnimationName())
			{
				Sound_->Stop();
				Sound_->PlayOverLap("sm_atk_03.ogg", 0);

				++AttackCount_;
				AttackInfo_.SetAttackInfo("Attack", 25, AttackCount_);
			}

			PlayerImageRenderer->SetChangeAnimation("Attack3");
			AttackCollision_->GetTransform()->SetLocalScaling(float4{ 250.f, 50.f, 1.f });

			if (true == DirLeft_)
			{
				AttackCollision_->GetTransform()->SetLocalPosition(float4{ -50.f, 0.f, -100.f });
			}
			else
			{
				AttackCollision_->GetTransform()->SetLocalPosition(float4{ 50.f, 0.f, -100.f });
			}
		}

		NextAttack_ = false;
	}
	else
	{
		DungeonStateManager_.ChangeState("Idle");
	}
}

void Player::Dash()
{
	if (true == GameEngineInput::GetInst().Up("MoveUp") ||
		true == GameEngineInput::GetInst().Up("MoveDown"))
	{
		MoveUp_ = true;
		MoveDown_ = true;
	}

	if (true == DashLeft_ &&
		false == GameEngineInput::GetInst().Press("MoveLeft") &&
		false == GameEngineInput::GetInst().Press("MoveUp") &&
		false == GameEngineInput::GetInst().Press("MoveDown") ||
		true == DashRight_ &&
		false == GameEngineInput::GetInst().Press("MoveRight") &&
		false == GameEngineInput::GetInst().Press("MoveUp") &&
		false == GameEngineInput::GetInst().Press("MoveDown"))
	{
		MoveLeft_ = true;
		MoveRight_ = true;
		DashLeft_ = false;
		DashRight_ = false;
		DungeonStateManager_.ChangeState("Idle");
	}

	PlayerImageRenderer->SetChangeAnimation("Dash");
	PrevPos_ = GetTransform()->GetLocalPosition();
	Speed = 500.f * DebuffValue_;

	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		if (true == GameEngineInput::GetInst().Down("MoveRight"))
		{
			MoveRight_ = true;
			MoveLeft_ = false;
			DashLeft_ = false;
			DungeonStateManager_.ChangeState("Move");
		}

		if (true == DashLeft_)
		{
			DirLeft_ = true;

			GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * Speed);
			ColorCheckX();
		}
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		if (true == GameEngineInput::GetInst().Down("MoveLeft"))
		{
			MoveLeft_ = true;
			MoveRight_ = false;
			DashRight_ = false;
			DungeonStateManager_.ChangeState("Move");
		}

		if (true == DashRight_)
		{
			DirLeft_ = false;

			GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * Speed);
			ColorCheckX();
		}
	}
	if (true == GameEngineInput::GetInst().Press("MoveUp"))
	{
		if (true == GameEngineInput::GetInst().Down("MoveDown"))
		{
			MoveUp_ = false;
			MoveDown_ = true;
		}

		if (true == MoveUp_)
		{
			GetTransform()->SetLocalDeltaTimeMove(float4::UP * Speed * 0.4f);
			ColorCheckY();
		}
	}
	if (true == GameEngineInput::GetInst().Press("MoveDown"))
	{
		if (true == GameEngineInput::GetInst().Down("MoveUp"))
		{
			MoveUp_ = true;
			MoveDown_ = false;
		}

		if (true == MoveDown_)
		{
			GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * Speed * 0.4f);
			ColorCheckY();
		}
	}

	if (true == GameEngineInput::GetInst().Down("Attack"))
	{
		DashLeft_ = false;
		DashRight_ = false;
		DungeonStateManager_.ChangeState("DashAttack");
	}

	if (true == GameEngineInput::GetInst().Down("Jump"))
	{
		DashLeft_ = false;
		DashRight_ = false;
		DungeonStateManager_.ChangeState("Jump");
	}
}

void Player::DashAttack()
{
	PlayerImageRenderer->SetChangeAnimation("DashAttack");
	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 120.f, 50.f, 1.f });

	if (true == DirLeft_)
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ -120.f, 0.f, -100.f });
	}
	else
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 120.f, 0.f, -100.f });
	}
}

void Player::DashAttackEnd()
{
	DungeonStateManager_.ChangeState("Idle");
}

void Player::Jump()
{
	if (true == DungeonStateManager_.IsCurrentState("Jump"))
	{
		if (true == JumpUp_)
		{
			PlayerImageRenderer->SetChangeAnimation("JumpUp");
		}
		else
		{
			PlayerImageRenderer->SetChangeAnimation("JumpDown");
		}

		if (true == GameEngineInput::GetInst().Press("Attack"))
		{
			DungeonStateManager_.ChangeState("JumpAttack");
		}
	}
	
	MoveBase();

	if (true == JumpUp_)
	{
		if (Acceleration_ >= 0.f)
		{
			Acceleration_ -= DeltaTime_;
		}

		ZPos_ += ((Acceleration_ * 3.f) * JumpPower_ * DeltaTime_);

		if (0.f >= Acceleration_)
		{
			JumpUp_ = false;
			Acceleration_ = 0.f;
		}
	}
	else
	{
		if (Acceleration_ <= 0.5f)
		{
			Acceleration_ += DeltaTime_;
		}

		ZPos_ -= ((Acceleration_ * 5.f) * JumpPower_ * DeltaTime_);

		if (ZPos_ <= 0.f)
		{
			ZPos_ = 0.f;
			JumpUp_ = true;
			Acceleration_ = 0.5f;

			DungeonStateManager_.ChangeState("Idle");
		}
	}
}

void Player::JumpAttack()
{
	KeyLock_ = true;

	if ("JumpAttack" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		Sound_->Stop();
		Sound_->PlayOverLap("sm_jumpatk_01.ogg", 0);
	}

	PlayerImageRenderer->SetChangeAnimation("JumpAttack");
	Jump();
}

void Player::JumpAttackFrame()
{
	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 80.f, 50.f, 1.f });

	if (true == DirLeft_)
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
	}
	else
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
	}
}

void Player::JumpAttackEnd()
{
	DungeonStateManager_.ChangeState("Jump");
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, -100.f });
}

void Player::HitAnimation()
{
	if (true == HitAnimationChange_)
	{
		if ("Hit1" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_dmg_01.ogg", 0);
		}

		PlayerImageRenderer->SetChangeAnimation("Hit1");
	}
	else
	{
		if ("Hit2" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_dmg_02.ogg", 0);
		}

		PlayerImageRenderer->SetChangeAnimation("Hit2");
	}

	HitTime_ -= DeltaTime_;

	if (HitTime_ <= 0.f)
	{
		HitTime_ = 0.4f;
		DungeonStateManager_.ChangeState("Idle");
	}
}

void Player::HitAnimationAir()
{
	if ("Hit1" != PlayerImageRenderer->GetCurrentAnimationName() &&
		"HitDown" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		Sound_->Stop();
		Sound_->PlayOverLap("sm_dmg_01.ogg", 0);
		PlayerImageRenderer->SetChangeAnimation("Hit1");
	}

	if ("Hit1" == PlayerImageRenderer->GetCurrentAnimationName())
	{
		if (Acceleration_ <= 0.5f)
		{
			Acceleration_ += DeltaTime_;
		}

		ZPos_ -= ((Acceleration_ * 5.f) * JumpPower_ * DeltaTime_);

		if (true == DirLeft_)
		{
			GetTransform()->SetWorldDeltaTimeMove(float4::RIGHT * 40.f);
		}
		else
		{
			GetTransform()->SetWorldDeltaTimeMove(float4::LEFT * 40.f);
		}
	}

	if (ZPos_ <= 0.f)
	{
		ZPos_ = 0.f;
		JumpUp_ = true;
		Acceleration_ = 0.5f;

		PlayerImageRenderer->SetChangeAnimation("HitDown");

		HitTime_ -= DeltaTime_;

		if (HitTime_ <= 0.f)
		{
			HitTime_ = 0.4f;
			DungeonStateManager_.ChangeState("Idle");
		}
	}
}

void Player::SkillStart(SkillSlot* _SkillSlot, std::string _Key)
{
	std::string SkillName = _SkillSlot->GetSkillName();

	if ("CoolTime" == SkillName)
	{
		Sound_->Stop();
		Sound_->PlayOverLap("sm_cooltime.ogg", 0);

		return;
	}

	SkillInfo* Info = GameManager::GetInst().FindSkill(SkillName);

	int MP = Info_.MP - Info->GetMp();

	if (MP < 0)
	{
		Sound_->Stop();
		Sound_->PlayOverLap("sm_nomana.ogg", 0);
		return;
	}

	IsSkill_ = true;
	SuperArmor_ = true;
	SkillKey_ = _Key;

	Info_.MP -= Info->GetMp();

	++AttackCount_;
	AttackInfo_.SetAttackInfo(SkillName, 100 * Info->GetDamage(), AttackCount_);
	_SkillSlot->CoolTime(Info->GetCoolTime());
	DungeonStateManager_.ChangeState(SkillName);
}

void Player::SkillEndCoolTime(std::string _Name)
{
	std::map<std::string, class SkillSlot*> Skill = GameManager::GetInst().GetUI()->GetSkillSlot();
	std::map<std::string, class SkillSlot*>::iterator Iter = Skill.begin();
	std::map<std::string, class SkillSlot*>::iterator IterEnd = Skill.end();

	for (; Iter != IterEnd; ++Iter)
	{
		if ((*Iter).second->GetSkillName() == _Name)
		{
			SkillInfo* Info = GameManager::GetInst().FindSkill(_Name);
			(*Iter).second->CoolTime(Info->GetEndCoolTime());
			return;
		}
	}
}

void Player::MonsterListClear()
{
	std::vector<MonsterBase*>::iterator Iter = MonsterList_.begin();
	std::vector<MonsterBase*>::iterator IterEnd = MonsterList_.end();

	for (; Iter != IterEnd; Iter++)
	{
		if (false == (*Iter)->IsDeath())
		{
			(*Iter)->SetTargetCheck(false);
		}
	}
	
	MonsterList_.clear();
}

void Player::Tripleslash()
{
	TripleslashTime_ -= DeltaTime_;

	Damage_ = 2000;

	if (TripleslashTime_ <= 0.f)
	{
		TripleslashTime_ = 0.3f;

		if (true == TripleslashNext_)
		{
			++TripleslashCount_;
			TripleslashNext_ = false;

			if (true == GameEngineInput::GetInst().Press("MoveLeft"))
			{
				DirLeft_ = true;
			}

			if (true == GameEngineInput::GetInst().Press("MoveRight"))
			{
				DirLeft_ = false;
			}
		}
		else
		{
			TripleslashCount_ = 0;
			SkillEndCoolTime("tripleslash");
			DungeonStateManager_.ChangeState("Idle");
			return;
		}

		if (TripleslashCount_ == 5)
		{
			TripleslashCount_ = 0;
			SkillEndCoolTime("tripleslash");
			DungeonStateManager_.ChangeState("Idle");
			return;
		}
	}

	if (true == GameEngineInput::GetInst().Down(SkillKey_) && TripleslashTime_ <=0.25f)
	{
		TripleslashNext_ = true;
	}

	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 150.f, 50.f, 1.f });

	switch (TripleslashCount_)
	{
	case 0:
		if ("tripleslash0" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_atk_01.ogg", 0);
		}

		PlayerImageRenderer->SetChangeAnimation("tripleslash0");
		SkillEffect1Renderer_->SetChangeAnimation("tripleslash0");
		SkillEffect2Renderer_->SetChangeAnimation("tripleslash0");

		if (DirLeft_)
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-30.f, -10.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		}
		else
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(30.f, -10.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(-75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		}

		break;
	case 1:
		if ("tripleslash1" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_atk_02.ogg", 0);

			++AttackCount_;
			AttackInfo_.SetAttackInfo("Attack", 300, AttackCount_);
		}

		PlayerImageRenderer->SetChangeAnimation("tripleslash1");
		SkillEffect1Renderer_->SetChangeAnimation("tripleslash1");
		SkillEffect2Renderer_->SetChangeAnimation("tripleslash1");

		if (DirLeft_)
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-40.f, 25.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		}
		else
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(40.f, 25.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(-75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		}

		break;
	case 2:
		if ("tripleslash2" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_atk_01.ogg", 0);

			++AttackCount_;
			AttackInfo_.SetAttackInfo("Attack", 300, AttackCount_);
		}

		PlayerImageRenderer->SetChangeAnimation("tripleslash2");
		SkillEffect1Renderer_->SetChangeAnimation("tripleslash2");
		SkillEffect2Renderer_->SetChangeAnimation("tripleslash0");

		if (DirLeft_)
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-30.f, 30.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		}
		else
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(30.f, 30.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(-75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		}

		break;
	case 3:
		if ("tripleslash3" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_atk_02.ogg", 0);

			++AttackCount_;
			AttackInfo_.SetAttackInfo("Attack", 300, AttackCount_);
		}

		PlayerImageRenderer->SetChangeAnimation("tripleslash3");
		SkillEffect1Renderer_->SetChangeAnimation("tripleslash3");
		SkillEffect2Renderer_->SetChangeAnimation("tripleslash1");

		if (DirLeft_)
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-30.f, -20.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		}
		else
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(30.f, -20.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(-75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		}

		break;
	case 4:
		if ("tripleslash4" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_atk_03.ogg", 0);

			++AttackCount_;
			AttackInfo_.SetAttackInfo("Attack", 300, AttackCount_);
		}

		PlayerImageRenderer->SetChangeAnimation("tripleslash4");
		SkillEffect1Renderer_->SetChangeAnimation("tripleslash4");
		SkillEffect2Renderer_->SetChangeAnimation("tripleslash0");

		if (DirLeft_)
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-60.f, 70.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		}
		else
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(60.f, 70.f));
			SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(-75.f, 30.f));
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		}

		break;
	}

	PrevPos_ = GetTransform()->GetLocalPosition();

	if (TripleslashTime_ >= 0.05f)
	{
		if (DirLeft_)
		{
			GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * 400.f);
			ColorCheckX();
		}
		else
		{
			GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * 400.f);
			ColorCheckX();
		}
	}
	else
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, -100.f });
	}
}

void Player::Chargecrash()
{
	AttackInfo_.SetAttackInfo("Attack", 100, AttackCount_);

	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 160.f, 80.f, 1.f });

	if ("chargecrash1" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		if ("chargecrash0" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_chargecrash.ogg", 0);
		}

		PlayerImageRenderer->SetChangeAnimation("chargecrash0");
		SkillEffect1Renderer_->SetChangeAnimation("chargecrash0");

		PrevPos_ = GetTransform()->GetLocalPosition();

		ChargecrashTime_ -= DeltaTime_;

		if (ChargecrashTime_ <= 0.f)
		{
			++AttackCount_;
			ChargecrashTime_ = 0.1f;
		}

		if (DirLeft_)
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-30.f, 30.f));
			GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * 350.f);
			ColorCheckX();
		}
		else
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(30.f, 30.f));
			GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * 350.f);
			ColorCheckX();
		}
	}

	if ("chargecrash1" == PlayerImageRenderer->GetCurrentAnimationName())
	{
		if ("chargecrash0" == SkillEffect1Renderer_->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_chargecrash_up.ogg", 0);
			ChargecrashTime_ = 0.1f;
			++AttackCount_;
			AttackInfo_.SetAttackInfo("Attack", 500, AttackCount_);

			SkillEffect1Renderer_->SetChangeAnimation("chargecrash1");
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, -100.f });
			return;
		}

		if (DirLeft_)
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-30.f, 100.f));
		}
		else
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(30.f, 100.f));
		}
	}
}

void Player::Momentaryslash()
{
	Damage_ = 3000;
	AttackCollision_->GetTransform()->SetLocalScaling(float4( 500.f, 170.f, 1.f ));
	SkillEffect3Renderer_->GetTransform()->SetLocalPosition(float4(0.f, 84.f));
	SkillEffect4Renderer_->GetTransform()->SetLocalPosition(float4(0.f, 85.f));

	if ("momentaryslash1" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		if ("momentaryslash0" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_guard.ogg", 0);
		}

		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 50.f, -100.f });
		PlayerImageRenderer->SetChangeAnimation("momentaryslash0");
		SkillEffect3Renderer_->SetChangeAnimation("momentaryslashBB");
		SkillEffect4Renderer_->SetChangeAnimation("momentaryslashBF");

		if (true == GameEngineInput::GetInst().Down(SkillKey_))
		{
			MomentaryslashNext_ = true;
		}
	}

	if ("momentaryslash1" == PlayerImageRenderer->GetCurrentAnimationName())
	{
		if ("momentaryslashBB" == SkillEffect3Renderer_->GetCurrentAnimationName())
		{
			MomentaryslashNext_ = false;
			SkillEffect3Renderer_->SetChangeAnimation("momentaryslashWB");
			SkillEffect4Renderer_->SetChangeAnimation("momentaryslashWF");
		}

		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 50.f, -100.f });
	}
}

void Player::MomentaryslashEnd()
{
	if (true == MomentaryslashNext_)
	{
		++AttackCount_;
		AttackInfo_.SetAttackInfo("Attack", 1000, AttackCount_);
		PlayerImageRenderer->SetChangeAnimation("momentaryslash1");
	}
	else
	{
		DungeonStateManager_.ChangeState("Idle");
	}
}

void Player::Flowmindone()
{
	Damage_ = 1500;
	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 150.f, 50.f, 1.f });

	if ("flowmindone" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		Sound_->Stop();
		Sound_->PlayOverLap("sm_leap_buff.ogg", 0);
	}

	PlayerImageRenderer->SetChangeAnimation("flowmindone");
	SkillEffect1Renderer_->SetChangeAnimation("flowmindone");
	PrevPos_ = GetTransform()->GetLocalPosition();

	if (DirLeft_)
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ -80.f, 0.f, -100.f });
		SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(0.f, 90.f));
		GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * 200.f);
		ColorCheckX();
	}
	else
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 80.f, 0.f, -100.f });
		SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(0.f, 90.f));
		GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * 200.f);
		ColorCheckX();
	}
}

void Player::Flowmindtwo()
{
	Damage_ = 2000;
	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 150.f, 50.f, 1.f });

	if ("flowmindtwo1" != PlayerImageRenderer->GetCurrentAnimationName() && true == JumpUp_)
	{
		PlayerImageRenderer->SetChangeAnimation("flowmindtwo0");		
	}

	if (true == GameEngineInput::GetInst().Down("x"))
	{
		if (DirLeft_)
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		}
		else
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		}

		Sound_->Stop();
		Sound_->PlayOverLap("sm_gue_01.ogg", 0);

		PlayerImageRenderer->SetChangeAnimation("flowmindtwo1");
		SkillEffect1Renderer_->SetChangeAnimation("flowmindtwo");
	}

	PrevPos_ = GetTransform()->GetLocalPosition();

	if (DirLeft_)
	{
		SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-15.f, 65.f));
		GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * 200.f);
		ColorCheckX();
	}
	else
	{
		SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(15.f, 65.f));
		GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * 200.f);
		ColorCheckX();
	}

	if (true == JumpUp_ && "flowmindtwo1" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		if (Acceleration_ >= 0.f)
		{
			Acceleration_ -= DeltaTime_;
		}

		ZPos_ += ((Acceleration_ * 5.f) * 50.f * DeltaTime_);

		if (0.f >= Acceleration_)
		{
			JumpUp_ = false;
			Acceleration_ = 0.f;
		}
	}
	else
	{
		if ("flowmindtwo1" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			PlayerImageRenderer->SetChangeAnimation("JumpDown");
		}
		
		if (Acceleration_ <= 0.5f)
		{
			Acceleration_ += DeltaTime_;
		}

		ZPos_ -= ((Acceleration_ * 7.f) * 50.f * DeltaTime_);

		if (ZPos_ <= 0.f)
		{
			ZPos_ = 0.f;
			JumpUp_ = true;
			Acceleration_ = 0.5f;

			if ("JumpDown" == PlayerImageRenderer->GetCurrentAnimationName())
			{
				DungeonStateManager_.ChangeState("Idle");
			}
		}
	}
}

void Player::FlowmindtwoEnd()
{
	if (ZPos_ == 0.f)
	{
		DungeonStateManager_.ChangeState("Idle");
	}
	else
	{
		PlayerImageRenderer->SetChangeAnimation("JumpDown");
		SkillEffect1Renderer_->AnimaOff();
	}
}

void Player::Flowmindthree()
{
	Damage_ = 2500;
	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 140.f, 70.f, 1.f });
	PrevPos_ = GetTransform()->GetLocalPosition();

	if (DirLeft_)
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ -60.f, 0.f, -100.f });
		PlayerImageRenderer->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, -30.f));
		CurAvatar_->SetRotation(float4(0.f, 0.f, -30.f));
		Pivot_ = float4(30.f, 0.f);

		SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-40.f, 150.f));
		SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(-40.f, 150.f));
		GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * 100.f);
		ColorCheckX();
	}
	else
	{
		AttackCollision_->GetTransform()->SetLocalPosition(float4{ 60.f, 0.f, -100.f });
		PlayerImageRenderer->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, 30.f));
		CurAvatar_->SetRotation(float4(0.f, 0.f, 30.f));
		Pivot_ = float4(-30.f, 0.f);

		SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(40.f, 150.f));
		SkillEffect2Renderer_->GetTransform()->SetLocalPosition(float4(40.f, 150.f));
		GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * 100.f);
		ColorCheckX();
	}

	if (true == JumpUp_)
	{
		if ("flowmindthree" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_gue_02.ogg", 0);
		}

		PlayerImageRenderer->SetChangeAnimation("flowmindthree");
		SkillEffect1Renderer_->SetChangeAnimation("flowmindthree0");
		SkillEffect2Renderer_->SetChangeAnimation("flowmindthree1");

		if (Acceleration_ >= 0.f)
		{
			Acceleration_ -= DeltaTime_;
		}

		ZPos_ += ((Acceleration_ * 3.f) * JumpPower_ * DeltaTime_);

		if (0.f >= Acceleration_)
		{
			JumpUp_ = false;
			Acceleration_ = 0.f;
		}
	}
	else
	{
		Pivot_ = float4(0.f, 0.f);

		PlayerImageRenderer->SetChangeAnimation("JumpDown");
		PlayerImageRenderer->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, 0.f));
		CurAvatar_->SetRotation(float4(0.f, 0.f, 0.f));

		if (Acceleration_ <= 0.5f)
		{
			Acceleration_ += DeltaTime_;
		}

		ZPos_ -= ((Acceleration_ * 5.f) * JumpPower_ * DeltaTime_);

		if (ZPos_ <= 0.f)
		{
			ZPos_ = 0.f;
			JumpUp_ = true;
			Acceleration_ = 0.5f;
			DungeonStateManager_.ChangeState("Idle");			
		}
	}
}

void Player::Guard()
{
	if (true == GameEngineInput::GetInst().Press(SkillKey_))
	{
		SuperArmor_ = false;
		PlayerImageRenderer->SetChangeAnimation("guard");
		GuardOn_ = true;

		GuardTime_ -= DeltaTime_;

		if (GuardTime_ > 0.f)
		{
			SkillEffect3Renderer_->SetChangeAnimation("guard");
			SkillEffect4Renderer_->SetChangeAnimation("guarddust");
			

			if (true == DirLeft_)
			{
				SkillEffect3Renderer_->GetTransform()->SetLocalPosition(float4(-25.f, 50.f));
				SkillEffect4Renderer_->GetTransform()->SetLocalPosition(float4(-25.f, -5.f));

				GetTransform()->SetWorldDeltaTimeMove(float4::RIGHT * 10.f);
			}
			else
			{
				SkillEffect3Renderer_->GetTransform()->SetLocalPosition(float4(25.f, 50.f));
				SkillEffect4Renderer_->GetTransform()->SetLocalPosition(float4(25.f, -5.f));

				GetTransform()->SetWorldDeltaTimeMove(float4::LEFT * 10.f);
			}
		}
	}
	else
	{
		GuardOn_ = false;
		DungeonStateManager_.ChangeState("Idle");
	}
}

void Player::MindSword()
{
	if (true == GameEngineInput::GetInst().Press(SkillKey_))
	{
		AttackCollision_->GetTransform()->SetLocalScaling(float4(400.f, 170.f, 1.f));
		AttackCollision_->GetTransform()->SetLocalPosition(float4(190.f * Dirf_, 35.f, 1.f));
		CollisionMonsterCheck_ = true;

		if ("MindSword1" != PlayerImageRenderer->GetCurrentAnimationName())
		{
			Sound_->Stop();
			Sound_->PlayOverLap("sm_gue_charge.ogg", 0);

			PlayerImageRenderer->SetChangeAnimation("MindSword1");
			SkillEffect1Renderer_->SetChangeAnimation("MindSwordCharge1");
			SkillEffect3Renderer_->SetChangeAnimation("MindSwordDust1");
			SkillEffect3Renderer_->GetTransform()->SetLocalPosition(float4(210.f * Dirf_, 35.f));
			MindSwordChargeTime_ = 3.f;
		}

		if ("MindSwordCharge1" == SkillEffect1Renderer_->GetCurrentAnimationName())
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-55.f * Dirf_, 45.f));
			SkillEffect1Renderer_->GetTransform()->SetWorldPositionZ(-100.f);
		}

		if ("MindSwordCharge2" == SkillEffect1Renderer_->GetCurrentAnimationName())
		{
			SkillEffect1Renderer_->GetTransform()->SetLocalPosition(float4(-40.f * Dirf_, 25.f));
			SkillEffect1Renderer_->GetTransform()->SetWorldPositionZ(-100.f);
		}

		MindSwordChargeTime_ -= DeltaTime_;
	}

	if (true == GameEngineInput::GetInst().Up(SkillKey_) || MindSwordChargeTime_ <= 0.f)
	{
		SkillEffect1Renderer_->AnimaOff();
		PlayerImageRenderer->SetChangeAnimation("MindSword2");

		Sound_->Stop();
		Sound_->PlayOverLap("sm_gue_02.ogg", 0);

		SkillEffect4Renderer_->SetChangeAnimation("MindSwordHit2");
		SkillEffect5Renderer_->SetChangeAnimation("MindSwordHit3");
		SkillEffect4Renderer_->GetTransform()->SetLocalPosition(float4(60.f * Dirf_, 130.f));
		SkillEffect5Renderer_->GetTransform()->SetLocalPosition(float4(90.f * Dirf_, 130.f));

		DungeonLevel* Level = (DungeonLevel*)GetLevel();
		Level->FadeOut(0.75f);
	}

	if (true == MindSwordNext_)
	{
		std::vector<MonsterBase*>::iterator Iter = MonsterList_.begin();
		std::vector<MonsterBase*>::iterator IterEnd = MonsterList_.end();

		for (; Iter != IterEnd; Iter++)
		{
			if (false == (*Iter)->IsDeath())
			{
				PlayerSkill* Skill = GetLevel()->CreateActor<PlayerSkill>();
				Skill->SkillSetting("MindSword", AttackCount_, (*Iter), &Info_);
				(*Iter)->SetTargetCheck(false);
			}
		}

		MonsterList_.clear();

		SkillEffect5Renderer_->SetChangeAnimation("MindSwordHit1");
		SkillEffect5Renderer_->GetTransform()->SetLocalPosition(float4(220.f * Dirf_, 150.f));

		SkillEffect3Renderer_->SetChangeAnimation("MindSwordDustUp");
		SkillEffect4Renderer_->SetChangeAnimation("MindSwordDustDown");
		SkillEffect3Renderer_->GetTransform()->SetLocalPosition(float4(90.f * Dirf_, 80.f));
		SkillEffect4Renderer_->GetTransform()->SetLocalPosition(float4(90.f * Dirf_, 80.f));

		MindSwordNext_ = false;
		CollisionMonsterCheck_ = false;
	}
}

void Player::Gorecross()
{
	if ("Gorecross1" != PlayerImageRenderer->GetCurrentAnimationName() &&
		"Gorecross2" != PlayerImageRenderer->GetCurrentAnimationName())
	{
		PlayerImageRenderer->SetChangeAnimation("Gorecross1");

		Sound_->Stop();
		Sound_->PlayOverLap("gorecross_hit_01.ogg", 0);

		SkillEffect3Renderer_->SetChangeAnimation("Gorecross");
		SkillEffect3Renderer_->GetTransform()->SetLocalPosition(float4( 0.f, 40.f, 1.f ));
	}

	if ("Gorecross2" == PlayerImageRenderer->GetCurrentAnimationName() && false == GorecrossNext_)
	{
		Sound_->Stop();
		Sound_->PlayOverLap("gorecross_hit_02.ogg", 0);

		GorecrossNext_ = true;
		++AttackCount_;
		SkillInfo* skill = GameManager::GetInst().FindSkill("Gorecross");
		AttackInfo_.SetAttackInfo("Gorecross", skill->GetDamage(), AttackCount_);
	}

	AttackCollision_->GetTransform()->SetLocalScaling(float4(80.f, 60.f, 1.f));
	AttackCollision_->GetTransform()->SetLocalPosition(float4(90.f * Dirf_, 0.f, 1.f));	
}

void Player::TelekineticSwords()
{
	DungeonStateManager_.ChangeState("Idle");
}

void Player::EtherealSlash()
{
	DungeonStateManager_.ChangeState("Idle");
}

void Player::Pentastrike()
{
	DungeonStateManager_.ChangeState("Idle");
}

void Player::ChargeBurst()
{
	DungeonStateManager_.ChangeState("Idle");
}

void Player::Lightning()
{
	DungeonStateManager_.ChangeState("Idle");
}

void Player::ChangeItemImage(const std::string& _Name, ItemType _Type)
{
	CurAvatar_->ChangeAvatar(_Name, _Type);
	GameManager::GetInst().GetInven()->GetAvatarUI()->ChangeAvatar(_Name, _Type);
	GameManager::GetInst().GetAvatarInven()->GetAvatarUI()->ChangeAvatar(_Name, _Type);
}

void Player::Hit(int _Damage, bool _AttackDirLeft)
{
	DamageFont* font = GetLevel()->CreateActor<DamageFont>();
	font->GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition() + float4(0.f, 150.f));
	font->DamageSetting(_Damage, false);

	if (Info_.HP > 0)
	{
		Info_.HP -= 1;
	}

	if (false == SuperArmor_)
	{
		if (true == GuardOn_)
		{
			if (DirLeft_ == _AttackDirLeft)
			{
				GuardTime_ = 0.2f;
				Sound_->Stop();
				Sound_->PlayOverLap("kat_touch.ogg", 0);
				return;
			}
		}

		DirLeft_ = _AttackDirLeft;

		if (0.f != ZPos_)
		{
			HitTime_ = 1.f;
			Acceleration_ = 0.f;
			DungeonStateManager_.ChangeState("HitAir");
		}
		else
		{
			HitTime_ = 0.4f;
			HitAnimationChange_ = !HitAnimationChange_;
			DungeonStateManager_.ChangeState("Hit");
		}
	}
}

void Player::Damage(GameEngineCollision* _Other)
{
	MonsterBase* monster = (MonsterBase*)_Other->GetActor();

	if (true == CollisionMonsterCheck_)
	{
		if (false == monster->GetTargetCheck())
		{
			monster->SetTargetCheck(true);
			MonsterList_.push_back(monster);
		}

		return;
	}

	monster->HitDamage(AttackInfo_, Info_);
}

void Player::HPPercentUp(float _Percent)
{
	float PlusHP = static_cast<float>(Info_.HPMax) * _Percent;

	Info_.HP += static_cast<int>(PlusHP);

	if (Info_.HP > Info_.HPMax)
	{
		Info_.HP = Info_.HPMax;
	}
}

void Player::MPPercentUp(float _Percent)
{
	float PlusMP = static_cast<float>(Info_.MPMax) * _Percent;

	Info_.MP += static_cast<int>(PlusMP);

	if (Info_.MP > Info_.MPMax)
	{
		Info_.MP = Info_.MPMax;
	}
}

void Player::Debuff(float _Time, float _Value)
{
	DebuffOn_ = true;
	DebuffTime_ = _Time;
	DebuffValue_ = _Value;
	BuffEffectRenderer_->SetChangeAnimation("Debuff");
}
