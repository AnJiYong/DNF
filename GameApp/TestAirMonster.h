#pragma once
#include "MonsterBase.h"

class TestAirMonster : public MonsterBase
{
public:
	TestAirMonster();
	~TestAirMonster();

protected:
	class GameEngineImageRenderer* ImageRenderer_;
	class GameEngineImageRenderer* ImageRenderer1_;
	class GameEngineCollision* ImageCollision_;
	class GameEngineCollision* ZImageCollision_;
	class GameEngineSoundPlayer* Sound_;
	int HP_;
	float ZPos;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Damege();
};