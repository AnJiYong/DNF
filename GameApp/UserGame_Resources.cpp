#include "PreCompile.h"
#include "UserGame.h"
#include "GameEngine\EngineVertex.h"
#include "UserGame_Resources_Shader.h"
#include "GameEngine\GameEngineFontManager.h"

void TextureLoading(GameEngineDirectory Dir)
{
	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		// Sleep(10);
	}

	--UserGame::LoadingFolder;
}

void FolderTextureLoading(GameEngineDirectory Dir)
{
	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	GameEngineFolderTextureManager::GetInst().Load(Dir.GetFullPath());

	--UserGame::LoadingFolder;
}

void UserGame::ResourcesLoad()
{
	GameEngineFontManager::GetInst().Load("±Ã¼­");
	GameEngineFontManager::GetInst().Load("UttumBatangBold");
	GameEngineFontManager::GetInst().Load("GasinamuNew");
	GameEngineFontManager::GetInst().Load("batang");
	GameEngineFontManager::GetInst().Load("Gasinamu");
	GameEngineFontManager::GetInst().Load("gulim");

	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("AJY");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Image");
		TextureDir.MoveChild("Image");
		std::vector<GameEngineDirectory> AllTextureDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder += static_cast<int>(AllTextureDir.size());
		for (size_t i = 0; i < AllTextureDir.size(); i++)
		{
			TextureLoading(AllTextureDir[i]);
		}
	}

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("AJY");
		SoundDir.MoveChild("Resources");
		SoundDir.MoveChild("Sound");

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}
}
