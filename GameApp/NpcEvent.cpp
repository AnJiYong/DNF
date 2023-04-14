#include "Precompile.h"
#include "NpcEvent.h"
#include "GlobalEnum.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine\GameEngineCollision.h"
#include "NpcDialog.h"

NpcEvent::NpcEvent()
	: ClickRenderer_(nullptr)
	, ButtonRenderer_(nullptr)
	, ButtonCollision_(nullptr)
	, NpcName_("")
{
}

NpcEvent::~NpcEvent()
{
}

void NpcEvent::Start()
{
	ClickRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	ClickRenderer_->SetImage("UI_NpcSeria.png");

	ButtonRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	ButtonRenderer_->GetTransform()->SetLocalPosition(float4(0.f, -21.f, 0.f));
	ButtonRenderer_->SetImage("UI_NpcTalk0.png");

	ButtonCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	ButtonCollision_->GetTransform()->SetLocalPosition(float4(0.f, -21.f, -10.f));
	ButtonCollision_->GetTransform()->SetLocalScaling(float4{ 100.f, 21.f, 1.f });
}

void NpcEvent::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(ButtonCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	ButtonRenderer_->SetImage("UI_NpcTalk0.png");

	ButtonCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&NpcEvent::MouseOn, this, std::placeholders::_1));
}

void NpcEvent::MouseOn(GameEngineCollision* _Other)
{
	ButtonRenderer_->SetImage("UI_NpcTalk1.png");

	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		NpcDialog* NpcDialog_ = GetLevel()->CreateActor<NpcDialog>();
		NpcDialog_->GetTransform()->SetWorldPosition(float4(0.f, 0.f, -100.f));
		NpcDialog_->SetNpcName(NpcName_);

		Off();
	}
}

void NpcEvent::SetNpcName(std::string _Name)
{
	NpcName_ = _Name;

	if ("Seria" == NpcName_)
	{
		ClickRenderer_->SetImage("UI_NpcSeria.png");
	}

	if ("Lia" == NpcName_)
	{
		ClickRenderer_->SetImage("UI_NpcLia.png");
	}
}
