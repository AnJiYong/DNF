#include "PreCompile.h"
#include "TownLevel.h"
#include "Player.h"
#include "TopUI.h"
#include "Map.h"
#include "SeriaRoom.h"
#include "Pillasia.h"
#include "Mouse.h"
#include "GameManager.h"
#include "UserGame.h"
#include "GameEngine/CameraComponent.h"
#include "GameEngine/GameEngineTransform.h"
#include "GameEngine/CameraActor.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "Inventory.h"
#include "AvatarInven.h"

TownLevel::TownLevel() 
	: Mouse_(nullptr)
	, CurPlayer_(nullptr)
	, StartCheck_(false)
{
}

TownLevel::~TownLevel() 
{
}

void TownLevel::CreateActorLevel()
{
	CurPlayer_ = CreateActor<Player>(-3);
	CurPlayer_->GetTransform()->SetWorldPosition(float4(0.f, -150.f, 0.f));
	CurPlayer_->SetIsDungeon(false);
	GetMainCameraActor()->GetTransform()->SetWorldPosition(CurPlayer_->GetTransform()->GetLocalPosition());
	GetMainCameraActor()->SetTargetTransform(CurPlayer_->GetTransform());
	GameManager::GetInst().SetPlayer(CurPlayer_);

	{
		Map* Actor = CreateActor<Map>();
		Actor->GetImageRenderer()->SetImage("TownMap.png");
		Actor->GetTransform()->SetWorldPosition(float4(0.f, -300.f, 10.f));
	}

	{
		SeriaRoom* Actor = CreateActor<SeriaRoom>();
		Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 0.f));
	}

	{
		Pillasia* Actor = CreateActor<Pillasia>();
		Actor->GetTransform()->SetWorldPosition(float4(0.f, -600.f, 0.f));
	}

	{ 
		Mouse* MouseActor = CreateActor<Mouse>(2);
		
		TopUI* UI = CreateActor<TopUI>();
		UI->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 100.f));
		UI->TownSet(true);

		Inventory* Inven = CreateActor<Inventory>();
		Inven->GetTransform()->SetWorldPosition(float4(25000.f, 275.f, -10.f));

		AvatarInven* Avatar = CreateActor<AvatarInven>();
		Avatar->GetTransform()->SetWorldPosition(float4(-25000.f, 275.f, -10.f));

		if (nullptr == GameManager::GetInst().GetInven())
		{
			Inven->FirstInit();
			Avatar->FirstInit();
		}
		else
		{
			UI->SetLeftSlot();
			Inven->SetItemSlot();
			Avatar->SetItemSlot();
		}

		GameManager::GetInst().SetUI(UI);
		GameManager::GetInst().SetMouse(MouseActor);
		GameManager::GetInst().SetInven(Inven);
		GameManager::GetInst().SetAvatarInven(Avatar);
	}

	GameManager::GetInst().ChangeSound("seria_gate.ogg");
}

void TownLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCamera()->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -100.f));


	GetMainCameraActor()->SetMapPivot(float4(954.f, 600.f), float4(0.f, 0.f));
}

void TownLevel::LevelUpdate(float _DeltaTime)
{
	// static GameEngineRenderWindow* Window = nullptr;

	// GameEngineGUI::GetInst()->CreateGUIWindow"GameEngineLevelControlWindow>("LevelControlWindow");

	static bool CreateActorCheck = false;

	if (false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = true;

	GameManager::GetInst().Update();
}

void TownLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	GameManager::GetInst().LevelChangeEndEvent();
}

void TownLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	if (true == StartCheck_)
	{
		GameManager::GetInst().ChangeSound("phillasia.ogg");
		Teleport();
	}

	StartCheck_ = true;
}

void TownLevel::Teleport()
{
	GameManager::GetInst().ChangeSound("phillasia.ogg");

	GetMainCameraActor()->SetMapPivot(float4(954.f, 1800.f), float4(0.f, -600.f));
	CurPlayer_->GetTransform()->SetWorldPosition(float4(0.f, -750.f, 0.f));
}
