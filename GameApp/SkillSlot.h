#pragma once
#include "GlobalEnum.h"
#include "GameEngine/GameEngineActor.h"

class GameEngineUIRenderer;
class GameEngineCollision;
class GameEngineProgressBarRenderer;
class SkillSlot : public GameEngineActor
{
public:
	SkillSlot();
	~SkillSlot();

protected:
	GameEngineCollision* MainCollision_;
	GameEngineCollision* MoveCollision_;
	GameEngineCollision* MouseCollision_;
	GameEngineCollision* SlotCollision_;

	GameEngineUIRenderer* MainUIRenderer_;
	GameEngineUIRenderer* IconRenderer_;
	GameEngineUIRenderer* KeyRenderer_;
	GameEngineUIRenderer* SelectUIRenderer_;
	GameEngineUIRenderer* TextRenderer_;

	GameEngineProgressBarRenderer* CoolTimeRenderer_;

	bool Empty_;
	bool GetSkill_;
	std::string SkillName_;
	bool Block_;
	float CoolTime_;
	float CoolTimeMax_;

public:
	bool GetEmpty()
	{
		return Empty_;
	}

	std::string GetSkillName()
	{
		if (true == Block_)
		{
			return "CoolTime";
		}

		return SkillName_;
	}

private:
	void Start() override;
	void Update(float _Time) override;

public:
	void SetKey(const std::string& _Key, const std::string& _Skill);
	void SetSkill(const std::string& _Name);
	void GetSkill(GameEngineCollision* _Other);
	void ChangeSlot(GameEngineCollision* _Other);
	void CoolTime(float _Time);
};

