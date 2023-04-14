#include "Precompile.h"
#include "PeterThePiper.h"
#include "PeterPase2.h"
#include "Pase2Rythem.h"
#include "PeterFire.h"
#include "MapBase.h"
#include "GameManager.h"
#include "TopUI.h"
#include "PeterTrap.h"
#include "Mouse.h"

#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineUIRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

PeterThePiper::PeterThePiper()
	: RandVector_(float4::ZERO)
	, ImageRenderer_(nullptr)
	, EffectRenderer_(nullptr)
	, EffectRenderer2_(nullptr)
	, AttackCount_(0)
	, NextPaseStartTime_(5.f)
	, Pase2UpTime_(1.5f)
	, Pase2UIOnTime_(0.5f)
	, Pase3UpTime_(1.5f)
	, Pase3AttackTime(0.1f)
	, ReviveEnd_(false)
	, Pase2_(false)
	, Pase2End_(false)
	, Pase3_(false)
	, Pase2Count_(0)
	, Pase2UIIndex_(0)
	, Pase3Count_(0)
	, IsAttack_(false)
	, Sound_(nullptr)
	, PeterTrap_(nullptr)
	, DeathTimeScale_(0.f)
{
}

PeterThePiper::~PeterThePiper()
{
}

void PeterThePiper::Start()
{
	MonsterBase::Start();

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4( 0.f, 70.f));
	ImageRenderer_->CreateAnimationFolder("Idle", "PeterThePiper", 0.1f, 0, 3);
	ImageRenderer_->CreateAnimationFolder("Hit", "PeterThePiper", 0.5f, 4, 4);
	ImageRenderer_->CreateAnimationFolder("Move", "PeterThePiper", 0.1f, 5, 12);
	ImageRenderer_->CreateAnimationFolder("Death", "PeterThePiper", 0.1f, 13, 37, false);
	ImageRenderer_->CreateAnimationFolder("RealDeath", "PeterThePiper", 0.1f, 13, 37, false);
	ImageRenderer_->CreateAnimationFolder("Sing", "PeterThePiper", 0.2f, 38, 41);
	ImageRenderer_->CreateAnimationFolder("Attack", "PeterThePiper", 0.1f, 43, 48);
	ImageRenderer_->CreateAnimationFolder("Revive", "PeterThePiperEffect2", 0.1f, 0, 11, false);
	ImageRenderer_->CreateAnimationFolder("ReviveSing", "PeterThePiper", 0.2f, 39, 41);
	ImageRenderer_->SetChangeAnimation("Idle");

	ImageRenderer_->SetFrameCallBack("Sing", 40, std::bind(&PeterThePiper::Fire, this));

	ImageRenderer_->SetEndCallBack("Attack", std::bind(&PeterThePiper::Idle, this));
	ImageRenderer_->SetEndCallBack("Sing", std::bind(&PeterThePiper::Idle, this));
	ImageRenderer_->SetEndCallBack("Hit", std::bind(&PeterThePiper::Idle, this));
	ImageRenderer_->SetEndCallBack("Revive", [&](){	ReviveEnd_ = true; });

	EffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	EffectRenderer_->CreateAnimationFolder("Empty", "PeterThePiperEffect2", 0.05f, 0, 0, false);
	EffectRenderer_->CreateAnimationFolder("ReviveR", "PeterThePiperEffect2", 0.1f, 27, 38, false);
	EffectRenderer_->CreateAnimationFolder("Rythem", "PeterThePiperEffect", 0.1f, 78, 88);
	EffectRenderer_->SetChangeAnimation("Empty");
	EffectRenderer_->SetRenderingPipeLine("TextureTrans");

	EffectRenderer_->SetEndCallBack("ReviveR", std::bind(&PeterThePiper::Effect, this));
	EffectRenderer_->SetFrameCallBack("ReviveR", 33, [&]() {	ImageRenderer_->SetChangeAnimation("ReviveSing"); });

	EffectRenderer2_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	EffectRenderer2_->CreateAnimationFolder("Empty", "PeterThePiperEffect2", 0.05f, 0, 0, false);
	EffectRenderer2_->CreateAnimationFolder("PeterShockWave", "PeterShockWave", 0.1f, 0, 3, false);
	EffectRenderer2_->SetChangeAnimation("Empty");

	EffectRenderer2_->SetEndCallBack("PeterShockWave", [&]() {	EffectRenderer2_->SetChangeAnimation("Empty"); });

	BodyCollision_->GetTransform()->SetLocalScaling(float4{ 70.f, 40.f, 1.f });
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	TraceCollision_->GetTransform()->SetLocalScaling(float4{ 200.f, 40.f, 1.f });
	TraceCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f, 0.f });

	AttackCollision_->GetTransform()->SetLocalScaling(float4{ 150.f, 40.f, 1.f });
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });

	StateManager_.CreateState("Idle", std::bind(&PeterThePiper::Idle, this));
	StateManager_.CreateState("Move", std::bind(&PeterThePiper::Move, this));
	StateManager_.CreateState("Attack", std::bind(&PeterThePiper::AttackState, this));
	StateManager_.CreateState("Sing", std::bind(&PeterThePiper::SingAttack, this));
	StateManager_.CreateState("Hit", std::bind(&PeterThePiper::Hit, this));
	StateManager_.CreateState("NextPase", std::bind(&PeterThePiper::NextPase, this));
	StateManager_.CreateState("Pase2", std::bind(&PeterThePiper::Pase2, this));
	StateManager_.CreateState("Pase3", std::bind(&PeterThePiper::Pase3, this));
	StateManager_.CreateState("Death", std::bind(&PeterThePiper::MonsterDeath, this));
	StateManager_.ChangeState("Idle");

	State_.AttackCoolTime_ = 1.5f;
	State_.MoveHoldTime_ = 1.f;
	State_.Speed_ = 250.f;
	State_.HpMax_ = 10000;
	State_.HP_ = State_.HpMax_;
	State_.MoveTime_ = State_.MoveHoldTime_;
	State_.AttackTime_ = State_.AttackCoolTime_;
	State_.IdleTime_ = 2.f;
	State_.DeathTime_ = 3.f;

	MonsterName_ = "보스 - 피터 더 파이퍼";
	IsBoss_ = true;
	// GameManager::GetInst().GetUI()->MonsterHPBarOn(this, true);

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();

	Sound_->PlayOverLap("peter_meet.ogg", 0);

	PeterTrap_ = GetLevel()->CreateActor<PeterTrap>();
}

