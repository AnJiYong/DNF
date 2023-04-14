#include "Precompile.h"
#include "LiaRichter.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "GlobalEnum.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCore.h"
#include "NpcEvent.h"

LiaRichter::LiaRichter()
	: ImageRenderer_()
	, ImageCollision_()
	, Sound_(nullptr)
	, DungeonStartBackRenderer_(nullptr)
	, TextRenderer_(nullptr)
	, OutlineRenderer_()
	, DungeonStartCollision_(nullptr)
	, Event_(nullptr)
{
}

LiaRichter::~LiaRichter()
{
}

void LiaRichter::Start()
{
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
			OutlineRenderer_[x + (y * 3)]->CreateAnimationFolder("LiaRichter", "LiaRichter", 0.2f);
			OutlineRenderer_[x + (y * 3)]->SetChangeAnimation("LiaRichter");
			OutlineRenderer_[x + (y * 3)]->SetRenderingPipeLine("TextureTrans");
			OutlineRenderer_[x + (y * 3)]->SetReverse(true);
			OutlineRenderer_[x + (y * 3)]->SetColor(float4::GREEN);
			OutlineRenderer_[x + (y * 3)]->SetPlusColor(float4::GREEN);
			OutlineRenderer_[x + (y * 3)]->GetTransform()->SetLocalPosition(float4((x - 1.f), (y - 1.f)));
		}
	}

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("LiaRichter", "LiaRichter", 0.2f);
	ImageRenderer_->SetChangeAnimation("LiaRichter");
	ImageRenderer_->SetReverse(true);

	ImageCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::NPC));
	ImageCollision_->GetTransform()->SetLocalScaling(float4{ 90.f, 140.f, 1.f });

	DungeonStartBackRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	DungeonStartBackRenderer_->GetTransform()->SetLocalPosition(float4(21.f, 105.f, 0.f));
	DungeonStartBackRenderer_->SetRenderingPipeLine("TextureTrans");	
	DungeonStartBackRenderer_->SetImage("UI_DungeonStart0.png");
	DungeonStartBackRenderer_->GetTransform()->SetLocalScaling(float4(120.f, 120.f, 1.f));

	TextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TextRenderer_->SetImage("UI_DungeonStart.png");
	TextRenderer_->TextSetting("gulim", "함장 리아 리히터", 11, float4(227.f / 255.f, 208.f / 255.f, 139.f / 255.f), float4(0.f, -28.f));
	TextRenderer_->SetOutLine(float4::BLACK);

	DungeonStartCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	DungeonStartCollision_->GetTransform()->SetLocalPosition(float4(20.f, 105.f, 0.f));
	DungeonStartCollision_->GetTransform()->SetLocalScaling(float4{ 60.f, 60.f, 1.f });

	Event_ = GetLevel()->CreateActor<NpcEvent>();
	Event_->SetNpcName("Lia");
	Event_->Off();

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void LiaRichter::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(ImageCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushDebugRender(DungeonStartCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	DungeonStartBackRenderer_->Off();

	TextRenderer_->GetTransform()->SetWorldPosition(ImageCollision_->GetTransform()->GetWorldPosition() -
		GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition() + float4(20.f, 105.f, 0.f));

	for (size_t i = 0; i < 9; i++)
	{
		if (nullptr != OutlineRenderer_[i])
		{
			OutlineRenderer_[i]->Off();
		}
	}

	ImageCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::MouseSub)
		, std::bind(&LiaRichter::OutLine, this, std::placeholders::_1));

	DungeonStartCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::MouseSub)
		, std::bind(&LiaRichter::DungeonStartMouseOn, this, std::placeholders::_1));
}

void LiaRichter::OutLine(GameEngineCollision* _Other)
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
		Sound_->PlayOverLap("ria_amb_01.ogg", 0);

		Event_->On();
		Event_->GetTransform()->SetLocalPosition(_Other->GetTransform()->GetWorldPosition()
			- GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition() + float4(50.f, -10.f, 0.f));
	}
}

void LiaRichter::DungeonStartMouseOn(GameEngineCollision* _Other)
{
	DungeonStartBackRenderer_->On();

	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		GameEngineCore::LevelChange("02.Dungeon");
	}
}
