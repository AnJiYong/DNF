#pragma once
#include "GameEngine/GameEngineActor.h"

class Potal : public GameEngineActor
{
public:
	Potal();
	~Potal();

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Teleport();

protected:
	class GameEngineImageRenderer* ImageRenderer_;
	class GameEngineImageRenderer* UpImageRenderer_;
	class GameEngineCollision* ImageCollision;
};

