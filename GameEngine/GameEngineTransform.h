#pragma once
#include "GameEngineBase/GameEngineMath.h"
#include "GameEngineComponent.h"
#include "GameEngineBase/GameEngineTime.h"

#include <DirectXCollision.h>
#include <DirectXCollision.inl>

union CollisionData
{
public:
	DirectX::BoundingSphere Sphere;
	DirectX::BoundingBox AABB;
	DirectX::BoundingOrientedBox OBB;

	CollisionData() 
		: OBB()
	{

	}
};

class TransformData 
{
public:
	TransformData() 
		: vWorldScaling_(float4::ONE)
		, vLocalScaling_(float4::ONE)
	{
	}

	void LocalCalculation()
	{
		LocalScaling_.Scaling(vLocalScaling_);
		LocalRotation_.RotationDeg(vLocalRotation_);
		LocalPosition_.Translation(vLocalPosition_);

		LocalWorld_ = LocalScaling_ * LocalRotation_ * LocalPosition_;
	}

	void ParentSetting(const float4x4& _Parent) 
	{
		Parent_ = _Parent;
		WorldWorld_ = LocalWorld_;
		WorldWorld_ *= Parent_;
	}

	void WVPCalculation()
	{
		WVP_ = WorldWorld_ * View_ * Projection_;
	}

	void RootCalculation() 
	{
		WorldWorld_ = LocalWorld_;
	}

public:
	float4 vWorldPosition_;
	float4 vWorldRotation_;
	float4 vWorldScaling_;

	float4 vLocalPosition_;
	float4 vLocalRotation_;
	float4 vLocalScaling_;

	float4x4 LocalScaling_;
	float4x4 LocalRotation_;
	float4x4 LocalPosition_;
	float4x4 LocalWorld_;

	float4x4 Parent_;
	float4x4 WorldWorld_;

	float4x4 View_;
	float4x4 Projection_;

	float4x4 WVP_;
};

class GameEngineTransform 
{
public:
	GameEngineTransform();
	~GameEngineTransform();

	GameEngineTransform& operator=(const GameEngineTransform& _Other) = delete;
	GameEngineTransform& operator=(GameEngineTransform&& _Other) noexcept = delete;

private:
	void AllChildCalculationScaling();
	void AllChildCalculationRotation();
	void AllChildCalculationPosition();

	void CalculationLocalScaling();
	void CalculationWorldScaling();

	void CalculationWorldRotation();
	void CalculationLocalRotation();

	void CalculationLocalPosition();
	void CalculationWorldPosition();

public:
	void TransformUpdate();

	float4 GetLocalScaling() { return TransformData_.vLocalScaling_; }
	float4 GetWorldScaling() { return TransformData_.vWorldScaling_; }
	float4 GetLocalRotation() { return TransformData_.vLocalRotation_; }
	float4 GetWorldRotation() { return TransformData_.vWorldRotation_; }
	float4 GetLocalPosition() { return TransformData_.vLocalPosition_; }
	float4 GetWorldPosition() { return TransformData_.vWorldPosition_; }

	float4 GetWorldForwardVector() { return TransformData_.WorldWorld_.vz.NormalizeReturn3D(); }
	float4 GetLocalForwardVector() { return TransformData_.LocalWorld_.vz.NormalizeReturn3D(); }

	float4 GetWorldBackVector() { return -TransformData_.WorldWorld_.vz.NormalizeReturn3D(); }
	float4 GetLocalBackVector() { return -TransformData_.LocalWorld_.vz.NormalizeReturn3D(); }

	float4 GetWorldRightVector() { return TransformData_.WorldWorld_.vx.NormalizeReturn3D(); }
	float4 GetLocalRightVector() { return TransformData_.LocalWorld_.vx.NormalizeReturn3D(); }

	float4 GetWorldLeftVector() { return -TransformData_.WorldWorld_.vx.NormalizeReturn3D(); }
	float4 GetLocalLeftVector() { return -TransformData_.LocalWorld_.vx.NormalizeReturn3D(); }

	float4 GetWorldUpVector() { return TransformData_.WorldWorld_.vy.NormalizeReturn3D(); }
	float4 GetLocalUpVector() { return TransformData_.LocalWorld_.vy.NormalizeReturn3D(); }

	float4 GetWorldDownVector() { return -TransformData_.WorldWorld_.vy.NormalizeReturn3D(); }
	float4 GetLocalDownVector() { return -TransformData_.LocalWorld_.vy.NormalizeReturn3D(); }

	void SetLocalScaling(const float4& _Value);
	void SetWorldScaling(const float4& _Value);

	void SetLocalRotationDegree(const float4& _Value);
	void SetWorldRotationDegree(const float4& _Value);

	void AddLocalRotationDegreeX(const float _Value) 
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.x += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeX(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.x += _Value;
		SetWorldRotationDegree(Local);
	}

	void AddLocalRotationDegreeY(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.y += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeY(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.y += _Value;
		SetWorldRotationDegree(Local);
	}

	void AddLocalRotationDegreeZ(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.z += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeZ(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.z += _Value;
		SetWorldRotationDegree(Local);
	}

	inline void AddLocalDeltaTimeRotation(const float4& _Value)
	{
		SetLocalRotationDegree(TransformData_.vLocalRotation_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void AddWorldDeltaTimeRotation(const float4& _Value)
	{
		SetWorldRotationDegree(TransformData_.vWorldRotation_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	void SetLocalPosition(const float4& _Value);
	void SetWorldPosition(const float4& _Value);
	void SetWorldPositionMove(const float4& _Value);
	void SetWorldPositionZ(float _Value);

	inline void SetLocalMove(const float4& _Value)
	{
		SetLocalPosition(TransformData_.vLocalPosition_ + _Value);
	}

	inline void SetWorldMove(const float4& _Value)
	{
		SetWorldPosition(TransformData_.vWorldPosition_ + _Value);
	}

	inline void SetLocalDeltaTimeMove(const float4& _Value)
	{
		SetLocalPosition(TransformData_.vLocalPosition_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void SetWorldDeltaTimeMove(const float4& _Value)
	{
		SetWorldPosition(TransformData_.vWorldPosition_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	void DetachChildTransform(GameEngineTransform* _Child);
	void AttachTransform(GameEngineTransform* _Transform);

	inline TransformData& GetTransformData()
	{
		return TransformData_;
	}

	inline const CollisionData& GetCollisionData()
	{
		return ColData_;
	}

	inline const DirectX::BoundingSphere& GetSphere()
	{
		return ColData_.Sphere;
	}

	inline const DirectX::BoundingOrientedBox& GetOBB()
	{
		return ColData_.OBB;
	}

	inline const DirectX::BoundingBox& GetAABB() 
	{
		return ColData_.AABB;
	}

	void Copy(const GameEngineTransform& _Other);

protected:
	TransformData TransformData_;
	CollisionData ColData_;

	GameEngineTransform* Parent_;
	std::vector<GameEngineTransform*> Childs_;
};