void PeterThePiper::Update(float _DeltaTime)
{
	MonsterBase::Update(_DeltaTime);
	AttackCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });
	StateManager_.Update();

	NestPatternCheck();
	DirCheck();

	if (DeathTimeScale_ >= 0.f)
	{
		DeathTimeScale_ -= _DeltaTime;

		if (DeathTimeScale_ <= 0.f)
		{
			GetLevel()->SetTimeScale(1.f);
		}
	}
}

void PeterThePiper::Attack()
{
	if (State_.AttackTime_ >= 0.f || true == IsHitAnimation_ || true == Invincibility_)
	{
		return;
	}

	SuperArmor_ = true;

	++AttackCount_;

	if (AttackCount_ == 3)
	{
		TraceCollision_->GetTransform()->SetLocalScaling(float4{ 200.f, 40.f, 1.f });
		AttackCount_ = 0;
		StateManager_.ChangeState("Sing");
	}
	else
	{
		if (AttackCount_ == 2)
		{
			if (true == Pase2_)
			{
				TraceCollision_->GetTransform()->SetLocalScaling(float4{ 600.f, 360.f, 1.f });
			}
			else
			{
				TraceCollision_->GetTransform()->SetLocalScaling(float4{ 480.f, 240.f, 1.f });
			}
		}

		StateManager_.ChangeState("Attack");
	}
}

void PeterThePiper::Hit()
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

void PeterThePiper::MonsterDeath()
{
	StateManager_.ChangeState("Death");

	if ("RealDeath" != ImageRenderer_->GetCurrentAnimationName())
	{
		ImageRenderer_->SetChangeAnimation("RealDeath");
		PeterTrap_->SetPlay(false, false, true);

		GetLevel()->SetTimeScale(0.3f);
		DeathTimeScale_ = 0.6f;

		Sound_->Stop();
		Sound_->PlayOverLap("peter_die.ogg", 0);
	}

	Invincibility_ = true;

	State_.DeathTime_ -= DeltaTime_;

	if (State_.DeathTime_ <= 0.f)
	{
		State_.DeathTime_ = FLT_MAX;
		CurMap_->BossDeath();

		GetLevel()->FadeOut(1.0f);
	}
}

