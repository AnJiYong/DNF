#pragma once

class GameEnginePixelShader;
class GameEnginePixelShaderManager
{
private:
	static GameEnginePixelShaderManager* Inst;

public:
	static GameEnginePixelShaderManager& GetInst()
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
	GameEnginePixelShaderManager(); 
	~GameEnginePixelShaderManager();

protected:		
	GameEnginePixelShaderManager(const GameEnginePixelShaderManager& _other) = delete; 
	GameEnginePixelShaderManager(GameEnginePixelShaderManager&& _other) noexcept; 

private:
	GameEnginePixelShaderManager& operator=(const GameEnginePixelShaderManager& _other) = delete; 
	GameEnginePixelShaderManager& operator=(const GameEnginePixelShaderManager&& _other) = delete;

public:
	GameEnginePixelShader* Create(const std::string& _Name,	const std::string& _ShaderCode,	UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		return Create(_Name, _ShaderCode, _Name, _VersionHigh, _VersionLow);
	}

	GameEnginePixelShader* Create(const std::string& _Name,	const std::string& _ShaderCode,	const std::string& _EntryPoint,	UINT _VersionHigh = 5, UINT _VersionLow = 0);
	GameEnginePixelShader* Load(const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	GameEnginePixelShader* Load(const std::string& _Name, const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	GameEnginePixelShader* Find(const std::string& _Name);

private:
	std::map<std::string, GameEnginePixelShader*> ResourcesMap;
};




