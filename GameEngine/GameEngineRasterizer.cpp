#include "PreCompile.h"
#include "GameEngineRasterizer.h"

GameEngineRasterizer::GameEngineRasterizer() 
    : State_(nullptr)
    , ViewPort_()
    , Desc_()
    , OldState_()
    , WireDesc_()
    , rects()
{
}

GameEngineRasterizer::~GameEngineRasterizer()
{
    Clear();
}

void GameEngineRasterizer::Clear()
{
    if (nullptr != OldState_)
    {
        OldState_->Release();
        OldState_ = nullptr;
    }

    if (nullptr != State_)
    {
        State_->Release();
        State_ = nullptr;
    }
}

void GameEngineRasterizer::Create(const D3D11_RASTERIZER_DESC& _Value)
{
    Desc_ = _Value;
    if (0 == ScissorRects_.size())
    {
        Desc_.ScissorEnable = false;
    }

    WireDesc_ = Desc_;
    WireDesc_.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;


    if (S_OK != GameEngineDevice::GetDevice()->CreateRasterizerState(&Desc_, &State_))
    {
        GameEngineDebug::MsgBoxError("레스터라이저 생성에 실패했습니다.");
        return;
    }

    if (S_OK != GameEngineDevice::GetDevice()->CreateRasterizerState(&WireDesc_, &OldState_))
    {
        GameEngineDebug::MsgBoxError("레스터라이저 생성에 실패했습니다.");
        return;
    }
}

void GameEngineRasterizer::PushScissorRect(D3D11_RECT _Rect)
{
    ScissorRects_.push_back(_Rect);

    Desc_.ScissorEnable = true;

    Clear();
    Create(Desc_);
}

void GameEngineRasterizer::SettingViewPort() 
{
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPort_);
}

void GameEngineRasterizer::Setting() 
{
    GameEngineDevice::GetContext()->RSSetState(State_);
}

void GameEngineRasterizer::SwitchState() 
{
    ID3D11RasterizerState* State = State_;
    State_ = OldState_;
    OldState_ = State;
}