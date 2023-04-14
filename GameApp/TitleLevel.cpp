#include "PreCompile.h"
#include "TitleLevel.h"
#include "Title.h"
#include "GameEngine/GameEngineCore.h"
#include "UserGame.h"
#include "DungeonLevel.h"
#include "TownLevel.h"
#include "TestLevel.h"

TitleLevel::TitleLevel()
	: ResourcesLoad_(false)
	, LoadEnd_(false)
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCamera()->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -100.f));

	Title* Actor = CreateActor<Title>();
	Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, -90.f));
}

void TitleLevel::LevelUpdate(float _DeltaTime)
{
	if (true == LoadEnd_)
	{
		GameEngineCore::LevelChange("01.Town");
		return;
	}

	if (false == ResourcesLoad_)
	{
		ResourcesLoad();
		ResourcesLoad_ = true;
	}

	if (GameEngineCore::ThreadQueue.GetWorkingCount() > 0)
	{
		Sleep(1);
	}
	else
	{
		LevelLoad();
		LoadEnd_ = true;
	}
}

void TitleLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}
void TitleLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void TitleLevel::ResourcesLoad()
{
	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Avatar");
			TextureDir.MoveChild("Avatar00");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);
		
	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Avatar");
			TextureDir.MoveChild("Avatar01");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Avatar");
			TextureDir.MoveChild("Avatar02");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("etc");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Skill");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Monster");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Npc");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue.JobPost([]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("AJY");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("Weapon");

			std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();

			for (size_t i = 1; i < AllTextureDir.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllTextureDir[i].GetFullPath());
			}
		}
	);
}

void TitleLevel::LevelLoad()
{
	GameEngineCore::LevelCreate<TownLevel>("01.Town");
	GameEngineCore::LevelCreate<DungeonLevel>("02.Dungeon");
}
