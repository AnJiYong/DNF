#include "PreCompile.h"
#include "GameEngineObjectNameBase.h"

GameEngineObjectNameBase::GameEngineObjectNameBase()
	: name_("")
{
}

GameEngineObjectNameBase::~GameEngineObjectNameBase() 
{
}

GameEngineObjectNameBase::GameEngineObjectNameBase(const GameEngineObjectNameBase& _other)
	: name_(_other.name_)
{
}

GameEngineObjectNameBase::GameEngineObjectNameBase(GameEngineObjectNameBase&& _other) noexcept
	: name_(_other.name_)
{

}
