#pragma once
#include "GameEngine/GameEngineActor.h"
#include "GlobalEnum.h"

class GameEngineImageRenderer;
class Avatar : public GameEngineActor
{
public:
	Avatar();
	~Avatar();

private:
	GameEngineImageRenderer* Weapon_Front1;
	GameEngineImageRenderer* Weapon_Front2;
	GameEngineImageRenderer* Weapon_Back1;
	GameEngineImageRenderer* Weapon_Back2;

	GameEngineImageRenderer* Avatar_Skin;

	GameEngineImageRenderer* Avatar_BeltA;
	GameEngineImageRenderer* Avatar_BeltC;
	GameEngineImageRenderer* Avatar_BeltD;
	
	GameEngineImageRenderer* Avatar_CapA;
	GameEngineImageRenderer* Avatar_CapB;

	GameEngineImageRenderer* Avatar_CoatA;
	GameEngineImageRenderer* Avatar_CoatB;
	GameEngineImageRenderer* Avatar_CoatC;
	GameEngineImageRenderer* Avatar_CoatD;
	GameEngineImageRenderer* Avatar_CoatX;

	GameEngineImageRenderer* Avatar_FaceB;

	GameEngineImageRenderer* Avatar_HairA;
	GameEngineImageRenderer* Avatar_HairD;

	GameEngineImageRenderer* Avatar_NeckA;
	GameEngineImageRenderer* Avatar_NeckC;
	GameEngineImageRenderer* Avatar_NeckD;
	GameEngineImageRenderer* Avatar_NeckX;

	GameEngineImageRenderer* Avatar_PantsA;
	GameEngineImageRenderer* Avatar_PantsB;

	GameEngineImageRenderer* Avatar_ShoesA;
	GameEngineImageRenderer* Avatar_ShoesB;

	GameEngineImageRenderer* Avatar_AuraFront;
	GameEngineImageRenderer* Avatar_AuraBack;

	GameEngineImageRenderer* Title;

	std::vector<GameEngineImageRenderer*> List_;

	int Index_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Weapon();
	void Avatar_Belt();
	void Avatar_Cap();
	void Avatar_Coat();
	void Avatar_Face();
	void Avatar_Hair();
	void Avatar_Neck();
	void Avatar_Pants();
	void Avatar_Shoes();

public:
	void SetIndex(int _Index)
	{
		Index_ = _Index;
	}

	void ChangeAvatar(const std::string& _Name, ItemType _Type);
	void SetReverse(bool _Reverse);
	void SetRotation(float4 _Value);
};

