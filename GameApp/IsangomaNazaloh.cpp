#include "Precompile.h"
#include "IsangomaNazaloh.h"
#include "MapBase.h"
#include "NazalohFire.h"
#include "NazalohSummon.h"
#include "NazalohBoom.h"
#include "Warning.h"
#include "GameManager.h"
#include "TopUI.h"
#include "DungeonLevel.h"
#include "TestBossMap.h"
#include "Warning.h"

#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineUIRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

IsangomaNazaloh::IsangomaNazaloh()
	: RandVector_(float4::ZERO)
	, ImageRenderer_(nullptr)
	, FrontEffectRenderer_(nullptr)
	, BackEffectRenderer_(nullptr)
	, Sound_(nullptr)
	, AttackChange_(false)
	, Dirf_(0.f)
	, IsAttack_(false)
	, AlphaValue_(1.f)
	, NextPase_(false)
	, Pase2SummonTime_(0.5f)
	, Pase2SummonCount_(0)
	, Pase2_(false)
	, Pase3_(false)
	, Pase3End_(false)
	, FadeOut_(false)
	, FadeOutTime_(0.5f)
	, Pase3Count_(0)
{
	Pase2SummonPos_[0] = float4(-80.f, 80.f);
	Pase2SummonPos_[1] = float4(-150.f, 0.f);
	Pase2SummonPos_[2] = float4(-80.f, -80.f);
	Pase2SummonPos_[3] = float4(80.f, -80.f);
	Pase2SummonPos_[4] = float4(150.f, 0.f);
	Pase2SummonPos_[5] = float4(80.f, 80.f);
}

IsangomaNazaloh::~IsangomaNazaloh()
{
}

void IsangomaNazaloh::Start()
{
	MonsterBase::Start();

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 40.f));
	ImageRenderer_->CreateAnimationFolder("Idle", "Nazaloh", 0.1f, 0, 0);
	ImageRenderer_->CreateAnimationFolder("Move", "Nazaloh", 0.1f, 1, 8);
	ImageRenderer_->CreateAnimationFolder("Hit", "Nazaloh", 0.5f, 9, 9);
	ImageRenderer_->CreateAnimationFolder("Death", "Nazaloh", 0.1f, 9, 11, false);
	ImageRenderer_->CreateAnimationFolder("Attack", "Nazaloh", 0.1f, 13, 20);
	ImageRenderer_->CreateAnimationFolder("NextPase", "Nazaloh", 0.1f, 0, 0);
	ImageRenderer_->CreateAnimationFolder("Pase2", "Nazaloh", 0.1f, 13, 20);
	ImageRenderer_->SetChangeAnimation("Idle");

	ImageRenderer_->SetEndCallBack("Attack", std::bind(&IsangomaNazaloh::Idle, this));
	ImageRenderer_->SetEndCallBack("Hit", std::bind(&IsangomaNazaloh::Idle, this));

	BackEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	BackEffectRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 50.f, 0.1f));
	BackEffectRenderer_->CreateAnimationFolder("Aura", "Aura_back", 0.05f, 0, 5);

	FrontEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	FrontEffectRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 50.f, -0.1f));
	FrontEffectRenderer_->CreateAnimationFolder("Aura", "Aura_front", 0.05f, 0, 5);

	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 150.f, 400.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	TraceCollision_->GetTransform()->SetLocalScaling(float4{ 400.f, 400.f, 1.f });
	TraceCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 150.f, 40.f, 1.f });
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });

	StateManager_.CreateState("Idle", std::bind(&IsangomaNazaloh::Idle, this));
	StateManager_.CreateState("Move", std::bind(&IsangomaNazaloh::Move, this));
	StateManager_.CreateState("Hit", std::bind(&IsangomaNazaloh::Hit, this));
	StateManager_.CreateState("Attack", std::bind(&IsangomaNazaloh::AttackState, this));
	StateManager_.CreateState("Death", std::bind(&IsangomaNazaloh::MonsterDeath, this));
	StateManager_.CreateState("Summon", std::bind(&IsangomaNazaloh::Summon, this));
	StateManager_.CreateState("NextPase", std::bind(&IsangomaNazaloh::NextPase, this));
	StateManager_.CreateState("Pase2", std::bind(&IsangomaNazaloh::Pase2, this));
	StateManager_.CreateState("Pase3", std::bind(&IsangomaNazaloh::Pase3, this));
	StateManager_.ChangeState("Idle");

	State_.AttackCoolTime_ = 1.5f;
	State_.MoveHoldTime_ = 1.f;
	State_.Speed_ = 100.f;
	State_.HpMax_ = 10000;
	State_.HP_ = State_.HpMax_;
	State_.MoveTime_ = State_.MoveHoldTime_;
	State_.AttackTime_ = State_.AttackCoolTime_;
	State_.IdleTime_ = 2.f;
	State_.DeathTime_ = 3.f;

	MonsterName_ = "보스 - 주술사 나잘로";
	IsBoss_ = true;

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();

	//Sound_->PlayOverLap("peter_meet.ogg", 0);
}

