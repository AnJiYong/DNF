#pragma once
#include "GameEngine/GameEngineDevice.h"

class GameEngineRasterizer;
class GameEngineRasterizerManager
{
private:
	static GameEngineRasterizerManager* Inst;

public:
	static GameEngineRasterizerManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	GameEngineRasterizerManager();
	~GameEngineRasterizerManager();

protected:
	GameEngineRasterizerManager(const GameEngineRasterizerManager& _other) = delete;
	GameEngineRasterizerManager(GameEngineRasterizerManager&& _other) noexcept;

private:
	GameEngineRasterizerManager& operator=(const GameEngineRasterizerManager& _other) = delete;
	GameEngineRasterizerManager& operator=(const GameEngineRasterizerManager&& _other) = delete;

public:
	GameEngineRasterizer* Create(const std::string& _Name, const D3D11_RASTERIZER_DESC& _RasterizerDesc);
	GameEngineRasterizer* Load(const std::string& _Path);
	GameEngineRasterizer* Load(const std::string& _Name, const std::string& _Path);
	GameEngineRasterizer* Find(const std::string& _Name);

private:
	std::map<std::string, GameEngineRasterizer*> ResourcesMap;
};




