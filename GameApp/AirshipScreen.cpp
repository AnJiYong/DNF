#include "Precompile.h"
#include "AirshipScreen.h"
#include "AirshipScreenUI.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "GlobalEnum.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

AirshipScreen::AirshipScreen()
	: ImageRenderer_()
	, MainImageRenderer_()
	, MainCollision_()
	, MainUI_()
	, MainUIOn(false)
	, Sound_(nullptr)	
{
}

AirshipScreen::~AirshipScreen()
{
}

void AirshipScreen::Start()
{
	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("AirshipScreen", "AirshipScreen", 0.1f);
	ImageRenderer_->SetChangeAnimation("AirshipScreen");

	for (size_t y = 0; y < 3; y++)
	{
		for (size_t x = 0; x < 3; x++)
		{
			if (x == 1 && y == 1)
			{
				OutlineRenderer_[x + (y * 3)] = nullptr;
				continue;
			}

			OutlineRenderer_[x + (y * 3)] = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
			OutlineRenderer_[x + (y * 3)]->CreateAnimationFolder("AirshipScreen", "AirshipMain", 0.1f);
			OutlineRenderer_[x + (y * 3)]->SetChangeAnimation("AirshipScreen");
			OutlineRenderer_[x + (y * 3)]->SetRenderingPipeLine("TextureTrans");
			OutlineRenderer_[x + (y * 3)]->SetColor(float4::GREEN);
			OutlineRenderer_[x + (y * 3)]->SetPlusColor(float4::GREEN);
			OutlineRenderer_[x + (y * 3)]->GetTransform()->SetLocalPosition(float4((x - 1.f) -18.f, (y - 1.f) + 13.f));
		}
	}

	MainImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	MainImageRenderer_->CreateAnimationFolder("AirshipScreen", "AirshipMain", 0.1f);
	MainImageRenderer_->SetChangeAnimation("AirshipScreen");
	MainImageRenderer_->GetTransform()->SetLocalPosition(float4( -18.f, 13.f ));

	MainCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::NPC));
	MainCollision_->GetTransform()->SetLocalScaling(float4{ 265.f, 300.f, 1.f });
	MainCollision_->GetTransform()->SetLocalPosition(float4{ -20.f, -60.f, -100.f });

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void AirshipScreen::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(MainCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	for (size_t i = 0; i < 9; i++)
	{
		if (nullptr != OutlineRenderer_[i])
		{
			OutlineRenderer_[i]->Off();
		}
	}

	if (false == MainUIOn)
	{
		MainCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
			, std::bind(&AirshipScreen::UIOnPlayer, this, std::placeholders::_1));

		MainCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::MouseSub)
			, std::bind(&AirshipScreen::UIOnMouse, this, std::placeholders::_1));
	}
	else
	{
		if (true == GameEngineInput::GetInst().Down("Esc"))
		{
			MainUI_->Death();
			MainUIOn = false;
		}
	}
}

void AirshipScreen::UIOnPlayer(GameEngineCollision* _Other)
{
	if (true == GameEngineInput::GetInst().Down("Space"))
	{		
		MainUI_ = GetLevel()->CreateActor<AirshipScreenUI>();
		MainUI_->GetTransform()->SetWorldPosition(float4(0.f, 0.f, -100.f));
		MainUIOn = true;
	}
}

void AirshipScreen::UIOnMouse(GameEngineCollision* _Other)
{
	for (size_t i = 0; i < 9; i++)
	{
		if (nullptr != OutlineRenderer_[i])
		{
			OutlineRenderer_[i]->On();
		}
	}

	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		Sound_->Stop();
		Sound_->PlayOverLap("air_ui_click.ogg", 0);

		MainUI_ = GetLevel()->CreateActor<AirshipScreenUI>();
		MainUI_->GetTransform()->SetWorldPosition(float4(0.f, 0.f, -100.f));
		MainUIOn = true;
	}
}