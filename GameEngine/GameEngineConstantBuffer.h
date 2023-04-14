#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"

class GameEngineConstantBuffer : public GameEngineObjectNameBase
{
public:
	GameEngineConstantBuffer();
	~GameEngineConstantBuffer();

	GameEngineConstantBuffer(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer(GameEngineConstantBuffer&& _Other) noexcept = delete;
	GameEngineConstantBuffer& operator=(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer& operator=(GameEngineConstantBuffer&& _Other) noexcept = delete;

public:
	void ChangeData(void* _Data, size_t _Size);

	inline unsigned int GetBufferSize() 
	{
		return BufferData_.ByteWidth;
	}

	inline ID3D11Buffer*& GetBuffer()
	{
		return Buffer_;
	}

	void Create(const D3D11_SHADER_BUFFER_DESC& _BufferDesc , ID3D11ShaderReflectionConstantBuffer* _VarInfo);

private:
	ID3D11Buffer* Buffer_;
	D3D11_BUFFER_DESC BufferData_;
	D3D11_MAPPED_SUBRESOURCE ResData_;
	D3D11_SHADER_BUFFER_DESC BufferDesc_;

	std::vector<D3D11_SHADER_VARIABLE_DESC> VarInfos_;
};

