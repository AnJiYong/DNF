#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"
#include "ThirdParty/Include/FW1Font/FW1FontWrapper.h"

#pragma comment(lib, "FW1FontWrapper.lib")

class GameEngineFontManager;
class GameEngineFont : public GameEngineObjectNameBase
{
	friend GameEngineFontManager;

public:
	GameEngineFont();
	~GameEngineFont();

	GameEngineFont(const GameEngineFont& _Other) = delete;
	GameEngineFont(GameEngineFont&& _Other) noexcept = delete;
	GameEngineFont& operator=(const GameEngineFont& _Other) = delete;
	GameEngineFont& operator=(GameEngineFont&& _Other) noexcept = delete;

protected:
	void Load(const std::string& _Path);

public:
	void DrawFont(const std::string& _Text, float _Size, const float4& _Pos, const float4& _Color, unsigned int _Pivot = FW1_CENTER);

private:
	static IFW1Factory* FontFactory_;
	IFW1FontWrapper* FontWrapper_;
};