void IsangomaNazaloh::Update(float _DeltaTime)
{
	MonsterBase::Update(_DeltaTime);
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });
	StateManager_.Update();

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&IsangomaNazaloh::Pase3Next, this, std::placeholders::_1), CollisionCheck::Begin);

	NextPatternCheck();
	DirCheck();
}

void IsangomaNazaloh::Attack()
{
	if (State_.AttackTime_ >= 0.f || true == IsHitAnimation_ || true == Invincibility_)
	{
		return;
	}

	SuperArmor_ = true;

	if (true == AttackChange_)
	{
		AttackChange_ = false;
		StateManager_.ChangeState("Summon");
	}
	else
	{
		AttackChange_ = true;
		StateManager_.ChangeState("Attack");
	}
}

void IsangomaNazaloh::Hit()
{
	if (true == SuperArmor_ || true == Invincibility_)
	{
		return;
	}

	StateManager_.ChangeState("Hit");
	IsHitAnimation_ = true;

	if ("Hit" != ImageRenderer_->GetCurrentAnimationName())
	{
		ImageRenderer_->SetChangeAnimation("Hit");

		Sound_->Stop();
		Sound_->PlayOverLap("peter_hit_01.ogg", 0);
	}
}

void IsangomaNazaloh::MonsterDeath()
{
	StateManager_.ChangeState("Death");

	if ("Death" != ImageRenderer_->GetCurrentAnimationName())
	{
		ImageRenderer_->SetChangeAnimation("Death");
		Invincibility_ = true;

		Sound_->Stop();
		Sound_->PlayOverLap("peter_die.ogg", 0);
	}

	State_.DeathTime_ -= DeltaTime_;

	if (State_.DeathTime_ <= 0.f)
	{
		State_.DeathTime_ = FLT_MAX;
		Off();
		CurMap_->BossDeath();
	}
}

void IsangomaNazaloh::NextPatternCheck()
{
	if (false == Pase2_ && State_.HP_ <= 7000 ||
		false == Pase3_ && State_.HP_ <= 4000)
	{
		Invincibility_ = true;

		if ("Idle" != ImageRenderer_->GetCurrentAnimationName())
		{
			ImageRenderer_->SetChangeAnimation("Idle");

			Sound_->Stop();
			Sound_->PlayOverLap("peter_hit_02.ogg", 0);
		}

		StateManager_.ChangeState("NextPase");
	}
}

void IsangomaNazaloh::DirCheck()
{
	if ("Move" == ImageRenderer_->GetCurrentAnimationName() ||
		"Idle" == ImageRenderer_->GetCurrentAnimationName())
	{
		DirLock_ = false;
	}
	else
	{
		DirLock_ = true;
	}

	if (false == DirLock_)
	{
		if (PlayerPos_.x >= BodyCollision_->GetTransform()->GetWorldPosition().x)
		{
			DirLeft_ = false;
			Dirf_ = -1.f;
			ImageRenderer_->SetReverse(false);
		}
		else
		{
			DirLeft_ = true;
			Dirf_ = 1.f;
			ImageRenderer_->SetReverse(true);
		}
	}
}

