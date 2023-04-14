#pragma once
#include "GameEngineShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

enum class SettingMode
{
	Link,
	Set,
	MAX,
};

class GameEngineShader;
class GameEngineConstantBuffer;
class GameEngineConstantBufferSetting
{
public:
	GameEngineConstantBufferSetting()
		: SettingData_(nullptr)
		, Mode_(SettingMode::MAX)
		, Res_()
		, SettingDataSize_()
		, SettingIndex_()
		, Shader()
	{
	}

	~GameEngineConstantBufferSetting()
	{
		Clear();
	}

public:
	void Clear() 
	{
		if (SettingMode::Set == Mode_)
		{
			if (nullptr != SettingData_)
			{
				delete SettingData_;
				SettingData_ = nullptr;
			}
		}
	}

	void ChangeData() 
	{
		Res_->ChangeData(SettingData_, SettingDataSize_);
	}

	void ShaderSetting()
	{
		Shader->SetConstantBuffers(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetConstantBuffers(this);
	}

public:
	GameEngineShader* Shader;
	GameEngineConstantBuffer* Res_;
	SettingMode Mode_;
	char* SettingData_;
	size_t SettingDataSize_;
	int SettingIndex_;
};

class GameEngineShader;
class GameEngineSampler;
class GameEngineSamplerSetting
{
public:
	GameEngineSamplerSetting()
		: Res_(nullptr)
		, Shader()
		, SettingIndex_()
	{
	}

	~GameEngineSamplerSetting()
	{
	}

public:
	void ShaderSetting()
	{
		Shader->SetSampler(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetSampler(this);
	}

public:
	GameEngineShader* Shader;
	GameEngineSampler* Res_;
	int SettingIndex_;
};

class GameEngineShader;
class GameEngineTexture;
class GameEngineTextureSetting
{
public:
	GameEngineTextureSetting()
		: Res_(nullptr)
		, SettingIndex_()
		, Shader()
	{
	}

	~GameEngineTextureSetting()
	{
	}

public:
	void ShaderSetting()
	{
		Shader->SetTexture(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetTexture(this);
	}

public:
	GameEngineShader* Shader;
	GameEngineTexture* Res_;
	int SettingIndex_;
};