void PeterThePiper::NestPatternCheck()
{
	if (false == Pase2_ && State_.HP_ <= 7000 ||
		false == Pase3_ && State_.HP_ <= 4000)
	{
		PeterTrap_->SetPlay(false);
		Invincibility_ = true;

		if ("Death" != ImageRenderer_->GetCurrentAnimationName())
		{
			ImageRenderer_->SetChangeAnimation("Death");

			GetLevel()->SetTimeScale(0.3f);
			DeathTimeScale_ = 0.6f;

			Sound_->Stop();
			Sound_->PlayOverLap("peter_hit_02.ogg", 0);
		}

		StateManager_.ChangeState("NextPase");
	}
}

void PeterThePiper::DirCheck()
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
			ImageRenderer_->SetReverse(false);
		}
		else
		{
			DirLeft_ = true;
			ImageRenderer_->SetReverse(true);
		}
	}
}

void PeterThePiper::Idle()
{
	StateManager_.ChangeState("Idle");
	SuperArmor_ = false;
	Invincibility_ = false;
	IsHitAnimation_ = false;
	IsAttack_ = false;
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

void PeterThePiper::Move()
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
	Vector.x = PlayerPos_.x - GetTransform()->GetWorldPosition().x;
	Vector.y = PlayerPos_.y - GetTransform()->GetWorldPosition().y;

	if (State_.AttackTime_ >= 0.f)
	{
		Vector = RandVector_;
	}

	Vector.Normalize3D();

	GetTransform()->SetLocalDeltaTimeMove(Vector * State_.Speed_);

	if (BodyCollision_->GetTransform()->GetWorldPosition().y > 0.f ||
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

void PeterThePiper::AttackState()
{
	if ("Attack" != ImageRenderer_->GetCurrentAnimationName())
	{
		ImageRenderer_->SetChangeAnimation("Attack");

		Sound_->Stop();
		Sound_->PlayOverLap("pipe_atk_01.ogg", 0);
	}

	State_.AttackTime_ = State_.AttackCoolTime_;

	if (false == IsAttack_)
	{
		IsAttack_ = true;

		if (true == DirLeft_)
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ -40.f, 0.f, 0.f });
		}
		else
		{
			AttackCollision_->GetTransform()->SetLocalPosition(float4{ 40.f, 0.f, 0.f });
		}
	}
}

void PeterThePiper::SingAttack()
{
	if ("Sing" != ImageRenderer_->GetCurrentAnimationName())
	{
		ImageRenderer_->SetChangeAnimation("Sing");

		Sound_->Stop();
		Sound_->PlayOverLap("pipe_03.ogg", 0);
	}

	if (true == Pase3_)
	{
		Pase3AttackTime -= DeltaTime_;

		if (Pase3AttackTime <= 0.f)
		{
			Pase3AttackTime = 0.1f;

			PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
			State_.AttackTime_ = State_.AttackCoolTime_;

			float4 Pos = PlayerPos_;
			Pos.y += 65.f;
			Fire->GetTransform()->SetWorldPosition(Pos);
			Fire->Pase3();
			Fire->SetMoveVector(PlayerPos_);
		}
	}
}

void PeterThePiper::Fire()
{
	if (true == Pase3_)
	{
		return;
	}

	if (State_.AttackTime_ <= 0.f)
	{
		State_.AttackTime_ = State_.AttackCoolTime_;
		PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();

		float4 Pos = BodyCollision_->GetTransform()->GetWorldPosition();
		Fire->GetTransform()->SetWorldPosition(Pos);

		Pos = PlayerPos_;
		Pos.y += 65.f;

		Fire->GetTransform()->SetWorldPosition(Pos);
		Fire->Pase2();
	}
}

