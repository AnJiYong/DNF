#include "PreCompile.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineLevel.h"
#include "CameraComponent.h"
#include "GameEngineFont.h"
#include "GameEngineFontManager.h"
#include "GameEngineWindow.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderTarget* GameEngineUIRenderer::FontTarget_ = nullptr;
int GameEngineUIRenderer::UIRendererCount = 0;

GameEngineUIRenderer::GameEngineUIRenderer()
	: FontName_("±Ã¼­")
	, PrintText_("")
	, FontPivot_(float4::ZERO)
	, Color_(float4::BLACK)
	, OutLineColor_(float4::GREEN)
	, OutLine_(false)
	, Flag_(FW1_CENTER)
	, FontSize_(0.f)
{
	++UIRendererCount;
}

GameEngineUIRenderer::~GameEngineUIRenderer()
{
	--UIRendererCount;
	if (0 == UIRendererCount && nullptr != FontTarget_)
	{
		delete FontTarget_;
		FontTarget_ = nullptr;
	}
}

void GameEngineUIRenderer::Start()
{
	GetLevel()->GetUICamera()->PushRenderer(GetOrder(), this);
	SetRenderingPipeLine("TextureUI");

	if (nullptr == FontTarget_
		&& UIRendererCount == 1)
	{
		FontTarget_ = new GameEngineRenderTarget();
		FontTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	}
}

void GameEngineUIRenderer::Render()
{
	GameEngineRenderer::Render();

	if ("" == PrintText_)
	{
		return;
	}

	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();

	ScreenSize = ScreenSize.halffloat4();
	float4 UIPos = GetTransform()->GetWorldPosition();

	GameEngineRenderTarget* RenderTarget = GameEngineRenderTarget::GetLastRenderTarget();

	FontTarget_->Clear();
	FontTarget_->Setting();

	float4 Pos = float4::ZERO;
	Pos.x = ScreenSize.x + UIPos.x + FontPivot_.x;
	Pos.y = ScreenSize.y - UIPos.y - FontPivot_.y;

	GameEngineFont* Font = GameEngineFontManager::GetInst().Find(FontName_);

	if (true == OutLine_)
	{
		for (size_t y = 0; y < 3; y++)
		{
			for (size_t x = 0; x < 3; x++)
			{
				if (x == 1 && y == 1)
				{
					continue;
				}

				float4 Pivot;
				Pivot.x = static_cast<float>(x) - 1.f;
				Pivot.y = static_cast<float>(y) - 1.f;

				Font->DrawFont(PrintText_, FontSize_, Pos + Pivot, OutLineColor_, Flag_);
			}
		}
	}

	Font->DrawFont(PrintText_, FontSize_, Pos, Color_, Flag_);

	GameEngineDevice::ShaderReset();
	RenderTarget->Merge(FontTarget_);
	RenderTarget->Setting();
}

void GameEngineUIRenderer::SetRenderGroup(int _Order) 
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);;
}

void GameEngineUIRenderer::TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, float4 _Color, float4 _Pivot, FW1_TEXT_FLAG _Flag)
{
	FontName_ = _FontName;
	PrintText_ = _PrintText;
	FontSize_ = _FontSize;
	Color_ = _Color;
	FontPivot_ = _Pivot;
	Flag_ = _Flag;
}