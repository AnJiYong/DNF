#include "Precompile.h"
#include "AirshipScreenUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCore.h"

AirshipScreenUI::AirshipScreenUI()
	: TopRenderer_(nullptr)
	, MapRenderer_(nullptr)
	, SelectMapRenderer_(nullptr)
	, HexagonRenderer_(nullptr)
	, AniEnd_(false)
	, MapNumber_(1)
{
}

AirshipScreenUI::~AirshipScreenUI()
{
}

void AirshipScreenUI::Start()
{
	MapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MapRenderer_->SetImage("AirshipScreenUI0.png");
	MapRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 1.f));

	TopRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TopRenderer_->SetImage("AirshipScreenUI01.png");
	TopRenderer_->GetTransform()->SetLocalPosition(float4(0.f, -100.f, 0.f));
}

void AirshipScreenUI::Update(float _Time)
{
	if (true == AniEnd_)
	{
		if (true == GameEngineInput::GetInst().Press("Space"))
		{
			Death();
			GameEngineCore::LevelChange("02.Dungeon");
		}

		if (true == GameEngineInput::GetInst().Down("MoveLeft"))
		{
			HexagonRenderer_->GetCurrentAnimation()->Reset();

			--MapNumber_;

			if (MapNumber_ == -1)
			{
				MapNumber_ = 2;
			}
		}

		if (true == GameEngineInput::GetInst().Down("MoveRight"))
		{
			HexagonRenderer_->GetCurrentAnimation()->Reset();

			++MapNumber_;

			if (MapNumber_ == 3)
			{
				MapNumber_ = 0;
			}
		}

		switch (MapNumber_)
		{
		case 0:
			SelectMapRenderer_->GetTransform()->SetLocalPosition(float4(-150.f, 0.f, -1.f));
			HexagonRenderer_->GetTransform()->SetLocalPosition(float4(-150.f, 0.f, 1.f));
			break;
		case 1:
			SelectMapRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -1.f));
			HexagonRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 1.f));
			break;
		case 2:
			SelectMapRenderer_->GetTransform()->SetLocalPosition(float4(150.f, 0.f, -1.f));
			HexagonRenderer_->GetTransform()->SetLocalPosition(float4(150.f, 0.f, 1.f));
			break;
		default:
			break;
		}
	}
	else
	{
		TopRenderer_->GetTransform()->SetLocalDeltaTimeMove(float4::UP * 200.f);

		if (0 <= TopRenderer_->GetTransform()->GetLocalPosition().y)
		{
			AnimationEnd();
		}
	}
}

void AirshipScreenUI::AnimationEnd()
{
	AniEnd_ = true;

	SelectMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	SelectMapRenderer_->SetRenderGroup(static_cast<int>(RenderingOrder::UI));
	SelectMapRenderer_->SetImage("AirshipScreenUI1.png");
	SelectMapRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));
		
	HexagonRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	HexagonRenderer_->CreateAnimationFolder("Hexagon", "Hexagon", 0.1f, true);
	HexagonRenderer_->SetChangeAnimation("Hexagon");
	HexagonRenderer_->SetRenderingPipeLine("TextureTrans");
	HexagonRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 1.f));	

	{
		GameEngineUIRenderer* Image = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Image->SetImage("AirshipScreenUI2.png");
		Image->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));
	}

	{
		GameEngineUIRenderer* Image = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Image->SetRenderGroup(static_cast<int>(RenderingOrder::UI));
		Image->SetImage("AirshipScreenUI2.png");
		Image->GetTransform()->SetLocalPosition(float4(-150.f, 0.f, 0.f));
	}

	{
		GameEngineUIRenderer* Image = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Image->SetRenderGroup(static_cast<int>(RenderingOrder::UI));
		Image->SetImage("AirshipScreenUI2.png");
		Image->GetTransform()->SetLocalPosition(float4(150.f, 0.f, 0.f));
	}
}
