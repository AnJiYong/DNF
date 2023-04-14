#pragma once
#include "FMOD/fmod.hpp"
#include "GameEngineSound.h"
#include "GameEngineSoundManager.h"

class GameEngineSoundPlayer
{
	friend GameEngineSoundManager;

private:		
	GameEngineSoundPlayer();
	~GameEngineSoundPlayer();

public:
	GameEngineSoundPlayer(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer(GameEngineSoundPlayer&& _other) noexcept;

public:
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer&& _other) = delete;

public:
	bool IsPlay();
	void PlayCountReset(int _Count = -1);
	void PlayOverLap(const std::string& _name, int _LoopCount = 1);
	void PlayAlone(const std::string& _name, int _LoopCount = 1);
	void Stop();
	void SetVolume(float _Volume);

private:
	GameEngineSoundManager* playSoundFile_;
	FMOD::Channel* playChannel_;
	int PlayCount;
};

