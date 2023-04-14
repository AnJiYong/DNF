#pragma once
#include "GameEngine/GameEngineCore.h"

class UserGame : public GameEngineCore
{
public:
	UserGame();
	~UserGame();

protected:
	UserGame(const UserGame& _other) = delete;
	UserGame(UserGame&& _other) noexcept;

private:		
	UserGame& operator=(const UserGame& _other) = delete;
	UserGame& operator=(const UserGame&& _other) = delete;

public:
	virtual void Initialize() override;
	virtual void ResourcesLoad() override;
	virtual void Release() override;

	virtual float4 StartWindowPos() {
		return { 0, 0 };
	}
	virtual float4 StartWindowSize() {
		return { 800, 600 };
	}

public:
	static std::atomic<int> LoadingFolder;
};

