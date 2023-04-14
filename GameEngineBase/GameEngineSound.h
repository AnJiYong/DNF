#pragma once
#include "GameEngineSoundManager.h"
#include "GameEngineObjectNameBase.h"

class GameEngineSoundPlayer;
class GameEngineSound : public GameEngineObjectNameBase
{
	friend GameEngineSoundManager;
	friend GameEngineSoundPlayer;

private:		
	GameEngineSound();
	~GameEngineSound();

public:
	GameEngineSound(const GameEngineSound& _other) = delete;
	GameEngineSound(GameEngineSound&& _other) noexcept;

public:
	GameEngineSound& operator=(const GameEngineSound& _other) = delete;
	GameEngineSound& operator=(const GameEngineSound&& _other) = delete;

public:
	bool Load(const std::string& _Path);

private:
	FMOD::Sound* sound_;
};

