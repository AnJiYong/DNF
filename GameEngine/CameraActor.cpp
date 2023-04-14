#include "PreCompile.h"
#include "CameraActor.h"
#include "CameraComponent.h"
#include "GameEngineTransform.h"
#include "GameEngineInput.h"

CameraActor::CameraActor() 
	: Camera_(nullptr)
	, Target_(nullptr)
	, MapPivot_(float4(10000.f, 10000.f))
    , IsFreeCameraMode_(false)
    , FreeCameraSpeed_(200.f)
    , RotSpeed_(360.f)
	, MoveLock_(false)
	, CameraEvent_(false)
	, ZoomValue_(1.f)
	, DeltaTime_(0.f)
	, CameraHolding_(false)
	, HoldingPos_(float4::ZERO)
	, PrevCamMode_(ProjectionMode::Orthographic)
{
}

CameraActor::~CameraActor() 
{
}

void CameraActor::Start() 
{
	Camera_ = CreateTransformComponent<CameraComponent>(GetTransform());

    PrevCamMode_ = Camera_->ProjectionMode_;

    if (false == GameEngineInput::GetInst().IsKey("CAMMoveFoward"))
    {
        GameEngineInput::GetInst().CreateKey("CAMMoveFoward", 'W');
        GameEngineInput::GetInst().CreateKey("CAMMoveBack", 'S');
        GameEngineInput::GetInst().CreateKey("CAMMoveUp", 'Q');
        GameEngineInput::GetInst().CreateKey("CAMMoveDown", 'E');
        GameEngineInput::GetInst().CreateKey("CAMMoveLeft", 'A');
        GameEngineInput::GetInst().CreateKey("CAMMoveRight", 'D');
        GameEngineInput::GetInst().CreateKey("CAMSpeedBoost", VK_LSHIFT);
        GameEngineInput::GetInst().CreateKey("CAMRot", VK_RBUTTON);
    }
}

float4x4 CameraActor::GetViewMatrix()
{
	return Camera_->GetTransform()->GetTransformData().View_;
}

float4x4 CameraActor::GetProjectionMatrix() 
{
	return Camera_->GetTransform()->GetTransformData().Projection_;
}

void CameraActor::CameraMoveEvent(float4 _Vector)
{
	MoveLock_ = true;
	CameraEvent_ = true;

	if (ZoomValue_ >= 0.7f)
	{
		ZoomValue_ -= DeltaTime_;
	}
	Camera_->CameraZoomSetting(ZoomValue_);

	double x = GetTransform()->GetWorldPosition().x;
	double y = GetTransform()->GetWorldPosition().y;
	double distance = sqrt(pow(x - _Vector.x, 2) + pow(y - _Vector.y, 2));

	if (static_cast<float>(distance) <= 20.f)
	{
		GetTransform()->SetWorldPosition(_Vector);
		return;
	}

	float4 Pos;
	Pos.x = _Vector.x - GetTransform()->GetWorldPosition().x;
	Pos.y = _Vector.y - GetTransform()->GetWorldPosition().y;
	Pos.Normalize3D();

	GetTransform()->SetWorldDeltaTimeMove(Pos * 300.f);
}

void CameraActor::CameraHolding(float4 _Vector, float _ZoomValue)
{
	MoveLock_ = true;
	CameraEvent_ = true;
	CameraHolding_ = true;
	Camera_->CameraZoomSetting(_ZoomValue);
	HoldingPos_ = _Vector;
}

void CameraActor::CameraReset()
{
	if (false == CameraEvent_)
	{
		return;
	}

	MoveLock_ = false;
	CameraEvent_ = false;
	CameraHolding_ = false;
	Camera_->CameraZoomSetting(1.f);
}

bool CameraActor::IsFreeCameraMode()
{
	return IsFreeCameraMode_;
}

void CameraActor::FreeCameraModeSwitch()
{
	IsFreeCameraMode_ = !IsFreeCameraMode_;

	switch (IsFreeCameraMode_)
	{
	case true:
		MoveLock_ = true;
		FreeCameraModeOn();
		break;
	case false:
		MoveLock_ = false;
		FreeCameraModeOff();
		break;
	}
}

void CameraActor::FreeCameraModeOn()
{
	IsFreeCameraMode_ = true;

	PrevCamMode_ = Camera_->ProjectionMode_;
	Camera_->SetProjectionMode(ProjectionMode::Perspective);

	OriginTransform.Copy(*GetTransform());
}

void CameraActor::FreeCameraModeOff()
{
	IsFreeCameraMode_ = false;
	Camera_->SetProjectionMode(PrevCamMode_);
	GetTransform()->Copy(OriginTransform);
}

void CameraActor::Update(float _DeltaTime) 
{
	if (true == CameraHolding_)
	{
		GetTransform()->SetWorldPosition(HoldingPos_);
		return;
	}

	DeltaTime_ = _DeltaTime;

    if (false == MoveLock_ && nullptr != Target_)
    {
        float4 Pos = Target_->GetWorldPosition();

        if (Pos.x > MapStartPos_.x + MapPivot_.x)
        {
            Pos.x = MapStartPos_.x + MapPivot_.x;
        }

        if (Pos.x < MapStartPos_.x - MapPivot_.x)
        {
            Pos.x = MapStartPos_.x - MapPivot_.x;
        }

        if (Pos.y > MapStartPos_.y + MapPivot_.y)
        {
            Pos.y = MapStartPos_.y + MapPivot_.y;
        }

        if (Pos.y < MapStartPos_.y - MapPivot_.y)
        {
            Pos.y = MapStartPos_.y - MapPivot_.y;
        }

        GetTransform()->SetWorldPosition(Pos);
    }

	if (false == IsFreeCameraMode_)
	{
		return;
	}

	float MoveSpeed = FreeCameraSpeed_;

	if (GameEngineInput::GetInst().Press("CAMSpeedBoost"))
	{
		MoveSpeed *= 3.f;
	}

	if (GameEngineInput::GetInst().Press("CAMMoveFoward"))
	{
		GetTransform()->SetWorldMove(GetTransform()->GetWorldForwardVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::GetInst().Press("CAMMoveBack"))
	{
		GetTransform()->SetWorldMove(GetTransform()->GetWorldBackVector() * _DeltaTime * MoveSpeed);

	}
	if (GameEngineInput::GetInst().Press("CAMMoveUp"))
	{
		GetTransform()->SetWorldMove(GetTransform()->GetWorldUpVector() * _DeltaTime * MoveSpeed);

	}
	if (GameEngineInput::GetInst().Press("CAMMoveDown"))
	{
		GetTransform()->SetWorldMove(GetTransform()->GetWorldDownVector() * _DeltaTime * MoveSpeed);

	}
	if (GameEngineInput::GetInst().Press("CAMMoveLeft"))
	{
		GetTransform()->SetWorldMove(GetTransform()->GetWorldLeftVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::GetInst().Press("CAMMoveRight"))
	{
		GetTransform()->SetWorldMove(GetTransform()->GetWorldRightVector() * _DeltaTime * MoveSpeed);
	}

	if (GameEngineInput::GetInst().Press("CAMRot"))
	{
		GetTransform()->AddWorldRotationDegreeY(GameEngineInput::GetMouse3DDir().x * _DeltaTime * RotSpeed_);
		GetTransform()->AddWorldRotationDegreeX(-GameEngineInput::GetMouse3DDir().y * _DeltaTime * RotSpeed_);
	}
}