void IsangomaNazaloh::Idle()
{
	StateManager_.ChangeState("Idle");
	SuperArmor_ = false;
	Invincibility_ = false;
	IsHitAnimation_ = false;
	ImageRenderer_->SetChangeAnimation("Idle");

	State_.IdleTime_ -= DeltaTime_;

	if (State_.IdleTime_ <= 0.f)
	{
		State_.IdleTime_ = 0.5f;

		GameEngineRandom Random;
		RandVector_.x = Random.RandomFloat(-1.f, 1.f);;
		RandVector_.y = Random.RandomFloat(-1.f, 1.f);
		State_.AttackTime_ += 0.2f;
		StateManager_.ChangeState("Move");
	}
}

void IsangomaNazaloh::Move()
{
	ImageRenderer_->SetChangeAnimation("Move");
	PrevPos_ = GetTransform()->GetWorldPosition();
	State_.MoveTime_ -= DeltaTime_;

	if (State_.MoveTime_ <= 0.f)
	{
		State_.MoveTime_ = State_.MoveHoldTime_;
		State_.AttackTime_ += 0.2f;
		StateManager_.ChangeState("Idle");
		return;
	}

	float4 Vector;
	Vector.x = RandVector_.x;
	Vector.y = PlayerPos_.y - GetTransform()->GetWorldPosition().y;

	if (State_.AttackTime_ >= 0.f)
	{
		Vector = RandVector_;
	}

	Vector.Normalize3D();

	GetTransform()->SetLocalDeltaTimeMove(Vector * State_.Speed_);

	if (BodyCollision_->GetTransform()->GetWorldPosition().y > 2.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().y < -250.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().x < -450.f ||
		BodyCollision_->GetTransform()->GetWorldPosition().x > 450.f)
	{
		GetTransform()->SetWorldPosition(PrevPos_);

		State_.MoveTime_ = 0.5f;
		StateManager_.ChangeState("Idle");
		return;
	}
}

void IsangomaNazaloh::AttackState()
{
	if ("Attack" != ImageRenderer_->GetCurrentAnimationName())
	{
		ImageRenderer_->SetChangeAnimation("Attack");

		Sound_->Stop();
		Sound_->PlayOverLap("pipe_atk_01.ogg", 0);

		NazalohFire* Fire = GetLevel()->CreateActor<NazalohFire>();
		State_.AttackTime_ = State_.AttackCoolTime_;

		float4 Pos = BodyCollision_->GetTransform()->GetWorldPosition();
		Fire->GetTransform()->SetWorldPosition(Pos);
		Fire->SetMoveDir(float4(-1.f * Dirf_, 0.f));

		Warning* War = GetLevel()->CreateActor<Warning>();
		War->WarningIine(Pos, Pos - (float4(20.f, 0.f, 0.f) * Dirf_), 30);
	}

	State_.AttackTime_ = State_.AttackCoolTime_;
}

void IsangomaNazaloh::Summon()
{
	if ("Attack" != ImageRenderer_->GetCurrentAnimationName())
	{
		IsAttack_ = true;
		ImageRenderer_->SetChangeAnimation("Attack");

		Sound_->Stop();
		Sound_->PlayOverLap("pipe_atk_01.ogg", 0);

		SummonMonster();
		
		State_.AttackTime_ = State_.AttackCoolTime_;
	}

	State_.AttackTime_ = State_.AttackCoolTime_;
}

void IsangomaNazaloh::SummonMonster()
{
	if (true == IsAttack_)
	{
		NazalohSummon* Monster = GetLevel()->CreateActor<NazalohSummon>();
		Monster->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4(-150.f * Dirf_, 0.f));
		SummonList_.push_back(Monster);
		IsAttack_ = false;
	}
}

void IsangomaNazaloh::SummonBoom()
{
	std::vector<NazalohSummon*>::iterator Iter = SummonList_.begin();
	std::vector<NazalohSummon*>::iterator IterEnd = SummonList_.end();

	for (; Iter != IterEnd; Iter++)
	{
		if (true == (*Iter)->IsUpdate())
		{
			(*Iter)->Boom();
		}
	}

	SummonList_.clear();
}

