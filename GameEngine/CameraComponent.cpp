#include "PreCompile.h"
#include "CameraComponent.h"
#include "GameEngineTransform.h"
#include "GameEngineRenderer.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineVertexShader.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineWindow.h"

CameraComponent::CameraComponent() :
	ProjectionMode_(ProjectionMode::Perspective)
	, FovAngleY_(90.f)
	, CamSize_(GameEngineWindow::GetInst().GetSize())
	, NearZ_(0.1f)
	, FarZ_(1000.f)
	, DebugRenderCount_(0)
	, CameraBufferTarget_(nullptr)
	, ZoomValue_(0.f)
{
}

CameraComponent::~CameraComponent()
{
	if (nullptr != CameraBufferTarget_)
	{
		delete CameraBufferTarget_;
		CameraBufferTarget_ = nullptr;
	}
}

void CameraComponent::ClearCameraTarget()
{
	CameraBufferTarget_->Clear();
}

void CameraComponent::CameraTransformUpdate()
{
	GetTransform()->GetTransformData().View_.ViewToLH(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldForwardVector(), GetTransform()->GetWorldUpVector());

	switch (ProjectionMode_)
	{
	case ProjectionMode::Perspective:
	{
		GetTransform()->GetTransformData().Projection_.PerspectiveFovLH(FovAngleY_, CamSize_.x, CamSize_.y, NearZ_, FarZ_);
		break;
	}
	case ProjectionMode::Orthographic:
	{
		GetTransform()->GetTransformData().Projection_.OrthographicLH(CamSize_.x, CamSize_.y, NearZ_, FarZ_);
		break;
	}
	}
}

bool ZSort(GameEngineRenderer* _Left, GameEngineRenderer* _Right)
{
	return _Left->GetTransform()->GetWorldPosition().z > _Right->GetTransform()->GetWorldPosition().z;
}

void CameraComponent::Render()
{
	CameraBufferTarget_->Setting();

	CameraTransformUpdate();

	float4x4 View = GetTransform()->GetTransformData().View_;
	float4x4 Projection = GetTransform()->GetTransformData().Projection_;

	for (std::pair<int, std::list<GameEngineRenderer*>> Pair : RendererList_)
	{
		std::list<GameEngineRenderer*>& Renderers = Pair.second;
		
		Renderers.sort(ZSort);
		
		for (GameEngineRenderer* Renderer : Renderers)
		{
			if (false == Renderer->IsUpdate())
			{
				continue;
			}

			Renderer->GetTransform()->GetTransformData().Projection_ = Projection;
			Renderer->GetTransform()->GetTransformData().View_ = View;

			Renderer->GetTransform()->GetTransformData().WVPCalculation();

			Renderer->Render();
		}
	}
}

void CameraComponent::DebugRender()
{
	if (true == IsDebugCheck())
	{
		return;
	}

	CameraBufferTarget_->Setting();

	float4x4 View = GetTransform()->GetTransformData().View_;
	float4x4 Projection = GetTransform()->GetTransformData().Projection_;

	for (size_t i = 0; i < DebugRenderCount_; i++)
	{
		DebugVector_[i].Data_.Projection_ = Projection;
		DebugVector_[i].Data_.View_ = View;
		DebugVector_[i].Data_.WVPCalculation();

		DebugVector_[i].ShaderHelper_.Setting();
		DebugVector_[i].PipeLine_->Rendering();
		DebugVector_[i].ShaderHelper_.ReSet();
		DebugVector_[i].PipeLine_->Reset();
	}

	DebugRenderCount_ = 0;
}

void CameraComponent::ReleaseRenderer()
{
	std::map<int, std::list<GameEngineRenderer*>>::iterator RenderMapBeginIter = RendererList_.begin();
	std::map<int, std::list<GameEngineRenderer*>>::iterator RenderMapEndIter = RendererList_.end();

	for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
	{
		std::list<GameEngineRenderer*>& Renderers = RenderMapBeginIter->second;

		std::list<GameEngineRenderer*>::iterator BeginIter = Renderers.begin();
		std::list<GameEngineRenderer*>::iterator EndIter = Renderers.end();

		for (; BeginIter != EndIter; )
		{
			GameEngineRenderer* ReleaseRenderer = *BeginIter;

			if (nullptr == ReleaseRenderer)
			{
				GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
			}

			if (true == ReleaseRenderer->IsDeath())
			{
				BeginIter = Renderers.erase(BeginIter);

				continue;
			}

			++BeginIter;
		}
	}
}

