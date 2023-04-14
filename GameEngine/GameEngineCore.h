#pragma once
#include "GameEngineLevel.h"
#include "GameEngineBase\GameEngineThreadQueue.h"

class GameEngineCore : public GameEngineObjectBase
{
	friend class GameEngineLevelControlWindow;

protected:
	GameEngineCore();
	~GameEngineCore();

protected:
	GameEngineCore(const GameEngineCore& _other) = delete;
	GameEngineCore(GameEngineCore&& _other) noexcept;

private:
	GameEngineCore& operator=(const GameEngineCore& _other) = delete;
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete;

private:
	static void WindowCreate(GameEngineCore& _RuntimeCore);
	static void Loop();
	static void MainLoop();

protected:
	virtual void Initialize() = 0;
	virtual void ResourcesLoad() = 0;
	virtual void Release() = 0;
	virtual float4 StartWindowSize() = 0;
	virtual float4 StartWindowPos() = 0;

public:
	template<typename LevelType>
	static void LevelCreate(const std::string& _Level)
	{
		if (nullptr != LevelFind(_Level))
		{
			GameEngineDebug::MsgBoxError("같은 이름의 레벨을 2번 만들려고 했습니다");
			return;
		}

		AllLevel_.insert(std::make_pair(_Level, new LevelType()));
		AllLevel_[_Level]->SetName(_Level);
		AllLevel_[_Level]->Init();
		AllLevel_[_Level]->LevelStart();
	}

	static void LevelDestroy(const std::string& _Level);

	static GameEngineLevel* CurrentLevel() 
	{
		return CurrentLevel_;
	}

	static void LevelChange(const std::string& _Level);
	static GameEngineLevel* LevelFind(const std::string& _Level);

	template<typename UserGameType>
	static void Start()
	{
		GameEngineDebug::LeakCheckOn();
		UserGameType NewUserGame;

		WindowCreate(NewUserGame);

		NewUserGame.EngineInitialize();
		NewUserGame.ResourcesLoad();
		NewUserGame.Initialize();

		MainCore_ = &NewUserGame;

		Loop();

		NewUserGame.Release();
		NewUserGame.EngineDestroy();
	}

	void EngineInitialize();
	void EngineResourcesLoad();
	void EngineResourcesCreate();
	void EngineDestroy();

private:
	static GameEngineLevel* NextLevel_;
	static GameEngineLevel* CurrentLevel_;
	static std::map<std::string, GameEngineLevel*> AllLevel_;

	static GameEngineCore* MainCore_;

public:
	static GameEngineThreadQueue ThreadQueue;
};

