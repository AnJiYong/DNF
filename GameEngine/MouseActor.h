#pragma once
#include "GameEngineActor.h"
#include "GameEngineUIRenderer.h"

class MouseActor : public GameEngineActor
{
public:
	MouseActor();
	~MouseActor();

	MouseActor(const MouseActor& _Other) = delete;
	MouseActor(MouseActor&& _Other) noexcept = delete;
	MouseActor& operator=(const MouseActor& _Other) = delete;
	MouseActor& operator=(MouseActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SetCurSor(std::string _Name);
	void SetCurSor(std::string _Name, int _Index);

	GameEngineUIRenderer* GetUIRenderer() 
	{
		return UIRenderer;
	}

	void WindowCursorOn();
	void WindowCursorOff();

private:
	GameEngineUIRenderer* UIRenderer;
};

