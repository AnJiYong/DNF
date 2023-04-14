#include "Precompile.h"
#include "PeterTrap.h"
#include "GlobalEnum.h"
#include "Player.h"
#include "GameManager.h"
#include "PeterFire.h"
#include "Warning.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

PeterTrap::PeterTrap()
	: ImageRenderer_(nullptr)
	, AuraRenderer_(nullptr)
	, Sound_(nullptr)
	, SoundOn_(false)
	, BodyCollision_(nullptr)
	, Play_(false)
	, Pase3_(false)
	, DirLeft_(false)
	, IsFire_(false)
	, AttackTime_(3.f)
	, ButterFlyTime_(4.f)
	, ButterFlyChange_(false)
	, ButterFlyCount_(0)
	, ButterFlyHardModeTime_(0.1f)
	, RandPos_(float4::ZERO)
	, PeterDeath_(false)
{
}

PeterTrap::~PeterTrap()
{
}

void PeterTrap::Start()
{
	AuraRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	AuraRenderer_->SetRenderingPipeLine("TextureTrans");
	AuraRenderer_->CreateAnimationFolder("Aura", "PeterThePiperEffect", 0.15f, 46, 50, false);
	AuraRenderer_->CreateAnimationFolder("Dummy", "tripleslash", 0.2f, 28, 28, false);
	AuraRenderer_->Off();

	AuraRenderer_->SetFrameCallBack("Aura", 49, std::bind(&PeterTrap::TrapStart, this));
	AuraRenderer_->SetEndCallBack("Aura", [&]() { AuraRenderer_->SetChangeAnimation("Dummy"); });

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Ratwave", "Ratwave", 0.1f, 0, 10, false);
	ImageRenderer_->CreateAnimationFolder("Dummy", "tripleslash", 0.2f, 28, 28, false);
	ImageRenderer_->Off();

	ImageRenderer_->SetFrameCallBack("Ratwave", 5, std::bind(&PeterTrap::AttackColliosinPos, this));
	ImageRenderer_->SetFrameCallBack("Ratwave", 7, std::bind(&PeterTrap::Pase3Fire, this));
	ImageRenderer_->SetFrameCallBack("Ratwave", 9, [&]() { BodyCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f }); });
	ImageRenderer_->SetEndCallBack("Ratwave", [&]() { ImageRenderer_->SetChangeAnimation("Dummy"); });

	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Trigger));
	BodyCollision_->GetTransform()->SetLocalScaling(float4( 160.f, 50.f, 1.f ));
	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void PeterTrap::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	BodyCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&PeterTrap::Damage, this, std::placeholders::_1), CollisionCheck::Begin);

	if (true == Play_)
	{
		TrapPattern(_DeltaTime);		
	}

	if (false == PeterDeath_)
	{
		ButterFlyPattern(_DeltaTime);
	}
}

void PeterTrap::Damage(GameEngineCollision* _Other)
{
	Player* player = GameManager::GetInst().GetPlayer();

	if (DirLeft_)
	{
		player->Hit(500, false);
	}
	else
	{
		player->Hit(500, true);
	}

	BodyCollision_->GetTransform()->SetLocalPosition(float4{ 10000.f, 10000.f, 0.f });
}

void PeterTrap::AttackColliosinPos()
{
	IsFire_ = false;

	if (true == DirLeft_)
	{
		BodyCollision_->GetTransform()->SetLocalPosition(float4(-140.f, 10.f, -100.f));
	}
	else
	{
		BodyCollision_->GetTransform()->SetLocalPosition(float4(140.f, 10.f, -100.f));
	}
}

