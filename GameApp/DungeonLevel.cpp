#include "Precompile.h"
#include "DungeonLevel.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/PostFade.h"

#include "Mouse.h"
#include "Player.h"
#include "GameManager.h"
#include "TopUI.h"
#include "Map.h"
#include "Inventory.h"
#include "AvatarInven.h"
#include "TestMap.h"
#include "TestBossMap.h"
#include "MiniMap.h"

DungeonLevel::DungeonLevel()
	: CurPlayer_(nullptr)
	, CurMap_(nullptr)
	, DungeonNumber_(12)
	, FadeOn_(false)
	, FadeTime_(0.3f)
	, MapPivot_(float4::ZERO)
	, PlayerPos_(float4::ZERO)
	, IsBossRoom_(false)
	, MiniMap_(nullptr)
	, BossRoomIn_(true)
{
}

DungeonLevel::~DungeonLevel()
{
}

void DungeonLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -100.f));

	{
		Map* Actor = CreateActor<Map>();
		Actor->GetImageRenderer()->SetImage("DungeonMap.png");
		Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 1000.f));
	}

	FadeEffect_ = AddPostProcessCameraMergeNext<PostFade>();
	FadeEffect_->SetTarget(GameEngineDevice::GetBackBufferTarget());

	GetMainCameraActor()->SetMapPivot(float4(1000.f, 600.f), float4(0.f, 0.f));
}

void DungeonLevel::LevelUpdate(float _DeltaTime)
{
	GameManager::GetInst().Update();

	if (true == CurMap_->MapClear())
	{
		CurMap_->GateOpen();
		MiniMap_->MapClear(true);		
	}

	if (true == FadeOn_)
	{
		FadeTime_ -= _DeltaTime;

		if (FadeTime_ <= 0.f)
		{
			FadeTime_ = 0.3f;
			FadeOn_ = false;
			PlayerPosChange();
		}
	}
}

void DungeonLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	GameManager::GetInst().LevelChangeEndEvent();
}

void DungeonLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	CurPlayer_ = CreateActor<Player>(-3);
	CurPlayer_->GetTransform()->SetWorldPosition(float4(-200.f, -50.f, 0.f));
	CurPlayer_->SetIsDungeon(true);
	GetMainCameraActor()->GetTransform()->SetWorldPosition(CurPlayer_->GetTransform()->GetLocalPosition());
	GetMainCameraActor()->SetTargetTransform(CurPlayer_->GetTransform());
	GameManager::GetInst().SetPlayer(CurPlayer_);

	{
		Mouse* MouseActor = CreateActor<Mouse>(2);

		TopUI* UI = CreateActor<TopUI>();
		UI->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 1000.f));
		UI->TownSet(false);

		DungeonNumber_ = 12;
		MiniMap_ = UI->GetMiniMap();
		MiniMap_->RoomOn(DungeonNumber_);

		Inventory* Inven = CreateActor<Inventory>();
		Inven->GetTransform()->SetWorldPosition(float4(25000.f, 275.f, -10.f));

		AvatarInven* Avatar = CreateActor<AvatarInven>();
		Avatar->GetTransform()->SetWorldPosition(float4(-25000.f, 275.f, -10.f));

		if (nullptr == GameManager::GetInst().GetInven())
		{
			Inven->FirstInit();
			Avatar->FirstInit();
		}
		else
		{
			UI->SetLeftSlot();
			Inven->SetItemSlot();
			Avatar->SetItemSlot();
		}

		GameManager::GetInst().SetUI(UI);
		GameManager::GetInst().SetMouse(MouseActor);
		GameManager::GetInst().SetInven(Inven);
		GameManager::GetInst().SetAvatarInven(Avatar);
	}

	{
		for (size_t y = 0; y < 5; y++)
		{
			for (size_t x = 0; x < 5; x++)
			{
				TestMap* Actor = CreateActor<TestMap>();
				Actor->GetTransform()->SetWorldPosition(float4(-2000.f + (x * 1000.f), 2000.f - (y * 1000.f)));

				if (x == 0 && y == 0)
				{
					Actor->CreateGate(false, true, false, true);
					Actor->SetMap("Dungeon5");
				}
				else if (x == 0 && y == 4)
				{
					Actor->CreateGate(false, true, true, false);
					Actor->SetMap("Dungeon5");
				}
				else if (x == 4 && y == 0)
				{
					Actor->CreateGate(true, false, false, true);
					Actor->SetMap("Dungeon5");
				}
				else if (x == 4 && y == 4)
				{
					Actor->CreateGate(true, false, true, false);
					Actor->SetMap("Dungeon5");
				}
				else if (x == 0)
				{
					Actor->CreateGate(false, true, true, true);
					Actor->SetMap("Dungeon4");
				}
				else if (x == 4)
				{
					Actor->CreateGate(true, false, true, true);
					Actor->SetMap("Dungeon4");
				}
				else if (y == 0)
				{
					Actor->CreateGate(true, true, false, true);
					Actor->SetMap("Dungeon2");
				}
				else if (y == 4)
				{
					Actor->CreateGate(true, true, true, false);
					Actor->SetMap("Dungeon2");
				}
				else
				{
					Actor->CreateGate(true, true, true, true);
				}

				if (x == 2 && y == 2)
				{
					Actor->SetMap("Dungeon3");
				}

				MapList_.push_back(Actor);
			}
		}

		CurMap_ = MapList_[DungeonNumber_];
		CurMap_->Clear();
	}

	//{
	//	TestBossMap* Actor = CreateActor<TestBossMap>();
	//	Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 0.f));
	//	Actor->GetTransform()->SetWorldPosition(float4(2000.f, 2000.f, 1000.f));
	//	MapList_.push_back(Actor);
	//	BossMapList_.push_back(Actor);
	//}

	//{
	//	TestBossMap* Actor = CreateActor<TestBossMap>();
	//	Actor->GetTransform()->SetWorldPosition(float4(0.f, 0.f, 0.f));
	//	Actor->GetTransform()->SetWorldPosition(float4(1000.f, 0.f, 1000.f));
	//	MapList_.push_back(Actor);
	//	BossMapList_.push_back(Actor);
	//	MapList_[13]->CreateBossGate();
	//}
	MapList_[4]->CreateBossGate(BossType::PeterThePiper);
	MapList_[13]->CreateBossGate(BossType::IsangomaNazaloh);
	GameManager::GetInst().ChangeSound("x");
	//GameManager::GetInst().ChangeSound("leshphon.ogg");
}

