#pragma once
#include "GameEngineActor.h"
#include "CameraComponent.h"

class CameraComponent;
class CameraActor : public GameEngineActor
{
	friend class GameEngineLevel;

public:
	CameraActor();
	~CameraActor();

	CameraActor(const CameraActor& _Other) = delete;
	CameraActor(CameraActor&& _Other) noexcept = delete;
	CameraActor& operator=(const CameraActor& _Other) = delete;
	CameraActor& operator=(CameraActor&& _Other) noexcept = delete;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

public:
	CameraComponent* GetCamera() 
	{
		return Camera_;
	}

	void SetTargetTransform(GameEngineTransform* _Target)
	{
		Target_ = _Target;
	}

	void SetMapPivot(float4 _MapSize, float4 _MapStartPos)
	{
		MapSize_ = _MapSize;
		MapStartPos_ = _MapStartPos;

		MapPivot_.x = MapSize_.x * 0.5f - 400.f + _MapStartPos.x;
		MapPivot_.y = MapSize_.y * 0.5f - 300.f + _MapStartPos.y;
	}

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

	void CameraMoveEvent(float4 _Vector);
	void CameraHolding(float4 _Vector, float _ZoomValue);
	void CameraReset();

	bool IsFreeCameraMode();

	void FreeCameraModeSwitch();
	void FreeCameraModeOn();
	void FreeCameraModeOff();

private:
	GameEngineTransform OriginTransform;
	float FreeCameraSpeed_;
	float RotSpeed_;
	bool IsFreeCameraMode_;
	ProjectionMode PrevCamMode_;
	CameraComponent* Camera_;
	GameEngineTransform* Target_;

	float4 MapPivot_;
	float4 MapStartPos_;
	float4 MapSize_;

	bool MoveLock_;
	bool CameraEvent_;
	float ZoomValue_;
	float DeltaTime_;
	bool CameraHolding_;
	float4 HoldingPos_;
};