void PeterThePiper::NextPase()
{
	NextPaseStartTime_ -= DeltaTime_;

	if (NextPaseStartTime_ <= 0.f)
	{
		if (true == Pase2End_)
		{
			GetTransform()->SetWorldPosition(float4(-15.f, -75.f, -1.f));
		}
		else
		{
			GetTransform()->SetWorldPosition(float4(-15.f, 0.f, -1.f));
		}

		ImageRenderer_->SetReverse(false);

		if ("Revive" != ImageRenderer_->GetCurrentAnimationName())
		{
			ImageRenderer_->SetChangeAnimation("Revive");

			Sound_->Stop();
			Sound_->PlayOverLap("mouse_tower.ogg", 0);
		}

		NextPaseStartTime_ = FLT_MAX;

		if (false == Pase2_)
		{
			Pase2_ = true;
		}
		else if (false == Pase3_)
		{
			Pase3_ = true;
		}
	}

	if (true == Pase3_)
	{
		GetLevel()->GetMainCameraActor()->CameraMoveEvent(float4(-15.f, -15.f));
	}
	else if (true == Pase2_ && false == Pase2End_)
	{
		GetLevel()->GetMainCameraActor()->CameraMoveEvent(float4(-15.f, 75.f));
	}

	if (true == ReviveEnd_ && true == Pase3_)
	{
		Pase2UIOnTime_ -= DeltaTime_;

		if (Pase3Count_ == 5 && Pase2UIOnTime_ <= 0.f)
		{
			Revive();
			ReviveEnd_ = false;

			EffectRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 70.f));
			EffectRenderer_->SetChangeAnimation("ReviveR");

			StateManager_.ChangeState("Pase3");
		}
		else if (Pase2UIOnTime_ <= 0.f)
		{
			Pase2UIOnTime_ = 0.5f;
			Pase2UI(125.f);
			++Pase3Count_;
		}
	}
	else if (true == ReviveEnd_ && true == Pase2_)
	{
		Pase2UIOnTime_ -= DeltaTime_;

		if (Pase2Count_ == 5 && Pase2UIOnTime_ <= 0.f)
		{
			Revive();
			ReviveEnd_ = false;
			Pase2UIOnTime_ = 0.5f;
			NextPaseStartTime_ = 5.f;

			EffectRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 70.f));
			EffectRenderer_->SetChangeAnimation("ReviveR");

			StateManager_.ChangeState("Pase2");
		}
		else if (Pase2UIOnTime_ <= 0.f)
		{
			Pase2UIOnTime_ = 0.5f;
			Pase2UI(200.f);
			++Pase2Count_;
		}
	}
}

void PeterThePiper::Revive()
{
	if (true == Pase3_)
	{
		for (size_t x = 0; x < 2; x++)
		{
			for (size_t y = 0; y < 2; y++)
			{
				PeterPase2* Actor = GetLevel()->CreateActor<PeterPase2>();
				Actor->GetTransform()->SetWorldPosition(float4(x * 850.f - 425.f, y * -150.f - 40.f, 0.f));
				Actor->SetPeter(this);
				Pase3List_.push_back(Actor);
			}
		}

		Sound_->Stop();
		Sound_->PlayOverLap("pipe_02.ogg", 2);
	}
	else if (true == Pase2_)
	{
		for (size_t x = 0; x < 6; x++)
		{
			for (size_t y = 0; y < 3; y++)
			{
				PeterPase2* Actor = GetLevel()->CreateActor<PeterPase2>();
				Actor->GetTransform()->SetWorldPosition(float4(x * 150.f - 400.f, (y * -90.f) - 30.f));
				Actor->SetPeter(this);
				Pase2List_.push_back(Actor);
			}
		}

		Sound_->Stop();
		Sound_->PlayOverLap("pipe_01.ogg", 2);

		GameManager::GetInst().GetMouse()->MouseChange(true);
		GameManager::GetInst().ChangeSound("hameln_boss.ogg");
	}
}

void PeterThePiper::Effect()
{
	EffectRenderer_->GetTransform()->SetLocalPosition(float4(-20.f, 160.f));
	EffectRenderer_->SetChangeAnimation("Rythem");
}

