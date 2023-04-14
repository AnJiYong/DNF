#pragma once

class GameEngineRenderingPipeLine;
class GameEngineRenderingPipeLineManager
{
private:
	static GameEngineRenderingPipeLineManager* Inst;

public:
	static GameEngineRenderingPipeLineManager& GetInst()
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
	GameEngineRenderingPipeLineManager(); 
	~GameEngineRenderingPipeLineManager();

protected:
	GameEngineRenderingPipeLineManager(const GameEngineRenderingPipeLineManager& _other) = delete;
	GameEngineRenderingPipeLineManager(GameEngineRenderingPipeLineManager&& _other) noexcept;

private:
	GameEngineRenderingPipeLineManager& operator=(const GameEngineRenderingPipeLineManager& _other) = delete;
	GameEngineRenderingPipeLineManager& operator=(const GameEngineRenderingPipeLineManager&& _other) = delete;

public:
	GameEngineRenderingPipeLine* Create(const std::string& _Name);
	GameEngineRenderingPipeLine* Load(const std::string& _Path);
	GameEngineRenderingPipeLine* Load(const std::string& _Name, const std::string& _Path);
	GameEngineRenderingPipeLine* Find(const std::string& _Name);

private:
	std::map<std::string, GameEngineRenderingPipeLine*> ResourcesMap;
};




