#include "Precompile.h"
#include "MiniMap.h"
#include "GameEngine/GameEngineUIRenderer.h"

MiniMap::MiniMap()
	: MiniMapRenderer_(nullptr)
	, BossMapRenderer_(nullptr)
	, PlayerRenderer_(nullptr)
	, RoomRenderer_()
	, RoomSubRenderer_()
	, MapClear_(false)
	, Check_(false)
	, Time_(0.2f)
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Start()
{
	MiniMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	MiniMapRenderer_->SetImage("MiniMap00.png");
	MiniMapRenderer_->GetTransform()->SetLocalPosition(float4(0.f, 0.f));

	for (size_t y = 0; y < 5; y++)
	{
		for (size_t x = 0; x < 5; x++)
		{
			RoomSubRenderer_[x + (y * 5)] = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			RoomSubRenderer_[x + (y * 5)]->SetImage("MiniMap01.png");
			RoomSubRenderer_[x + (y * 5)]->GetTransform()->SetLocalPosition(float4(-54.f + (x * 27.f), 66.5f - (y * 27.f)));
			RoomSubRenderer_[x + (y * 5)]->SetAlpha(0.5f);
			RoomSubRenderer_[x + (y * 5)]->Off();

			RoomRenderer_[x + (y * 5)] = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			RoomRenderer_[x + (y * 5)]->SetImage("MiniMap01.png");
			RoomRenderer_[x + (y * 5)]->GetTransform()->SetLocalPosition(float4(-54.f + (x * 27.f), 66.5f - (y * 27.f), -1.f));
			RoomRenderer_[x + (y * 5)]->Off();
		}
	}

	BossMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	BossMapRenderer_->SetImage("MiniMap02.png");
	BossMapRenderer_->GetTransform()->SetLocalPosition(float4(54.f, 66.5f));

	PlayerRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	PlayerRenderer_->SetImage("MiniMap03.png");
	PlayerRenderer_->GetTransform()->SetLocalPosition(float4(54.f, 66.5f));
}

void MiniMap::Update(float _DeltaTime)
{
	if (true == MapClear_)
	{
		Time_ -= _DeltaTime;

		if (Time_ <= 0.f)
		{
			for (size_t i = 0; i < 25; i++)
			{
				if (true == RoomSubRenderer_[i]->IsUpdate())
				{
					if (true == Check_)
					{
						RoomSubRenderer_[i]->SetAlpha(0.5f);
					}
					else
					{
						RoomSubRenderer_[i]->SetAlpha(1.f);
					}
				}
			}

			Check_ = !Check_;
			Time_ = 0.2f;
		}
	}
}

void MiniMap::RoomOn(int _Index)
{
	RoomRenderer_[_Index]->On();

	if (_Index - 5 >= 0)
	{
		RoomSubRenderer_[_Index - 5]->On();
	}

	if (_Index + 5 <= 24)
	{
		RoomSubRenderer_[_Index + 5]->On();
	}

	if (_Index - 1 >= 0)
	{
		RoomSubRenderer_[_Index - 1]->On();
	}

	if (_Index + 1 <= 24)
	{
		if (_Index != 4)
		{
			RoomSubRenderer_[_Index + 1]->On();
		}

	}

	if (_Index <= 24)
	{
		PlayerRenderer_->GetTransform()->SetLocalPosition(RoomRenderer_[_Index]->GetTransform()->GetLocalPosition());
	}
}

void MiniMap::SubRoomOff()
{
	for (size_t i = 0; i < 25; i++)
	{
		RoomSubRenderer_[i]->Off();
	}
}
