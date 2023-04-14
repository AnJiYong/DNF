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
	
	LevelCreate<TitleLevel>("00.Title");
	LevelChange("00.Title");

	return;
}

void UserGame::Release()
{
	GameManager::GetInst().Destroy();
}