#pragma once
#include "GameEngineBase/GameEngineMath.h"
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngine/GameEngineDevice.h"

class GameEngineVertexBuffer : public GameEngineObjectNameBase
{
public:
	GameEngineVertexBuffer();
	~GameEngineVertexBuffer();

protected:
	GameEngineVertexBuffer(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _other) noexcept;

private:
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer&& _other) = delete;

public:
	template<typename VertexType>
	void Create(const std::vector<VertexType>& _Datas, D3D11_USAGE _Usage)
	{
		Create(reinterpret_cast<const void*>(&_Datas[0]), sizeof(VertexType), _Datas.size(), _Usage);
	}

	void Create(const void* _Data, size_t _Datas, size_t _Count, D3D11_USAGE _Usage);

	void Setting();

private:
	ID3D11Buffer* Buffer_;
	UINT Size_;
	UINT Count_;
	UINT Offset_;

	D3D11_BUFFER_DESC BufferData_;
	D3D11_SUBRESOURCE_DATA ResData_;
};

