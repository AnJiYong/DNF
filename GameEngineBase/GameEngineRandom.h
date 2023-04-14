#pragma once

class GameEngineRandom
{
public:
	GameEngineRandom()
		: mt_()
	{
		std::random_device rng_;
		mt_.seed(rng_());
	}

	GameEngineRandom(__int64 _Seed)
		: mt_(_Seed)
	{
	}

	~GameEngineRandom()
	{
	}

protected:
	GameEngineRandom(const GameEngineRandom& _other) = delete;
	GameEngineRandom(GameEngineRandom&& _other) noexcept;

private:
	GameEngineRandom& operator=(const GameEngineRandom& _other) = delete;
	GameEngineRandom& operator=(const GameEngineRandom&& _other) = delete;

public:
	int RandomInt(int _min, int _max)
	{
		std::uniform_int_distribution<int> Dis(_min, _max);
		return Dis(mt_);
	}

	float RandomFloat(float _min, float _max)
	{
		std::uniform_real_distribution<float> Dis(_min, _max);
		return Dis(mt_);
	}

private:
	std::mt19937_64 mt_;
};

