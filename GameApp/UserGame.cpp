#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include "GameEngineBase/GameEngineTime.h"
#include "GameEngine/GameEngineWindow.h"
#include "GameEngine/GameEngineRenderingPipeLine.h"

#include "GameManager.h"
#include "TitleLevel.h"
#include "TownLevel.h"
#include "DungeonLevel.h"
#include "TestLevel.h"

std::atomic<int> UserGame::LoadingFolder = 0;

UserGame::UserGame()
{
}

UserGame::~UserGame() 
{
}

UserGame::UserGame(UserGame&& _other) noexcept
{
}

void UserGame::Initialize()
{
	GameManager::GetInst().Init();
	
#ifdef _DEBUG
	LevelCreate<TownLevel>("01.Town");
	LevelCreate<DungeonLevel>("02.Dungeon");
	//LevelCreate<TestLevel>("99.Test");
	LevelChange("02.Dungeon");
#else
	LevelCreate<TitleLevel>("00.Title");
	LevelChange("00.Title");
#endif // _DEBUG

	return;
}

void UserGame::Release()
{
	GameManager::GetInst().Destroy();
}