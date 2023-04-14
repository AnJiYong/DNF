#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

class Mouse : public GameEngineActor
{
public:
	Mouse();
	~Mouse();

protected:
	class GameEngineUIRenderer* Mouse_;
	class GameEngineCollision* MainCollision_;
	class GameEngineCollision* SubCollision_;

	float4 PrevPos_;
	float HideTime_;
	ItemType CatchItemType_;
	bool MouseChange_;

public:
	void SetCatchItemType(ItemType _Type)
	{
		CatchItemType_ = _Type;
	}

	ItemType GetCatchItemType()
	{
		return CatchItemType_;
	}

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void WindowCursorOn();
	void WindowCursorOff();
	void MouseChange(bool _Value);
};

