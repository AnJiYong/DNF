#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineBase\GameEngineObjectNameBase.h"

struct CorrectResultColor
{
	float4 vMulColor;
	float4 vPlusColor;
};

class GameEngineTexture;
class GameEngineFolderTexture;
class GameEngineImageRenderer : public GameEngineRenderer
{
private:
	class Animation2D : public GameEngineObjectNameBase
	{
		friend GameEngineImageRenderer;

	private:
		Animation2D();
		~Animation2D();

	public:
		void SetCurrentIndex(int _Index)
		{
			CurFrame_ = _Index;
		}

		void Reset();
		void CallStart();
		void CallEnd();
		void CallFrame();
		void Update(float _DeltaTime);

		void FrameUpdate();
		void ReverseFrameUpdate();

	public:
		GameEngineFolderTexture* FolderTextures_;
		GameEngineTexture* AnimationTexture_;
		float InterTime_;
		float CurTime_;

		bool IsEnd;
		bool Loop_;
		int CurFrame_;
		int StartFrame_;
		int EndFrame_;

		GameEngineImageRenderer* Renderer;

		std::map<int, std::vector<std::function<void()>>> FrameCallBack_;
		std::vector<std::function<void()>> EndCallBack_;
		std::vector<std::function<void()>> StartCallBack_;
	};

public:
	GameEngineImageRenderer();
	~GameEngineImageRenderer();

	GameEngineImageRenderer(const GameEngineImageRenderer& _Other) = delete;
	GameEngineImageRenderer(GameEngineImageRenderer&& _Other) noexcept = delete;
	GameEngineImageRenderer& operator=(const GameEngineImageRenderer& _Other) = delete;
	GameEngineImageRenderer& operator=(GameEngineImageRenderer&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void SetRenderingPipeLineSettingNext() override;

public:
	void SetImage(const std::string& _ImageName, const std::string& _Sampler = "");

	void SetIndex(const int Index);

	void CreateAnimation(const std::string& _TextureName, const std::string& _Name, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop = true);

	void CreateAnimationFolder(const std::string& _Name, const std::string& _FolderTexName, float _InterTime, bool _Loop = true);

	void CreateAnimationFolder(const std::string& _Name, const std::string& _FolderTexName, float _InterTime, int _Cur, int _End, bool _Loop = true);

	void SetChangeAnimation(const std::string& _Name, bool _IsForce = false);
	void SetChangeAnimation(const std::string& _Name, int _Frame, bool _IsForce = false);

	void SetStartCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetEndCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetFrameCallBack(const std::string& _Name, int _Index, std::function<void()> _CallBack);

	void AnimaOff();

	inline GameEngineTexture* GetCurrentTexture()
	{
		return CurTexture;
	}

	inline GameEngineTexture* GetCurAnimationTexture()
	{
		return CurAnimationTexture;
	}

	inline Animation2D* GetCurrentAnimation()
	{
		return CurAnimation_;
	}

	inline std::string GetCurrentAnimationName()
	{
		return CurAnimation_->GetName();
	}

	inline void IsCurrentAnimationIndex(const int _Index)
	{
		if (false == IsUpdate())
		{
			return;
		}

		CurAnimation_->SetCurrentIndex(_Index);
	}

	inline bool IsCurrentAnimationString(const std::string& _Name)
	{
		return CurAnimation_->GetName() == _Name;
	}

	inline bool IsCurrentAnimationPtr(const char* _Name)
	{
		return CurAnimation_->GetName() == _Name;
	}

	inline void SetAlpha(float _Value)
	{
		CorrectResultColor_.vMulColor.a = _Value;
	}

	inline void SetPlusColor(float4 _Color)
	{
		CorrectResultColor_.vPlusColor = _Color;
	}

	inline void SetColor(float4 _Value)
	{
		CorrectResultColor_.vMulColor = _Value;
	}

	void SetReverse(bool _Reverse)
	{
		Reverse = _Reverse;
	}

	inline void AnimationStop()
	{
		IsPlay_ = false;
	}

	inline void AnimationPlay()
	{
		IsPlay_ = true;
	}

protected:
	std::map<std::string, Animation2D*> AllAnimations_;
	Animation2D* CurAnimation_;

	bool Reverse;
	CorrectResultColor CorrectResultColor_;
	float4 CutData;
	GameEngineTexture* CurTexture;
	GameEngineTexture* CurAnimationTexture;
	bool IsPlay_;
};

