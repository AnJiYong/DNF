#include "PreCompile.h"
#include "GameEngine/GameEngineImageRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "Player.h"
#include "Map.h"

Map* Map::CurrentMap = nullptr;

Map::Map()
{
	RandomStart = float4::ZERO;
}

Map::~Map()
{
}

void Map::Start()
{
	CurrentMap = this;
	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>();
}

void Map::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	CurrentMap = this;
}

float4 Map::GetColor(GameEngineTransform* _Ptr, bool YRevers /*= true*/)
{
	float PosX = CurrentMap->ImageRenderer->GetCurrentTexture()->GetTextureSize().x;
	float4 Pos = _Ptr->GetWorldPosition() + float4(PosX * 0.5f, -600.f * 0.5f) + float4(0.f, 0.f);

	if (true == YRevers)
	{
		Pos.y *= -1.f;
	}

	return GetColor(Pos);
}

float4 Map::GetColor(float4 _Postion)
{
	return CurrentMap->ImageRenderer->GetCurrentTexture()->GetPixel(_Postion.ix(), _Postion.iy());
}