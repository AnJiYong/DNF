#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

class GameEngineUIRenderer;
class GameEngineProgressBarRenderer;
class AvatarUI : public GameEngineActor
{
public:
	AvatarUI();
	~AvatarUI();

private:
	GameEngineProgressBarRenderer* Weapon_Front1;
	GameEngineProgressBarRenderer* Weapon_Front2;
	GameEngineProgressBarRenderer* Weapon_Back1;
	GameEngineProgressBarRenderer* Weapon_Back2;

	GameEngineUIRenderer* Avatar_Skin;

	GameEngineUIRenderer* Avatar_BeltA;
	GameEngineUIRenderer* Avatar_BeltC;
	GameEngineUIRenderer* Avatar_BeltD;

	GameEngineUIRenderer* Avatar_CapA;
	GameEngineUIRenderer* Avatar_CapB;

	GameEngineUIRenderer* Avatar_CoatA;
	GameEngineUIRenderer* Avatar_CoatB;
	GameEngineUIRenderer* Avatar_CoatC;
	GameEngineUIRenderer* Avatar_CoatD;
	GameEngineUIRenderer* Avatar_CoatX;

	GameEngineUIRenderer* Avatar_FaceB;

	GameEngineUIRenderer* Avatar_HairA;
	GameEngineUIRenderer* Avatar_HairD;

	GameEngineUIRenderer* Avatar_NeckA;
	GameEngineUIRenderer* Avatar_NeckC;
	GameEngineUIRenderer* Avatar_NeckD;
	GameEngineUIRenderer* Avatar_NeckX;

	GameEngineUIRenderer* Avatar_PantsA;
	GameEngineUIRenderer* Avatar_PantsB;

	GameEngineUIRenderer* Avatar_ShoesA;
	GameEngineUIRenderer* Avatar_ShoesB;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Avatar_Belt();
	void Avatar_Cap();
	void Avatar_Coat();
	void Avatar_Face();
	void Avatar_Hair();
	void Avatar_Neck();
	void Avatar_Pants();
	void Avatar_Shoes();

public:
	void ChangeAvatar(const std::string& _Name, ItemType _Type);
};

