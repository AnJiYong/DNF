#include "Precompile.h"
#include "TestBossMap.h"
#include "GameEngine/GameEngineImageRenderer.h"

#include "PeterThePiper.h"
#include "IsangomaNazaloh.h"

TestBossMap::TestBossMap()
	: ImageRenderer_()
	, Type_(BossType::None)
{
}

TestBossMap::~TestBossMap()
{
}

void TestBossMap::Start()
{
	MapBase::Start();

	Map1Renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	Map1Renderer_->SetImage("Dungeon6.png");
	Map1Renderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, static_cast<float>(ZOrder::BackGroundDown)));
	
	//Map2Renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	//Map2Renderer_->SetImage("Dungeon6f.png");
	//Map2Renderer_->GetTransform()->SetLocalPosition(float4(0.f, 30.f, static_cast<float>(ZOrder::BackGroundUp)));
}

void TestBossMap::Update(float _DeltaTime)
{
	MapBase::Update(_DeltaTime);
}

void TestBossMap::MapStart(int _MapCount)
{
	if (Type_ == BossType::PeterThePiper)
	{
		PeterThePiper* NomalMonster = GetLevel()->CreateActor<PeterThePiper>();
		NomalMonster->GetTransform()->AttachTransform(GetTransform());
		NomalMonster->GetTransform()->SetLocalPosition(float4(200.f, -150.f, 0.f));
		NomalMonster->RoomSetting(this, true);
	}
	else if(Type_ == BossType::IsangomaNazaloh)
	{
		IsangomaNazaloh* NomalMonster = GetLevel()->CreateActor<IsangomaNazaloh>();
		NomalMonster->GetTransform()->AttachTransform(GetTransform());
		NomalMonster->GetTransform()->SetLocalPosition(float4(200.f, -150.f, 0.f));
		NomalMonster->RoomSetting(this, true);
	}
}

void TestBossMap::MapColorReverse(bool _On)
{
	if (true == _On)
	{
		Map1Renderer_->SetPlusColor(float4::WHITE);
		//Map2Renderer_->SetPlusColor(float4::WHITE);
	}
	else
	{
		Map1Renderer_->SetPlusColor(float4::NONE);
		//Map2Renderer_->SetPlusColor(float4::NONE);
	}
}
