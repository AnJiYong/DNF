#include "Precompile.h"
#include "Mouse.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineCollision.h"
#include "GlobalEnum.h"

Mouse::Mouse()
	: Mouse_()
	, MainCollision_()
	, SubCollision_()
	, PrevPos_(float4::ZERO)
	, HideTime_(0.f)
	, CatchItemType_(ItemType::None)
	, MouseChange_(false)
{
}

Mouse::~Mouse()
{
}

void Mouse::Start()
{
	GameEngineInput::GetInst().HideCursor();

	Mouse_ = CreateTransformComponent<GameEngineUIRenderer>();
	Mouse_->SetImage("Cursor2.png");
	Mouse_->SetRenderGroup(static_cast<int>(RenderingOrder::Mouse));

	MainCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Mouse));
	MainCollision_->GetTransform()->SetLocalScaling(float4(1.f, 1.f, 1.f));
	MainCollision_->GetTransform()->SetLocalPosition(float4(-15.f, 15.f));

	SubCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::MouseSub));
	SubCollision_->GetTransform()->SetLocalScaling(float4(1.f, 1.f, 1.f));
	SubCollision_->GetTransform()->SetLocalPosition(float4(-15.f, 15.f));
}

void Mouse::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushDebugRender(SubCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());

	if (false == MouseChange_)
	{
		if (PrevPos_ != GameEngineInput::GetInst().GetMouse3DPos())
		{
			if (false == Mouse_->IsUpdate())
			{
				Mouse_->On();
			}

			PrevPos_ = GameEngineInput::GetInst().GetMouse3DPos();
			HideTime_ = 0.f;
		}
		else
		{
			HideTime_ += _DeltaTime;

			if (HideTime_ >= 2.f)
			{
				Mouse_->Off();
			}
		}
	}
	
	SubCollision_->GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos()
		+ GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition() + float4(-15.f, 15.f));

	if (true == GameEngineInput::GetInst().Press("MouseLButton"))
	{
		if (true == MouseChange_)
		{
			Mouse_->SetImage("Cursor2.png");
			Mouse_->GetTransform()->SetWorldRotationDegree(float4(0.f, 0.f, 50.f));
		}
		else
		{
			Mouse_->SetImage("Cursor1.png");
		}
	}
	else
	{
		if (true == MouseChange_)
		{
			Mouse_->SetImage("Cursor2.png");
			Mouse_->GetTransform()->SetWorldRotationDegree(float4::ZERO);
		}
		else
		{
			Mouse_->SetImage("Cursor0.png");
		}
	}
}

void Mouse::WindowCursorOn()
{
	ShowCursor(true);
}

void Mouse::WindowCursorOff()
{
	ShowCursor(false);
}

void Mouse::MouseChange(bool _Value)
{
	MouseChange_ = _Value;
	Mouse_->On();

	if (true == MouseChange_)
	{
		Mouse_->GetTransform()->SetLocalPosition(float4(0.f, 20.f));
	}
	else
	{
		Mouse_->GetTransform()->SetLocalPosition(float4::ZERO);
		Mouse_->GetTransform()->SetWorldRotationDegree(float4::ZERO);
	}
}
