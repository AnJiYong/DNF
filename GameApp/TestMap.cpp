#include "Precompile.h"
#include "TestMap.h"
#include "GameEngine/GameEngineImageRenderer.h"

#include "NomalMonster.h"
#include "TestAirMonster.h"

TestMap::TestMap()
	: BackRenderer_(nullptr)
	, FrontRenderer_(nullptr)
{
}

TestMap::~TestMap()
{
}

void TestMap::Start()
{
	MapBase::Start();
	BackRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	BackRenderer_->SetImage("Dungeon1.png");
	BackRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, static_cast<float>(ZOrder::BackGroundDown)));

	FrontRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	FrontRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f, static_cast<float>(ZOrder::BackGroundUp)));
}

void TestMap::Update(float _DeltaTime)
{
	MapBase::Update(_DeltaTime);
}

void TestMap::SetMap(const std::string& _Name)
{
	std::string Name = _Name;
	Name.append(".png");
	BackRenderer_->SetImage(Name);

	Name = _Name;
	Name.append("f");
	Name.append(".png");
	FrontRenderer_->SetImage(Name);
}

void TestMap::MapStart(int _MapCount)
{
	if (true == Clear_)
	{
		return;
	}

	switch (_MapCount)
	{
	case 0:
		for (size_t i = 0; i < 3; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(200.f - (200.f * i), -150.f, 0.f));
			Monster->SetMonsterType(MonsterType::MeltKnight);
			Monster->RoomSetting(this);
		}

		for (size_t i = 0; i < 2; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(100.f - (200.f * i), -100.f, 0.f));
			Monster->SetMonsterType(MonsterType::Gackle);
			Monster->RoomSetting(this);
		}

		for (size_t i = 0; i < 2; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(100.f - (200.f * i), -200.f, 0.f));
			Monster->SetMonsterType(MonsterType::Gackle);
			Monster->RoomSetting(this);
		}
		break;
	case 1:
		for (size_t i = 0; i < 3; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(200.f - (200.f * i), -100.f, 0.f));
			Monster->SetMonsterType(MonsterType::MeltKnight);
			Monster->RoomSetting(this);
		}

		for (size_t i = 0; i < 3; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(200.f - (200.f * i), -200.f, 0.f));
			Monster->SetMonsterType(MonsterType::MeltKnight);
			Monster->RoomSetting(this);
		}

		for (size_t i = 0; i < 2; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(100.f - (200.f * i), -150.f, 0.f));
			Monster->SetMonsterType(MonsterType::Infectee);
			Monster->RoomSetting(this);
		}
		break;
	case 2:
		for (size_t i = 0; i < 2; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(250.f - (500.f * i), -100.f, 0.f));
			Monster->SetMonsterType(MonsterType::Infectee);
			Monster->RoomSetting(this);
		}

		for (size_t i = 0; i < 2; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(250.f - (500.f * i), -200.f, 0.f));
			Monster->SetMonsterType(MonsterType::Infectee);
			Monster->RoomSetting(this);
		}

		for (size_t i = 0; i < 2; i++)
		{
			NomalMonster* Monster = GetLevel()->CreateActor<NomalMonster>(-1);
			Monster->GetTransform()->SetLocalPosition(float4(250.f - (500.f * i), -150.f, 0.f));
			Monster->SetMonsterType(MonsterType::Gackle);
			Monster->RoomSetting(this);
		}
		break;
	}
}
