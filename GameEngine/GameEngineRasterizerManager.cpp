#include "PreCompile.h"
#include "GameEngineRasterizerManager.h"
#include "GameEngineRasterizer.h"

GameEngineRasterizerManager* GameEngineRasterizerManager::Inst = new GameEngineRasterizerManager();

GameEngineRasterizerManager::GameEngineRasterizerManager()
{
}

GameEngineRasterizerManager::~GameEngineRasterizerManager()
{
	for (const std::pair<std::string, GameEngineRasterizer*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineRasterizerManager::GameEngineRasterizerManager(GameEngineRasterizerManager&& _other) noexcept
{
}

GameEngineRasterizer* GameEngineRasterizerManager::Create(const std::string& _Name, const D3D11_RASTERIZER_DESC& _RasterizerDesc)
{
	GameEngineRasterizer* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineRasterizer* NewRes = new GameEngineRasterizer();
	NewRes->SetName(_Name);
	NewRes->Create(_RasterizerDesc);

	ResourcesMap.insert(std::map<std::string, GameEngineRasterizer*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineRasterizer* GameEngineRasterizerManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineRasterizer* GameEngineRasterizerManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineRasterizer* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineRasterizer* NewRes = new GameEngineRasterizer();
	NewRes->SetName(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineRasterizer*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineRasterizer* GameEngineRasterizerManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineRasterizer*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}