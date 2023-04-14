#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"

class GameEngineMesh : public GameEngineObjectNameBase
{
public:
	GameEngineMesh();
	~GameEngineMesh();

protected:
	GameEngineMesh(const GameEngineMesh& _other) = delete;
	GameEngineMesh(GameEngineMesh&& _other) noexcept;

private:
	GameEngineMesh& operator=(const GameEngineMesh& _other) = delete;
	GameEngineMesh& operator=(const GameEngineMesh&& _other) = delete;

private:
	GameEngineVertexBuffer* VertexBuffer_;
	GameEngineIndexBuffer* IndexBuffer_;
};