void CameraComponent::NextLevelMoveRenderer(CameraComponent* _NextCamera, GameEngineActor* _Actor)
{
	std::map<int, std::list<GameEngineRenderer*>>::iterator RenderMapBeginIter = RendererList_.begin();
	std::map<int, std::list<GameEngineRenderer*>>::iterator RenderMapEndIter = RendererList_.end();

	for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
	{
		std::list<GameEngineRenderer*>& Renderers = RenderMapBeginIter->second;

		std::list<GameEngineRenderer*>::iterator BeginIter = Renderers.begin();
		std::list<GameEngineRenderer*>::iterator EndIter = Renderers.end();

		for (; BeginIter != EndIter; )
		{
			GameEngineRenderer* ReleaseRenderer = *BeginIter;

			if (nullptr == ReleaseRenderer)
			{
				GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
			}

			if (ReleaseRenderer->GetActor() == _Actor)
			{
				_NextCamera->RendererList_[(*BeginIter)->GetOrder()].push_back(*BeginIter);

				BeginIter = Renderers.erase(BeginIter);


				continue;
			}

			++BeginIter;
		}
	}
}

void CameraComponent::Start()
{
	DebugVector_.resize(1000);
	DebugRenderCount_ = 0;

	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
	for (size_t i = 0; i < DebugVector_.size(); i++)
	{
		DebugVector_[i].ShaderHelper_.ShaderResourcesCheck(Pipe->GetVertexShader());
		DebugVector_[i].ShaderHelper_.ShaderResourcesCheck(Pipe->GetPixelShader());
		DebugVector_[i].Color_ = float4::GREEN;
		DebugVector_[i].ShaderHelper_.SettingConstantBufferLink("ResultColor", DebugVector_[i].Color_);
		DebugVector_[i].ShaderHelper_.SettingConstantBufferLink("TransformData", DebugVector_[i].Data_);
	}

	CameraBufferTarget_ = new GameEngineRenderTarget();
	CameraBufferTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	CameraBufferTarget_->CreateDepthBuffer(GameEngineWindow::GetInst().GetSize());
}

void CameraComponent::Update(float _DeltaTime)
{
}

void CameraComponent::CameraZoomReset()
{
	if (ProjectionMode_ == ProjectionMode::Orthographic)
	{
		CamSize_ = GameEngineWindow::GetInst().GetSize();
	}
}

void CameraComponent::CameraZoomSetting(float _Value)
{
	if (ProjectionMode_ == ProjectionMode::Orthographic)
	{
		ZoomValue_ = _Value;
		CamSize_ = GameEngineWindow::GetInst().GetSize() * _Value;
	}
}

void CameraComponent::SetProjectionMode(ProjectionMode _ProjectionMode)
{
	ProjectionMode_ = _ProjectionMode;
}

void CameraComponent::PushRenderer(int _Order, GameEngineRenderer* _Renderer)
{
	RendererList_[_Order].push_back(_Renderer);
}

void CameraComponent::PushDebugRender(GameEngineTransform* _Trans, CollisionType _Type)
{
	switch (_Type)
	{
	case CollisionType::Point2D:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		break;
	}
	case CollisionType::CirCle:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		break;
	}
	case CollisionType::Rect:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		break;
	}
	case CollisionType::OrientedRect:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		break;
	}

	case CollisionType::Point3D:
	case CollisionType::Sphere3D:
	case CollisionType::AABBBox3D:
	case CollisionType::OBBBox3D:
	case CollisionType::MAX:
	{
		GameEngineDebug::MsgBoxError("처리할수 없는 디버그 타입입니다.");
		break;
	}
	}

	DebugVector_[DebugRenderCount_].Data_ = _Trans->GetTransformData();
	++DebugRenderCount_;
}

void CameraComponent::ChangeRendererGroup(int _Group, GameEngineRenderer* _Renderer)
{
	RendererList_[_Renderer->GetOrder()].remove(_Renderer);
	_Renderer->SetOrder(_Group);
	RendererList_[_Renderer->GetOrder()].push_back(_Renderer);
}

