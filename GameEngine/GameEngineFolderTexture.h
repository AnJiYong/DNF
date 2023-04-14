#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

class GameEngineTexture;
class GameEngineFolderTexture : public GameEngineObjectNameBase
{
public:
	GameEngineFolderTexture();
	~GameEngineFolderTexture();

private:
	GameEngineFolderTexture(const GameEngineFolderTexture& _other) = delete;
	GameEngineFolderTexture(GameEngineFolderTexture&& _other) noexcept = delete;
	GameEngineFolderTexture& operator=(const GameEngineFolderTexture& _other) = delete;
	GameEngineFolderTexture& operator=(const GameEngineFolderTexture&& _other) = delete;

public:
	void Load(const std::string& _Path);

	GameEngineTexture* GetTextureIndex(int _Index);

	inline int GetTextureCount() 
	{
		return static_cast<int>(AllTexture_.size());
	}

private:
	std::vector<GameEngineTexture*> AllTexture_;
};

