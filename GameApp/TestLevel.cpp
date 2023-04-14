#include "Precompile.h"
#include "TestLevel.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/PostFade.h"

#include "Mouse.h"
#include "Player.h"
#include "GameManager.h"
#include "TopUI.h"
#include "Map.h"
#include "Inventory.h"
#include "AvatarInven.h"
#include "TestMap.h"
#include "TestBossMap.h"
#include "UserGame.h"

#include "PeterTrap.h"
#include "IsangomaNazaloh.h"
#include "NazalohSummon.h"
#include "NazalohBoom.h"

TestLevel::TestLevel()
	: CurPlayer_(nullptr)
	, CurMap_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::CreateActorLevel()
{
	CurPlayer_ = CreateActor<Player>(-3);
	CurPlayer_->GetTransform()->SetWorldPosition(float4(0.f, -50.f, 0.f));
	CurPlayer_->SetIsDungeon(true);
	GetMainCameraActor()->GetTransform()->SetWorldPosition(CurPlayer_->GetTransform()->GetLocalPosition());
	GetMainCameraActor()->SetTargetTransform(CurPlayer_->GetTransform());
	GameManager::GetInst().SetPlayer(CurPlayer_);

	{
		Mouse* Actor = CreateActor<Mouse>(2);
	}

	{
		Map* Actor = CreateActor<Map>();
		Actor->GetImageRenderer()->SetImage("DungeonMap.png");
		Actor->GetImageRenderer()->SetAlpha(0.f);
		Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 0.f));
	}

	{
		TestMap* Actor = CreateActor<TestMap>();
		Actor->CreateGate(false, false, false, false);
		Actor->SetMap("Dungeon6");
	}

	{
		TopUI* Actor = CreateActor<TopUI>();
		Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 10.f));
		GameManager::GetInst().SetUI(Actor);
	}
}

void TestLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -100.f));

	GetMainCameraActor()->SetMapPivot(float4(1000.f, 600.f), float4(0.f, 0.f));

	{
		IsangomaNazaloh* Peter = CreateActor< IsangomaNazaloh>();
		Peter->GetTransform()->SetWorldPosition(float4(-60.f, -60.f));
	}
	FadeEffect_ = AddPostProcessCameraMergeNext<PostFade>();
	FadeEffect_->SetTarget(GameEngineDevice::GetBackBufferTarget());
}

void TestLevel::LevelUpdate(float _DeltaTime)
{
	static bool CreateActorCheck = false;

	if (0 >= UserGame::LoadingFolder
		&& false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = false;

	GameManager::GetInst().Update();

}

void TestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void TestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}

void TestLevel::MapChange()
{
}
