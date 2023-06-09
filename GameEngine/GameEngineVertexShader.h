#pragma once
#include <vector>
#include "GameEngineBase/GameEngineMath.h"
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineShader.h"
#include "GameEngineDevice.h"

class GameEngineVertexShader : public GameEngineShader
{
public:
	GameEngineVertexShader();
	~GameEngineVertexShader();

protected:
	GameEngineVertexShader(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader(GameEngineVertexShader&& _other) noexcept = delete;

private:
	GameEngineVertexShader& operator=(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader& operator=(const GameEngineVertexShader&& _other) = delete;

private:
	void AddInputLayOut(const char* _SemanticName, unsigned int _Index, unsigned int _AlignedByteOffset, 
		DXGI_FORMAT _Format, unsigned int _InputSlot, unsigned int _InstanceDataStepRate, D3D11_INPUT_CLASSIFICATION _inputClass);

	void CreateLayOut();

	void LayOutCheck();

	void LayOutClear();

public:
	bool Create(const std::string& _ShaderCode,	const std::string& _EntryPoint,	UINT _VersionHigh = 5, UINT _VersionLow = 0);

	bool Load(const std::string& _Path,	const std::string& _EntryPoint,	UINT _VersionHigh = 5, UINT _VersionLow = 0);

	bool StringCompile();

	bool FileCompile(const std::string& _Path);

	void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;

	void SetTexture(const GameEngineTextureSetting* _Setting) override;

	void SetSampler(const GameEngineSamplerSetting* _Setting) override;

	void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;

	void ReSetTexture(const GameEngineTextureSetting* _Setting) override;

	void ReSetSampler(const GameEngineSamplerSetting* _Setting) override;

	void InputLayOutSetting();
	void Setting();

private:
	ID3D11VertexShader* Shader_;

	ID3D11InputLayout* LayOut_;
	unsigned int LayOutOffset_;
	std::vector<std::string> SemanticName_;
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc_;
};