void IsangomaNazaloh::NextPase()
{
	if (false == Pase2_)
	{
		GetLevel()->GetMainCameraActor()->CameraMoveEvent(float4(-15.f, -100.f));
	}
	else if (false == Pase3_ && false == FadeOut_)
	{
		FadeOut_ = true;
		GetLevel()->FadeOut(1.5f);
	}

	if (false == NextPase_)
	{
		Invincibility_ = true;
		AlphaValue_ -= DeltaTime_;
		ImageRenderer_->SetAlpha(AlphaValue_);

		if (AlphaValue_ <= 0.f)
		{
			NextPase_ = true; 
			SummonBoom();
		}
	}

	if (true == NextPase_)
	{
		ImageRenderer_->SetChangeAnimation("NextPase");
		ImageRenderer_->SetReverse(true);

		AlphaValue_ += DeltaTime_;
		ImageRenderer_->SetAlpha(AlphaValue_);

		if (false == Pase2_)
		{
			GetTransform()->SetWorldPosition(float4(-15.f, -150.f));

			if (AlphaValue_ >= 1.f)
			{
				AlphaValue_ = 1.f;
				NextPase_ = false;
				Pase2_ = true;
				ImageRenderer_->SetAlpha(AlphaValue_);
				StateManager_.ChangeState("Pase2");
				return;
			}
		}
		else if (false == Pase3_)
		{
			GetTransform()->SetWorldPosition(float4(440.f, -150.f, 0.f));

			if (AlphaValue_ >= 1.f)
			{
				AlphaValue_ = 1.f;
				NextPase_ = false;
				Pase3_ = true;
				FadeOut_ = false;
				ImageRenderer_->SetAlpha(AlphaValue_);
				StateManager_.ChangeState("Pase3");
				return;
			}
		}
	}
}

void IsangomaNazaloh::Pase2()
{
	ImageRenderer_->SetReverse(true);

	if (Pase2SummonCount_ != 6)
	{
		FrontEffectRenderer_->SetChangeAnimation("Aura");
		BackEffectRenderer_->SetChangeAnimation("Aura");
		ImageRenderer_->SetChangeAnimation("Pase2");

		Pase2SummonTime_ -= DeltaTime_;

		if (Pase2SummonTime_ <= 0.f)
		{
			Pase2SummonTime_ = 0.5f;
			NazalohSummon* Monster = GetLevel()->CreateActor<NazalohSummon>();
			Monster->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + Pase2SummonPos_[Pase2SummonCount_]);
			Monster->Pase2Setting(Pase2SummonCount_);
			SummonList_.push_back(Monster);
			++Pase2SummonCount_;
		}

		if (Pase2SummonCount_ == 6)
		{
			GetLevel()->GetMainCameraActor()->CameraReset();
		}
	}
	else
	{
		for (size_t i = 1; i < 6; i++)
		{
			if (true == SummonList_[i]->DeathCheck() && false == SummonList_[i - 1]->DeathCheck())
			{
				for (size_t i = 0; i < 6; i++)
				{
					SummonList_[i]->Revive();
				}
				break;
			}
		}
		
		if (true == SummonList_[5]->DeathCheck() && true == SummonList_[4]->DeathCheck())
		{
			SummonBoom();
			StateManager_.ChangeState("Idle");
			FrontEffectRenderer_->AnimaOff();
			BackEffectRenderer_->AnimaOff();
		}
	}
}