void PeterThePiper::Pase2()
{
	GetTransform()->SetWorldPosition(float4(-15.f, 0.f, -1.f));
	GetLevel()->GetMainCameraActor()->CameraReset();

	if (Pase2List_.size() != 0)
	{
		Pase2UpTime_ -= DeltaTime_;

		if (Pase2UpTime_ <= 0.f)
		{
			Pase2UpTime_ = 0.5f;

			GameEngineRandom Random;

			int Number = Random.RandomInt(0, 17);
			
			if (false == Pase2List_[Number]->GetIsUp())
			{
				Pase2List_[Number]->Up();
			}
		}
	}

	if (Pase2Count_ == 0)
	{
		for (size_t i = 0; i < Pase2List_.size(); i++)
		{
			Pase2List_[i]->Death();
		}

		for (size_t i = 0; i < Pase2UIList_.size(); i++)
		{
			Pase2UIList_[i]->Death();
		}

		Pase2End_ = true;
		EffectRenderer_->SetChangeAnimation("Empty");

		Sound_->Stop();
		GameManager::GetInst().GetMouse()->MouseChange(false);
		StateManager_.ChangeState("Idle");
		PeterTrap_->SetPlay(true);
	}
}

void PeterThePiper::Pase2UI(float _YVelue)
{
	Pase2Rythem* Actor = GetLevel()->CreateActor<Pase2Rythem>();

	if (true == Pase3_)
	{
		Actor->GetTransform()->SetWorldPosition(float4(55.f * Pase3Count_ - 120.f, _YVelue, -10.f));
		Actor->Clear();
		Pase3UIList_.push_back(Actor);
	}
	else
	{
		Actor->GetTransform()->SetWorldPosition(float4(55.f * Pase2Count_ - 120.f, _YVelue, -10.f));

		GameEngineRandom Random;
		int Number = Random.RandomInt(0, 1);

		if (Number == 0)
		{
			Actor->SetAnimationIndex(Number);
		}
		else if (Number == 1)
		{
			Actor->SetAnimationIndex(Number);
		}

		Pase2UIList_.push_back(Actor);
	}
}

void PeterThePiper::Pase3()
{
	GetTransform()->SetWorldPosition(float4(0.f, -75.f, -1.f));
	GetLevel()->GetMainCameraActor()->CameraReset();

	if (Pase3List_.size() != 0)
	{
		Pase3UpTime_ -= DeltaTime_;

		if (Pase3UpTime_ <= 0.f)
		{
			Pase3UpTime_ = 1.5f;

			GameEngineRandom Random;

			int Number = Random.RandomInt(0, 3);

			for (size_t i = 0; i < Pase3List_.size(); i++)
			{
				if (i == Number)
				{
					Pase3List_[i]->White();
				}
				else
				{
					Pase3List_[i]->Gray();
				}
			}
		}
	}

	if (Pase3Count_ == 0)
	{
		for (size_t i = 0; i < Pase3List_.size(); i++)
		{
			Pase3List_[i]->Death();
		}

		EffectRenderer_->SetChangeAnimation("Empty");
	
		Sound_->Stop();
		StateManager_.ChangeState("Idle");
		PeterTrap_->SetPlay(true, true);
	}
}

void PeterThePiper::ShockWave()
{
	if ("PeterShockWave" != EffectRenderer2_->GetCurrentAnimationName())
	{
		Sound_->Stop();
		Sound_->PlayOverLap("supersuplex_down.ogg", 0);

		EffectRenderer2_->SetChangeAnimation("PeterShockWave");
	}

	EffectRenderer2_->GetTransform()->SetWorldPosition(BodyCollision_->GetTransform()->GetWorldPosition());
	EffectRenderer2_->GetTransform()->SetWorldPositionZ(static_cast<float>(ZOrder::GroundEffectUp));

	float PlayerZPos = GameManager::GetInst().GetPlayerZPos();

	float AttackTopPos = 10.f;
	float AttackBottomPos = 0.f;
	int AttackDamage = 500;
	bool PlayerDirection = GameManager::GetInst().GetPlayer()->GetPlayerDirection();

	if (PlayerZPos <= AttackTopPos && PlayerZPos >= AttackBottomPos)
	{
		GameManager::GetInst().GetPlayer()->Hit(AttackDamage, PlayerDirection);		
	}
}

void PeterThePiper::Pase2Count(int _Color)
{
	if (Pase2UIList_[Pase2UIIndex_]->GetColor() == _Color)
	{
		Pase2UIList_[Pase2UIIndex_]->Clear();
		++Pase2UIIndex_;
		--Pase2Count_;
	}
	else
	{
		ShockWave();
	}
}

void PeterThePiper::Pase3Count()
{
	if (Pase3Count_ < 0)
	{
		return;
	}

	--Pase3Count_;
	Pase3UIList_[Pase3Count_]->Death();
}