void PeterTrap::Pase3Fire()
{
	if (true == IsFire_)
	{
		return;
	}

	Sound_->Stop();
	Sound_->PlayOverLap("dragonmine_03_talisman.ogg", 0);

	IsFire_ = true;

	if (false == Pase3_)
	{
		return;
	}

	float4 Pos = BodyCollision_->GetTransform()->GetWorldPosition();

	for (size_t i = 0; i < 3; i++)
	{
		float4 Start = Pos + float4(0.f, -50.f);
		float4 End = float4(1000.f, (i * 1000.f) - 1000.f);

		PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
		Fire->GetTransform()->SetWorldPosition(Start);
		Fire->SetMoveDir(End);
		Fire->SetSpeed(100.f);

		Warning* War = GetLevel()->CreateActor<Warning>();
		War->WarningIine(Start, End, 30);
	}

	for (size_t i = 0; i < 3; i++)
	{
		float4 Start = Pos + float4(0.f, -50.f);
		float4 End = float4(-1000.f, (i * 1000.f) - 1000.f);

		PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
		Fire->GetTransform()->SetWorldPosition(Start);
		Fire->SetMoveDir(End);
		Fire->SetSpeed(100.f);

		Warning* War = GetLevel()->CreateActor<Warning>();
		War->WarningIine(Start, End, 30);
	}
}

void PeterTrap::TrapStart()
{
	if (false == SoundOn_)
	{
		SoundOn_ = true;

		Sound_->Stop();
		Sound_->PlayOverLap("dragonmine_04.ogg", 0);

		ImageRenderer_->SetChangeAnimation("Ratwave");
	}
}

void PeterTrap::TrapPattern(float _DeltaTime)
{
	GetTransform()->SetWorldPositionZ(GetTransform()->GetWorldPosition().y);

	AttackTime_ -= _DeltaTime;

	if (AttackTime_ <= 0.f)
	{
		if (true == Pase3_)
		{
			AttackTime_ = 7.5f;
		}
		else
		{
			AttackTime_ = 7.5f;
		}
		
		SoundOn_ = false;

		AuraRenderer_->SetChangeAnimation("Aura");
		AuraRenderer_->GetTransform()->SetWorldPositionZ(static_cast<float>(ZOrder::GroundEffect));
		IsFire_ = false;

		Player* player = GameManager::GetInst().GetPlayer();
		float4 PlayerXPos = player->BodyCollision_->GetTransform()->GetWorldPosition();

		if (0.f >= PlayerXPos.x)
		{
			GetTransform()->SetWorldPosition(PlayerXPos + float4(150.f, 0.f));

			DirLeft_ = true;
			AuraRenderer_->SetReverse(true);
			ImageRenderer_->SetReverse(true);

			ImageRenderer_->GetTransform()->SetLocalPosition(float4(-80.f, 60.f));
		}
		else
		{
			GetTransform()->SetWorldPosition(PlayerXPos + float4(-150.f, 0.f));

			DirLeft_ = false;
			AuraRenderer_->SetReverse(false);
			ImageRenderer_->SetReverse(false);

			ImageRenderer_->GetTransform()->SetLocalPosition(float4(80.f, 60.f));
		}
	}
}

