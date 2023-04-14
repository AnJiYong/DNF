#include "Precompile.h"
#include "SeriaKirmin.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "GlobalEnum.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "NpcEvent.h"

SeriaKirmin::SeriaKirmin()
	: ImageRenderer_()
	, ImageCollision_()
	, Sound_(nullptr)
	, TextRenderer_(nullptr)
	, OutlineRenderer_()
	, Event_(nullptr)
{
}

SeriaKirmin::~SeriaKirmin()
{
}

void SeriaKirmin::Start()
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
			OutlineRenderer_[x + (y * 3)]->CreateAnimationFolder("Seria", "Seria", 0.2f);
			OutlineRenderer_[x + (y * 3)]->SetChangeAnimation("Seria");
			OutlineRenderer_[x + (y * 3)]->SetRenderingPipeLine("TextureTrans");
			OutlineRenderer_[x + (y * 3)]->SetColor(float4::GREEN);
			OutlineRenderer_[x + (y * 3)]->SetPlusColor(float4::GREEN);
			OutlineRenderer_[x + (y * 3)]->GetTransform()->SetLocalPosition(float4((x - 1.f), (y - 1.f)));
		}
	}

	ImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ImageRenderer_->CreateAnimationFolder("Seria", "Seria", 0.2f);
	ImageRenderer_->SetChangeAnimation("Seria");

	ImageCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::NPC));
	ImageCollision_->GetTransform()->SetLocalScaling(float4{ 70.f, 140.f, 1.f });
	ImageCollision_->GetTransform()->SetLocalPosition(float4{ 0.f, 0.f });

	TextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TextRenderer_->SetImage("Dummy.png");
	TextRenderer_->TextSetting("gulim", "¼¼¸®¾Æ", 11, float4(227.f / 255.f, 208.f / 255.f, 139.f / 255.f));
	TextRenderer_->SetOutLine(float4::BLACK);

	Event_ = GetLevel()->CreateActor<NpcEvent>();
	Event_->SetNpcName("Seria");
	Event_->Off();

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();
}

void SeriaKirmin::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(ImageCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	TextRenderer_->GetTransform()->SetWorldPosition(ImageCollision_->GetTransform()->GetWorldPosition() -
		GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition() + float4(0.f, 70.f, 0.f));

	for (size_t i = 0; i < 9; i++)
	{
		if (nullptr != OutlineRenderer_[i])
		{
			OutlineRenderer_[i]->Off();
		}
	}

	ImageCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::MouseSub)
		, std::bind(&SeriaKirmin::OutLine, this, std::placeholders::_1));
}

void SeriaKirmin::OutLine(GameEngineCollision* _Other)
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
		Sound_->PlayOverLap("seria_amb_01.ogg", 0);

		Event_->On();
		Event_->GetTransform()->SetLocalPosition(_Other->GetTransform()->GetWorldPosition()
		- GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition() + float4(50.f, -10.f, 0.f));
	}
}
