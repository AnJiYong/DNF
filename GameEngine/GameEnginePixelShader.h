#pragma once
#include <vector>
#include "GameEngineBase/GameEngineMath.h"
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"
#include "GameEngineShader.h"

class GameEngineTextureSetting;
class GameEnginePixelShader : public GameEngineShader
{
public:
	GameEnginePixelShader();
	~GameEnginePixelShader();

	GameEnginePixelShader(const GameEnginePixelShader& _Other) = delete;
	GameEnginePixelShader(GameEnginePixelShader&& _Other) noexcept = delete;
	GameEnginePixelShader& operator=(const GameEnginePixelShader& _Other) = delete;
	GameEnginePixelShader& operator=(GameEnginePixelShader&& _Other) noexcept = delete;

private:
	void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;

	void SetTexture(const GameEngineTextureSetting* _Setting) override;

	void SetSampler(const GameEngineSamplerSetting* _Setting) override;

	void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void ReSetTexture(const GameEngineTextureSetting* _Setting) override;
	void ReSetSampler(const GameEngineSamplerSetting* _Setting) override;

public:
	bool Create(const std::string& _ShaderCode, const std::string& _EntryPoint,	UINT _VersionHigh = 5, UINT _VersionLow = 0);

	bool Load(const std::string& _Path,	const std::string& _EntryPoint,	UINT _VersionHigh = 5, UINT _VersionLow = 0);

	bool StringCompile();

	bool FileCompile(const std::string& _Path);

	void Setting();

private:
	ID3D11PixelShader* Shader_;
};