void PeterTrap::ButterFlyPattern(float _DeltaTime)
{
	ButterFlyTime_ -= _DeltaTime;

	if (ButterFlyTime_ <= 0.f)
	{
		srand((unsigned int)time(NULL));

		if (true == Pase3_)
		{
			if (true == ButterFlyChange_)
			{
				ButterFlyHardModeTime_ -= _DeltaTime;

				if (ButterFlyHardModeTime_ <= 0.f)
				{
					float4 Start;
					float4 End;

					if (ButterFlyCount_ == 0)
					{
						int RandX = ((rand() % 7) - 3) * 100;
						int RandY = ((rand() % 3) + 2) * -50;

						RandPos_.x = static_cast<float>(RandX);
						RandPos_.y = static_cast<float>(RandY);

						Start = float4(RandPos_.x + 600.f * 2.f, RandPos_.y + 300.f * 2.f);
						End = float4(RandPos_.x - 600.f * 2.f, RandPos_.y - 300.f * 2.f);
					}
					else if (ButterFlyCount_ == 1)
					{
						Start = float4(RandPos_.x - 600.f * 2.f, RandPos_.y - 66.6f * 2.f);
						End = float4(RandPos_.x + 600.f * 2.f, RandPos_.y - 66.6f * 2.f);
					}
					else if (ButterFlyCount_ == 2)
					{
						Start = float4(RandPos_.x + 600.f * 2.f, RandPos_.y - 300.f * 2.f);
						End = float4(RandPos_.x - 600.f * 2.f, RandPos_.y + 300.f * 2.f);
					}
					else if (ButterFlyCount_ == 3)
					{
						Start = float4(RandPos_.x - 300.f * 2.f, RandPos_.y + 600.f * 2.f);
						End = float4(RandPos_.x + 200.f * 2.f, RandPos_.y - 600.f * 2.f);
					}
					else if (ButterFlyCount_ == 4)
					{
						Start = float4(RandPos_.x - 200.f * 2.f, RandPos_.y - 600.f * 2.f);
						End = float4(RandPos_.x + 300.f * 2.f, RandPos_.y + 600.f * 2.f);
					}

					Warning* War = GetLevel()->CreateActor<Warning>();
					War->WarningIine(Start, End, 30.f);

					PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
					Fire->GetTransform()->SetWorldPosition(Start);
					Fire->SetMoveVector(End);

					++ButterFlyCount_;
					ButterFlyHardModeTime_ = 0.1f;

					if (ButterFlyCount_ == 5)
					{
						ButterFlyCount_ = 0;
						ButterFlyTime_ = 4.f;
						ButterFlyChange_ = false;
					}
				}
			}
			else
			{
				ButterFlyHardModeTime_ -= _DeltaTime;

				if (ButterFlyHardModeTime_ <= 0.f)
				{
					float4 Start;
					float4 End;

					if (ButterFlyCount_ == 0)
					{
						int RandX = ((rand() % 5) - 2) * 100;

						RandPos_.x = static_cast<float>(RandX);
						RandPos_.y = -125.f;

						Start = float4(RandPos_.x + 600.f * 2.f, RandPos_.y + 50.f * 2.f);
						End = float4(RandPos_.x - 600.f * 2.f, RandPos_.y + 50.f * 2.f);
					}
					else if (ButterFlyCount_ == 1)
					{
						Start = float4(RandPos_.x + 250.f * 2.f, RandPos_.y + 600.f * 2.f);
						End = float4(RandPos_.x - 350.f * 2.f, RandPos_.y - 600.f * 2.f);
					}
					else if (ButterFlyCount_ == 2)
					{
						Start = float4(RandPos_.x - 350.f * 2.f, RandPos_.y + 600.f * 2.f);
						End = float4(RandPos_.x + 250.f * 2.f, RandPos_.y - 600.f * 2.f);
					}
					else if (ButterFlyCount_ == 3)
					{
						Start = float4(RandPos_.x - 600.f * 2.f, RandPos_.y - 50.f * 2.f);
						End = float4(RandPos_.x + 600.f * 2.f, RandPos_.y - 50.f * 2.f);
					}
					else if (ButterFlyCount_ == 4)
					{
						Start = float4(RandPos_.x - 250.f * 2.f, RandPos_.y - 600.f * 2.f);
						End = float4(RandPos_.x + 350.f * 2.f, RandPos_.y + 600.f * 2.f);
					}
					else if (ButterFlyCount_ == 5)
					{
						Start = float4(RandPos_.x + 350.f * 2.f, RandPos_.y - 600.f * 2.f);
						End = float4(RandPos_.x - 250.f * 2.f, RandPos_.y + 600.f * 2.f);
					}

					Warning* War = GetLevel()->CreateActor<Warning>();
					War->WarningIine(Start, End, 30.f);

					PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
					Fire->GetTransform()->SetWorldPosition(Start);
					Fire->SetMoveVector(End);

					++ButterFlyCount_;
					ButterFlyHardModeTime_ = 0.1f;

					if (ButterFlyCount_ == 6)
					{
						ButterFlyCount_ = 0;
						ButterFlyTime_ = 4.f;
						ButterFlyChange_ = true;
					}
				}
			}
		}
		else if (true == Play_)
		{
			if (true == ButterFlyChange_)
			{
				ButterFlyHardModeTime_ -= _DeltaTime;

				if (ButterFlyHardModeTime_ <= 0.f)
				{
					float4 Start;
					float4 End;

					if (ButterFlyCount_ == 0)
					{
						int RandX = ((rand() % 7) - 3) * 100;
						int RandY = ((rand() % 5) + 1) * -50;

						RandPos_.x = static_cast<float>(RandX);
						RandPos_.y = static_cast<float>(RandY);

						Start = float4(-600.f, RandPos_.y - 30.f);
						End = float4(600.f, RandPos_.y - 30.f);
					}
					else if (ButterFlyCount_ == 1)
					{
						Start = float4(RandPos_.x + 300.f, -600.f + RandPos_.y);
						End = float4(RandPos_.x - 200.f, 600.f + RandPos_.y);
					}
					else if (ButterFlyCount_ == 2)
					{
						Start = float4(RandPos_.x + 200.f, 600.f + RandPos_.y);
						End = float4(RandPos_.x - 300.f, -600.f + RandPos_.y);
					}

					Warning* War = GetLevel()->CreateActor<Warning>();
					War->WarningIine(Start, End, 30.f);

					PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
					Fire->GetTransform()->SetWorldPosition(Start);
					Fire->SetMoveVector(End);

					++ButterFlyCount_;
					ButterFlyHardModeTime_ = 0.1f;

					if (ButterFlyCount_ == 3)
					{
						ButterFlyCount_ = 0;
						ButterFlyTime_ = 4.f;
						ButterFlyChange_ = false;
					}
				}
			}
			else
			{
				ButterFlyHardModeTime_ -= _DeltaTime;

				if (ButterFlyHardModeTime_ <= 0.f)
				{
					float4 Start;
					float4 End;

					if (ButterFlyCount_ == 0)
					{
						int RandX = ((rand() % 7) - 3) * 100;
						int RandY = ((rand() % 3) + 2) * -50;

						RandPos_.x = static_cast<float>(RandX);
						RandPos_.y = static_cast<float>(RandY);

						Start = float4(RandPos_.x - 50.f, -600.f);
						End = float4(RandPos_.x - 50.f, 600.f);
					}
					else if (ButterFlyCount_ == 1)
					{
						Start = float4(-600.f, RandPos_.y + 50.f);
						End = float4(600.f, RandPos_.y + 50.f);
					}
					else if (ButterFlyCount_ == 2)
					{
						Start = float4(RandPos_.x + 50.f, 600.f);
						End = float4(RandPos_.x + 50.f, -600.f);
					}
					else if (ButterFlyCount_ == 3)
					{
						Start = float4(600.f, RandPos_.y - 50.f);
						End = float4(-600.f, RandPos_.y - 50.f);
					}

					Warning* War = GetLevel()->CreateActor<Warning>();
					War->WarningIine(Start, End, 30.f);

					PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
					Fire->GetTransform()->SetWorldPosition(Start);
					Fire->SetMoveVector(End);

					++ButterFlyCount_;
					ButterFlyHardModeTime_ = 0.1f;

					if (ButterFlyCount_ == 4)
					{
						ButterFlyCount_ = 0;
						ButterFlyTime_ = 4.f;
						ButterFlyChange_ = true;
					}
				}
			}
		}
		else
		{
			ButterFlyTime_ = 4.f;

			if (true == ButterFlyChange_)
			{
				int RandTop = ((rand() % 9) - 4) * 100;
				int RandBottom = ((rand() % 9) - 4) * 100;
				ButterFlyChange_ = false;

				Warning* War = GetLevel()->CreateActor<Warning>();
				War->WarningIine(float4(static_cast<float>(RandBottom), -400.f), float4(static_cast<float>(RandTop), 200.f), 30.f);

				PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
				Fire->GetTransform()->SetWorldPosition(float4(static_cast<float>(RandBottom), -400.f));
				Fire->SetMoveVector(float4(static_cast<float>(RandTop), 400.f));
			}
			else
			{
				int RandLeft = (rand() % 4) * -100;
				int RandRight = (rand() % 4) * -100;
				ButterFlyChange_ = true;

				Warning* War = GetLevel()->CreateActor<Warning>();
				War->WarningIine(float4(-600.f, static_cast<float>(RandLeft)), float4(600.f, static_cast<float>(RandRight)), 30.f);

				PeterFire* Fire = GetLevel()->CreateActor<PeterFire>();
				Fire->GetTransform()->SetWorldPosition(float4(-600.f, static_cast<float>(RandLeft)));
				Fire->SetMoveVector(float4(600.f, static_cast<float>(RandRight)));
			}
		}
	}
}
