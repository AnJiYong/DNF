#pragma once
#include "GameEnginePostProcessRender.h"

enum FadeOption
{
	LIGHT,
	DARK,
};

struct FadeData
{
	float CurTime;
	float Ratio;
	int Clip;			 

	float Time;
};

class PostFade : public GameEnginePostProcessRender
{
public:
	PostFade();
	~PostFade();

	PostFade(const PostFade& _Other) = delete;
	PostFade(PostFade&& _Other) noexcept = delete;
	PostFade& operator=(const PostFade& _Other) = delete;
	PostFade& operator=(PostFade&& _Other) noexcept = delete;

protected:
	void Effect(float _DeltaTime) override;
	void Initialize() override;

public:
	void SetData(float _Time, FadeOption _Option, bool _Auto = true);

private:
	FadeData Data;
	FadeOption Option;
	float FadeTime_;
	bool Auto_;
};

