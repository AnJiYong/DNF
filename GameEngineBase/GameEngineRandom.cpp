#include "PreCompile.h"
#include "GameEngineRandom.h"

GameEngineRandom::GameEngineRandom(GameEngineRandom&& _other) noexcept
	:mt_(_other.mt_)
{
}

