#include "Precompile.h"
#include "NpcDialog.h"
#include "GlobalEnum.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine\GameEngineCollision.h"

NpcDialog::NpcDialog()
	: DialogRenderer_(nullptr)
	, ButtonRenderer_(nullptr)
	, ButtonCollision_(nullptr)
	, ImageRenderer_(nullptr)
	, NameRenderer_(nullptr)
	, NpcName_("")
	, Alpha_(0.f)
{
}

NpcDialog::~NpcDialog()
{
}

void NpcDialog::Start()
{
	DialogRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	DialogRenderer_->GetTransform()->SetWorldPosition(float4(0.f, -200.f, 0.f));
	DialogRenderer_->SetImage("UI_Dialog0.png");
	DialogRenderer_->GetTransform()->SetLocalScaling(float4(750.f, 200.f));
	DialogRenderer_->TextSetting("gulim", "안녕 난 세리아라고해", 18, float4::WHITE, float4(0.f, 12.f));
	DialogRenderer_->SetOutLine(float4::BLACK);
	DialogRenderer_->SetAlpha(0.f);

	ButtonRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	ButtonRenderer_->GetTransform()->SetLocalPosition(float4(320.f, -265.f, 0.f));
	ButtonRenderer_->SetImage("UI_Dialog1.png");
	ButtonRenderer_->SetAlpha(0.f);

	ImageRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	ImageRenderer_->GetTransform()->SetLocalPosition(float4(-200.f, -148.f, 0.f));
	ImageRenderer_->SetImage("UI_NpcSeria1.png");
	ImageRenderer_->SetAlpha(0.f);

	NameRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	NameRenderer_->GetTransform()->SetLocalPosition(float4(-230.f, -270.f, 0.f));
	NameRenderer_->SetImage("UI_Dialog3.png");
	NameRenderer_->TextSetting("gulim", "세리아", 18, float4::WHITE, float4(0.f, 12.f));
	NameRenderer_->SetOutLine(float4::BLACK);
	NameRenderer_->SetAlpha(0.f);

	ButtonCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::UI));
	ButtonCollision_->GetTransform()->SetLocalPosition(float4(320.f, -265.f, 0.f));
	ButtonCollision_->GetTransform()->SetLocalScaling(float4{ 75.f, 31.f, 1.f });
}

void NpcDialog::Update(float _DeltaTime)
{
#ifdef _DEBUG
	GetLevel()->PushUIDebugRender(ButtonCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	ButtonRenderer_->SetImage("UI_Dialog1.png");

	if (Alpha_ <= 1.f)
	{
		Alpha_ += _DeltaTime * 3.f;
		DialogRenderer_->SetAlpha(Alpha_);
		ButtonRenderer_->SetAlpha(Alpha_);
		ImageRenderer_->SetAlpha(Alpha_);
		NameRenderer_->SetAlpha(Alpha_);
	}

	ButtonCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Mouse)
		, std::bind(&NpcDialog::MouseOn, this, std::placeholders::_1));

	if (true == GameEngineInput::GetInst().Down("Enter"))
	{
		Death();
	}
}

void NpcDialog::MouseOn(GameEngineCollision* _Other)
{
	ButtonRenderer_->SetImage("UI_Dialog2.png");

	if (true == GameEngineInput::GetInst().Down("MouseLButton"))
	{
		Death();
	}
}

void NpcDialog::SetNpcName(std::string _Name)
{
	NpcName_ = _Name;

	if ("Seria" == NpcName_)
	{
		ImageRenderer_->SetImage("UI_NpcSeria1.png");
		NameRenderer_->TextSetting("Gasinamu", "세리아 키르민", 18, float4(237.f / 255.f, 213.f / 255.f, 127.f / 255.f), float4(0.f, 12.f));
		DialogRenderer_->TextSetting("Gasinamu", "세리아 키르민", 18, float4(237.f / 255.f, 213.f / 255.f, 127.f / 255.f), float4(0.f, 12.f));
		DialogRenderer_->SetOutLine(float4::BLACK);
	}

	if ("Lia" == NpcName_)
	{
		ImageRenderer_->SetImage("UI_NpcLia1.png");
		ImageRenderer_->GetTransform()->SetLocalPosition(float4(-250.f, -148.f, 0.f));
		NameRenderer_->TextSetting("gulim", "리아 리히터", 18, float4(237.f / 255.f, 213.f / 255.f, 127.f / 255.f), float4(0.f, 12.f));
		DialogRenderer_->TextSetting("gulim", "안녕 난 리아라고해", 18, float4(237.f / 255.f, 213.f / 255.f, 127.f / 255.f), float4(0.f, 12.f));
		DialogRenderer_->SetOutLine(float4::BLACK);
	}
}