void IsangomaNazaloh::Pase3()
{
	GameManager::GetInst().GetUI()->UIOnOff(false);
	TestBossMap* map = (TestBossMap*)CurMap_;
	map->MapColorReverse(true);

	if ("Pase2" != ImageRenderer_->GetCurrentAnimationName())
	{
		GameManager::GetInst().GetPlayer()->GetTransform()->SetWorldPosition(float4(-435.f, -150.f, 0.f));

		GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, 180.f));
		GetLevel()->GetMainCameraActor()->CameraHolding(float4(0.f, 70.f), 1.25f);
		GetLevel()->FadeIn(0.5f);

		FrontEffectRenderer_->SetChangeAnimation("Aura");
		BackEffectRenderer_->SetChangeAnimation("Aura");
		ImageRenderer_->SetChangeAnimation("Pase2");

		for (size_t i = 0; i < 6; i++)
		{
			NazalohBoom* Boom = GetLevel()->CreateActor<NazalohBoom>();
			Boom->GetTransform()->SetWorldPosition(float4(-300.f + (i*120.f), -180.f));
			BoomList_.push_back(Boom);
		}

		for (size_t i = 0; i < 6; i++)
		{
			NazalohBoom* Boom = GetLevel()->CreateActor<NazalohBoom>();
			Boom->GetTransform()->SetWorldPosition(float4(-300.f + (i * 120.f), -60.f));
			BoomList_.push_back(Boom);
		}

		for (size_t i = 0; i < 6; i++)
		{
			BoomList_[i]->BoomDelay(true);
			BoomList_[i + 6]->BoomDelay(false);
		}

		{
			Warning* War = GetLevel()->CreateActor<Warning>();
			War->GetTransform()->SetWorldPosition(float4(-420.f, -150.f));
			War->GetTransform()->SetWorldRotationDegree(float4(0.f, 0.f, -90.f));
			WarningList_.push_back(War);
		}

		{
			Warning* War = GetLevel()->CreateActor<Warning>();
			War->GetTransform()->SetWorldPosition(float4(420.f, -150.f));
			War->GetTransform()->SetWorldRotationDegree(float4(0.f, 0.f, 90.f));
			WarningList_.push_back(War);
		}
	}

	if (true == FadeOut_)
	{
		FadeOutTime_ -= DeltaTime_;

		if (FadeOutTime_ <= 0.f)
		{
			FadeOut_ = false;
			FadeOutTime_ = 0.5f;
			Pase3NextPattern();
		}
	}
}

void IsangomaNazaloh::Pase3Next(GameEngineCollision* _Other)
{
	if (false == Pase3_ || true == Pase3End_)
	{
		return;
	}

	FadeOut_ = true;

	GetLevel()->FadeOut(0.5f);
}

void IsangomaNazaloh::Pase3NextPattern()
{
	++Pase3Count_;
	GameManager::GetInst().GetPlayer()->GetTransform()->SetWorldPosition(float4(-435.f, -150.f, 0.f));

	GetLevel()->FadeIn(0.5f);

	if (Pase3Count_ == 1)
	{
		WarningList_[0]->WarningAnimation("VioletLine");
		WarningList_[1]->WarningAnimation("VioletLine");

		for (size_t i = 0; i < 3; i++)
		{
			BoomList_[i * 4]->BoomDelay(true);
			BoomList_[i * 4 + 1]->BoomDelay(true);
			BoomList_[i * 4 + 2]->BoomDelay(false);
			BoomList_[i * 4 + 3]->BoomDelay(false);
		}
	}
	else if (Pase3Count_ == 2)
	{
		WarningList_[0]->WarningAnimation("RedLine");
		WarningList_[1]->WarningAnimation("RedLine");

		for (size_t i = 0; i < 6; i++)
		{
			if (i % 2 == 0)
			{
				BoomList_[i]->BoomDelay(true);
				BoomList_[i + 6]->BoomDelay(false);
			}
			else
			{
				BoomList_[i]->BoomDelay(false);
				BoomList_[i + 6]->BoomDelay(true);
			}
		}
	}
	else
	{
		WarningList_[0]->Off();
		WarningList_[1]->Off();

		for (size_t i = 0; i < 12; i++)
		{
			BoomList_[i]->Off();
		}
		WarningList_.clear();
		BoomList_.clear();

		GetLevel()->GetMainCameraActor()->CameraReset();
		GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree(float4(0.f, 0.f, 0.f));

		Pase3End_ = true;
		StateManager_.ChangeState("Idle");
		FrontEffectRenderer_->AnimaOff();
		BackEffectRenderer_->AnimaOff();
		GameManager::GetInst().GetUI()->UIOnOff(true);
		TestBossMap* map = (TestBossMap*)CurMap_;
		map->MapColorReverse(false);
	}
}
