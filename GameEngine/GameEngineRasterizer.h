#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngine/GameEngineDevice.h"
#include "GameEngine\GameEngineWindow.h"

class GameEngineRasterizer : public GameEngineObjectNameBase
{
public:
	GameEngineRasterizer();
	~GameEngineRasterizer();

protected:
	GameEngineRasterizer(const GameEngineRasterizer& _other) = delete;
	GameEngineRasterizer& operator=(const GameEngineRasterizer& _other) = delete;
	GameEngineRasterizer& operator=(const GameEngineRasterizer&& _other) = delete;

private:
	void Clear();

public:
	void AddWindowSizeViewPort()
	{
		ViewPort_.Height = GameEngineWindow::GetInst().GetSize().y;
		ViewPort_.Width = GameEngineWindow::GetInst().GetSize().x;
		ViewPort_.TopLeftX = 0;
		ViewPort_.TopLeftY = 0;
		ViewPort_.MinDepth = 0;
		ViewPort_.MaxDepth = 1.f;
	}

	void SetViewPort(float _Width, float _Height, float _TopLeftX, float _TopLeftY, float _MinDepth, float _MaxDepth)
	{
		ViewPort_.Height = _Height;
		ViewPort_.Width = _Width;
		ViewPort_.TopLeftX = _TopLeftX;
		ViewPort_.TopLeftY = _TopLeftY;
		ViewPort_.MinDepth = _MinDepth;
		ViewPort_.MaxDepth = _MaxDepth;
	}

	void Create(const D3D11_RASTERIZER_DESC& _Value);

	void PushScissorRect(D3D11_RECT _Rect);

	void SettingViewPort();

	void SwitchState();

	void Setting();

private:
	ID3D11RasterizerState* State_;
	ID3D11RasterizerState* OldState_;
	D3D11_RASTERIZER_DESC Desc_;
	D3D11_RASTERIZER_DESC WireDesc_;
	D3D11_VIEWPORT ViewPort_;
	D3D11_RECT rects;

	std::vector<D3D11_RECT> ScissorRects_;
};

