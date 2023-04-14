#pragma once

class SkillInfo
{
public:
	SkillInfo();
	~SkillInfo();

private:
	std::string Name_;
	float CoolTime_;
	float EndCoolTime_;
	int Damage_;
	int Mp_;

public:
	void SetName(const std::string& _Name)
	{
		Name_ = _Name;
	}

	std::string GetName()
	{
		return Name_;
	}

	void SetCoolTime(float _CoolTime, bool _End = false)
	{
		if (true == _End)
		{
			EndCoolTime_ = _CoolTime;
		}
		else
		{
			CoolTime_ = _CoolTime;
		}
	}

	float GetCoolTime()
	{
		return CoolTime_;
	}

	float GetEndCoolTime()
	{
		return EndCoolTime_;
	}

	void SetDamage(int _Damage)
	{
		Damage_ = _Damage;
	}

	int GetDamage()
	{
		return Damage_;
	}

	void SetMp(int _Mp)
	{
		Mp_ = _Mp;
	}

	int GetMp()
	{
		return Mp_;
	}
};