void DungeonLevel::MapChange(GateType _GateType)
{
	MiniMap_->SubRoomOff();

	FadeEffect_->SetData(0.3f, FadeOption::DARK);
	FadeOn_ = true;

	float4 MapPivot = float4::ZERO;
	float4 PlayerPos = float4::ZERO;

	switch (_GateType)
	{
	case GateType::Left:
		MapPivot.x = 1000.f;
		PlayerPos = float4(445.f, -85.f);
		--DungeonNumber_;
		break;
	case GateType::Right:
		MapPivot.x = -1000.f;
		PlayerPos = float4(-455.f, -85.f);
		++DungeonNumber_;
		break;
	case GateType::Up:
		MapPivot.y = -1000.f;
		PlayerPos = float4(-200.f, -200.f);
		DungeonNumber_ -= 5;
		break;
	case GateType::Down:
		MapPivot.y = 1000.f;
		PlayerPos = float4(200.f, -40.f);
		DungeonNumber_ += 5;
		break;
	case GateType::BossIn:
		MapPivot.z = 1000.f;
		PlayerPos = float4(-455.f, -85.f);
		IsBossRoom_ = true;
		BossRoomIn_ = true;
		break;
	case GateType::BossOut:
		MapPivot.z = -1000.f;
		PlayerPos = float4(-455.f, -85.f);
		IsBossRoom_ = false;
		MapList_[DungeonNumber_]->GateOpen();
		break;
	}

	MapPivot_ = MapPivot;
	PlayerPos_ = PlayerPos;
}

void DungeonLevel::FadeOut(float _Time)
{
	CurMap_->FadeOut(_Time);
}

void DungeonLevel::PlayerPosChange()
{
	std::vector<class MapBase*>::iterator iter = MapList_.begin();
	std::vector<class MapBase*>::iterator iterEnd = MapList_.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->GetTransform()->SetWorldPositionMove(MapPivot_);
	}

	CurPlayer_->GetTransform()->SetWorldPosition(PlayerPos_);
	CurMap_->Clear();

	if (true == IsBossRoom_)
	{
		CurMap_->GateClose();
		CreateBossRoom(CurMap_->GetBossType());
	}
	else
	{
		if (true == BossRoomIn_)
		{
			BossRoomIn_ = false;

			CurMap_->GetTransform()->SetWorldPositionZ(-1000.f);
			CurMap_->Off();
		}

		CurMap_ = MapList_[DungeonNumber_];
		MapList_[DungeonNumber_]->MapStart(DungeonNumber_ % 3);
	}

	MiniMap_->RoomOn(DungeonNumber_);
	MiniMap_->MapClear(false);
}

void DungeonLevel::CreateBossRoom(BossType _Type)
{
	TestBossMap* Actor = CreateActor<TestBossMap>();
	Actor->GetTransform()->SetWorldPosition(float4::ZERO);
	Actor->SetBossType(_Type);
	Actor->MapStart();

	CurMap_ = Actor;
}
