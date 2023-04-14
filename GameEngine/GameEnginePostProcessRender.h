#pragma once
#include "GameEngineRenderTarget.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineBase\GameEngineDebug.h"
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineTexture.h"

class GameEnginePostProcessRender : public GameEngineObjectNameBase
{
	friend class GameEngineLevel;

public:
	GameEnginePostProcessRender();
	~GameEnginePostProcessRender();

	GameEnginePostProcessRender(const GameEnginePostProcessRender& _Other) = delete;
	GameEnginePostProcessRender(GameEnginePostProcessRender&& _Other) noexcept = delete;
	GameEnginePostProcessRender& operator=(const GameEnginePostProcessRender& _Other) = delete;
	GameEnginePostProcessRender& operator=(GameEnginePostProcessRender&& _Other) noexcept = delete;

protected:
	virtual void Initialize() = 0;
	virtual void Effect(float _DeltaTime) = 0;

public:
	inline void SetTarget(GameEngineRenderTarget* _Target) 
	{
		Target_ = _Target;
	}

	inline void SetResult(GameEngineRenderTarget* _Result)
	{
		if (true == IsResultCreate_
			&& nullptr == Result_)
		{
			delete Result_;
		}

		Result_ = _Result;
	}

	void CreateResultTarget();

	inline void SetEffect(const std::string& _Effect)
	{
		Effect_ = GameEngineRenderingPipeLineManager::GetInst().Find(_Effect);

		if (nullptr == Effect_)
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 효과를 주려고 했습니다.");
			return;
		}

		Res_.ShaderResourcesCheck(Effect_);
	}

	GameEngineRenderTarget* GetResult() 
	{
		return Result_;
	}

protected:
	GameEngineRenderTarget* Target_;
	GameEngineRenderingPipeLine* Effect_;
	GameEngineShaderResHelper Res_;

	bool IsResultCreate_;
	GameEngineRenderTarget* Result_;
};

