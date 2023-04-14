#include "Precompile.h"
#include "Avatar.h"
#include "GameManager.h"
#include "Player.h"
#include "GameEngine/GameEngineImageRenderer.h"

Avatar::Avatar()
	: Avatar_BeltA(nullptr)
	, Avatar_BeltD(nullptr)
	, Avatar_CapA(nullptr)
	, Avatar_CapB(nullptr)
	, Avatar_CoatA(nullptr)
	, Avatar_CoatB(nullptr)
	, Avatar_CoatC(nullptr)
	, Avatar_FaceB(nullptr)
	, Avatar_HairA(nullptr)
	, Avatar_HairD(nullptr)
	, Avatar_NeckA(nullptr)
	, Avatar_PantsA(nullptr)
	, Avatar_PantsB(nullptr)
	, Avatar_ShoesA(nullptr)
	, Avatar_ShoesB(nullptr)
	, Index_(0)
{
}

Avatar::~Avatar()
{
}

void Avatar::Start()
{
	Avatar_Skin = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_Skin->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));
	Avatar_Skin->CreateAnimationFolder("default", "Avatar_Skin00", FLT_MAX, 0, 241);
	Avatar_Skin->CreateAnimationFolder("Avatar_Skin01", "Avatar_Skin01", FLT_MAX, 0, 241);
	Avatar_Skin->CreateAnimationFolder("Avatar_Skin02", "Avatar_Skin02", FLT_MAX, 0, 241);
	Avatar_Skin->SetChangeAnimation("default");
	List_.push_back(Avatar_Skin);

	Avatar_Shoes();
	Avatar_Pants();
	Avatar_Coat();
	Avatar_Belt();
	Avatar_Face();
	Avatar_Neck();
	Avatar_Hair();
	Avatar_Cap();
	Weapon();

	Title = CreateTransformComponent<GameEngineImageRenderer>();
	Title->GetTransform()->SetLocalPosition(float4(0.f, 130.f, -0.04f));
	Title->CreateAnimationFolder("Title00", "Title00", 0.1f, 0, 148);
	Title->CreateAnimationFolder("Title01", "Title01", 0.1f, 0, 169);
	Title->SetChangeAnimation("default");

	Avatar_AuraBack = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_AuraBack->GetTransform()->SetLocalPosition(float4(0.f, 50.f, -0.04f));
	Avatar_AuraBack->SetRenderingPipeLine("TextureTrans");
	Avatar_AuraBack->CreateAnimationFolder("Avatar_Aura00", "Avatar_Aura00Back", 0.1f, 0, 26);
	Avatar_AuraBack->SetChangeAnimation("default");

	Avatar_AuraFront = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_AuraFront->GetTransform()->SetLocalPosition(float4(0.f, 50.f, -0.04f));
	Avatar_AuraFront->SetRenderingPipeLine("TextureTrans");
	Avatar_AuraFront->CreateAnimationFolder("Avatar_Aura00", "Avatar_Aura00Front", 0.1f, 0, 26);
	Avatar_AuraFront->SetChangeAnimation("default");
}

void Avatar::Update(float _DeltaTime)
{
	std::vector<GameEngineImageRenderer*>::iterator Iter = List_.begin();
	std::vector<GameEngineImageRenderer*>::iterator IterEnd = List_.end();

	for(; Iter != IterEnd; Iter++)
	{
		(*Iter)->IsCurrentAnimationIndex(Index_);
	}
}

void Avatar::Weapon()
{
	Weapon_Front1 = CreateTransformComponent<GameEngineImageRenderer>();
	Weapon_Front1->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.1f));
	Weapon_Front1->CreateAnimationFolder("default", "sswd0000c", FLT_MAX, 0, 241);
	Weapon_Front1->CreateAnimationFolder("Weapon01", "beamswd00c1", FLT_MAX, 0, 241);
	Weapon_Front1->CreateAnimationFolder("Weapon02", "beamswd59c1", FLT_MAX, 0, 241);
	Weapon_Front1->SetChangeAnimation("default");
	List_.push_back(Weapon_Front1);

	Weapon_Front2 = CreateTransformComponent<GameEngineImageRenderer>();
	Weapon_Front2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.2f));
	Weapon_Front2->CreateAnimationFolder("Weapon01", "beamswd00c2", FLT_MAX, 0, 241);
	Weapon_Front2->CreateAnimationFolder("Weapon02", "beamswd59c2", FLT_MAX, 0, 241);
	Weapon_Front2->SetChangeAnimation("default");
	List_.push_back(Weapon_Front2);

	Weapon_Back1 = CreateTransformComponent<GameEngineImageRenderer>();
	Weapon_Back1->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.2f));
	Weapon_Back1->CreateAnimationFolder("default", "sswd0000b", FLT_MAX, 0, 241);
	Weapon_Back1->CreateAnimationFolder("Weapon01", "beamswd00b1", FLT_MAX, 0, 241);
	Weapon_Back1->CreateAnimationFolder("Weapon02", "beamswd59b1", FLT_MAX, 0, 241);
	Weapon_Back1->SetChangeAnimation("default");
	List_.push_back(Weapon_Back1);

	Weapon_Back2 = CreateTransformComponent<GameEngineImageRenderer>(1);
	Weapon_Back2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.1f));
	Weapon_Back2->CreateAnimationFolder("Weapon01", "beamswd00b2", FLT_MAX, 0, 241);
	Weapon_Back2->CreateAnimationFolder("Weapon02", "beamswd59b2", FLT_MAX, 0, 241);
	Weapon_Back2->SetChangeAnimation("default");
	List_.push_back(Weapon_Back2);
}

void Avatar::Avatar_Belt()
{
	Avatar_BeltA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_BeltA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_BeltA->CreateAnimationFolder("Avatar_Belt02", "Avatar_Belt02A", FLT_MAX, 0, 241);
	Avatar_BeltA->SetChangeAnimation("default");
	List_.push_back(Avatar_BeltA);

	Avatar_BeltC = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_BeltC->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.02f));
	Avatar_BeltC->CreateAnimationFolder("Avatar_Belt01", "Avatar_Belt01C", FLT_MAX, 0, 241);
	Avatar_BeltC->SetChangeAnimation("default");
	List_.push_back(Avatar_BeltC);

	Avatar_BeltD = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_BeltD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_BeltD->CreateAnimationFolder("Avatar_Belt01", "Avatar_Belt01D", FLT_MAX, 0, 241);
	Avatar_BeltD->CreateAnimationFolder("Avatar_Belt02", "Avatar_Belt02D", FLT_MAX, 0, 241);
	Avatar_BeltD->SetChangeAnimation("default");
	List_.push_back(Avatar_BeltD);
}

void Avatar::Avatar_Cap()
{
	Avatar_CapA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CapA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_CapA->CreateAnimationFolder("Avatar_Cap01", "Avatar_Cap01A", FLT_MAX, 0, 241);
	Avatar_CapA->CreateAnimationFolder("Avatar_Cap02", "Avatar_Cap02A", FLT_MAX, 0, 241);
	Avatar_CapA->SetChangeAnimation("default");
	List_.push_back(Avatar_CapA);

	Avatar_CapB = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CapB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_CapB->CreateAnimationFolder("Avatar_Cap01", "Avatar_Cap01B", FLT_MAX, 0, 241);
	Avatar_CapB->CreateAnimationFolder("Avatar_Cap02", "Avatar_Cap02B", FLT_MAX, 0, 241);
	Avatar_CapB->SetChangeAnimation("default");
	List_.push_back(Avatar_CapB);
}

void Avatar::Avatar_Coat()
{
	Avatar_CoatA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CoatA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_CoatA->CreateAnimationFolder("default", "Avatar_Coat00A", FLT_MAX, 0, 241);
	Avatar_CoatA->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01A", FLT_MAX, 0, 241);
	Avatar_CoatA->CreateAnimationFolder("Avatar_Coat02", "Avatar_Coat02A", FLT_MAX, 0, 241);
	Avatar_CoatA->SetChangeAnimation("default");
	List_.push_back(Avatar_CoatA);

	Avatar_CoatB = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CoatB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_CoatB->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01B", FLT_MAX, 0, 241);
	Avatar_CoatB->CreateAnimationFolder("Avatar_Coat02", "Avatar_Coat02B", FLT_MAX, 0, 241);
	Avatar_CoatB->SetChangeAnimation("default");
	List_.push_back(Avatar_CoatB);

	Avatar_CoatC = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CoatC->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.02f));
	Avatar_CoatC->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat02C", FLT_MAX, 0, 241);
	Avatar_CoatC->CreateAnimationFolder("Avatar_Coat02", "Avatar_Coat02C", FLT_MAX, 0, 241);
	Avatar_CoatC->SetChangeAnimation("default");
	List_.push_back(Avatar_CoatC);

	Avatar_CoatD = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CoatD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_CoatD->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01D", FLT_MAX, 0, 241);
	Avatar_CoatD->SetChangeAnimation("default");
	List_.push_back(Avatar_CoatD);

	Avatar_CoatX = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_CoatX->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.05f));
	Avatar_CoatX->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01X", FLT_MAX, 0, 241);
	Avatar_CoatX->SetChangeAnimation("default");
	List_.push_back(Avatar_CoatX);
}

void Avatar::Avatar_Face()
{
	Avatar_FaceB = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_FaceB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_FaceB->CreateAnimationFolder("Avatar_Face01", "Avatar_Face01B", FLT_MAX, 0, 241);
	Avatar_FaceB->CreateAnimationFolder("Avatar_Face02", "Avatar_Face02B", FLT_MAX, 0, 241);
	Avatar_FaceB->SetChangeAnimation("default");
	List_.push_back(Avatar_FaceB);
}

void Avatar::Avatar_Hair()
{
	Avatar_HairA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_HairA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_HairA->CreateAnimationFolder("default", "Avatar_Hair00A", FLT_MAX, 0, 241);
	Avatar_HairA->CreateAnimationFolder("Avatar_Hair01", "Avatar_Hair01A", FLT_MAX, 0, 241);
	Avatar_HairA->CreateAnimationFolder("Avatar_Hair02", "Avatar_Hair02A", FLT_MAX, 0, 241);
	Avatar_HairA->SetChangeAnimation("default");
	List_.push_back(Avatar_HairA);

	Avatar_HairD = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_HairD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_HairD->CreateAnimationFolder("Avatar_Hair02", "Avatar_Hair02D", FLT_MAX, 0, 241);
	Avatar_HairD->SetChangeAnimation("default");
	List_.push_back(Avatar_HairD);
}

void Avatar::Avatar_Neck()
{
	Avatar_NeckA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_NeckA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_NeckA->CreateAnimationFolder("Avatar_Neck02", "Avatar_Neck02A", FLT_MAX, 0, 241);
	Avatar_NeckA->SetChangeAnimation("default");
	List_.push_back(Avatar_NeckA);

	Avatar_NeckC = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_NeckC->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.02f));
	Avatar_NeckC->CreateAnimationFolder("Avatar_Neck01", "Avatar_Neck01C", FLT_MAX, 0, 241);
	Avatar_NeckC->SetChangeAnimation("default");
	List_.push_back(Avatar_NeckC);

	Avatar_NeckD = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_NeckD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_NeckD->CreateAnimationFolder("Avatar_Neck01", "Avatar_Neck01D", FLT_MAX, 0, 241);
	Avatar_NeckD->SetChangeAnimation("default");
	List_.push_back(Avatar_NeckD);

	Avatar_NeckX = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_NeckX->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.05f));
	Avatar_NeckX->CreateAnimationFolder("Avatar_Neck01", "Avatar_Neck01X", FLT_MAX, 0, 241);
	Avatar_NeckX->SetChangeAnimation("default");
	List_.push_back(Avatar_NeckX);
}

void Avatar::Avatar_Pants()
{
	Avatar_PantsA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_PantsA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_PantsA->CreateAnimationFolder("default", "Avatar_Pants00A", FLT_MAX, 0, 241);
	Avatar_PantsA->CreateAnimationFolder("Avatar_Pants01", "Avatar_Pants01A", FLT_MAX, 0, 241);
	Avatar_PantsA->CreateAnimationFolder("Avatar_Pants02", "Avatar_Pants02A", FLT_MAX, 0, 241);
	Avatar_PantsA->SetChangeAnimation("default");
	List_.push_back(Avatar_PantsA);

	Avatar_PantsB = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_PantsB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_PantsB->CreateAnimationFolder("default", "Avatar_Pants00B", FLT_MAX, 0, 241);
	Avatar_PantsB->CreateAnimationFolder("Avatar_Pants01", "Avatar_Pants01B", FLT_MAX, 0, 241);
	Avatar_PantsB->CreateAnimationFolder("Avatar_Pants02", "Avatar_Pants02B", FLT_MAX, 0, 241);
	Avatar_PantsB->SetChangeAnimation("default");
	List_.push_back(Avatar_PantsB);
}

void Avatar::Avatar_Shoes()
{
	Avatar_ShoesA = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_ShoesA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_ShoesA->CreateAnimationFolder("default", "Avatar_Shoes00A", FLT_MAX, 0, 241);
	Avatar_ShoesA->CreateAnimationFolder("Avatar_Shoes01", "Avatar_Shoes01A", FLT_MAX, 0, 241);
	Avatar_ShoesA->CreateAnimationFolder("Avatar_Shoes02", "Avatar_Shoes02A", FLT_MAX, 0, 241);
	Avatar_ShoesA->SetChangeAnimation("default");
	List_.push_back(Avatar_ShoesA);

	Avatar_ShoesB = CreateTransformComponent<GameEngineImageRenderer>();
	Avatar_ShoesB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_ShoesB->CreateAnimationFolder("default", "Avatar_Shoes00B", FLT_MAX, 0, 241);
	Avatar_ShoesB->CreateAnimationFolder("Avatar_Shoes01", "Avatar_Shoes01B", FLT_MAX, 0, 241);
	Avatar_ShoesB->CreateAnimationFolder("Avatar_Shoes02", "Avatar_Shoes02B", FLT_MAX, 0, 241);
	Avatar_ShoesB->SetChangeAnimation("default");
	List_.push_back(Avatar_ShoesB);
}

void Avatar::ChangeAvatar(const std::string& _Name, ItemType _Type)
{
	switch (_Type)
	{
	case ItemType::Weapon:
		Weapon_Front1->SetChangeAnimation(_Name, Index_);
		Weapon_Front2->SetChangeAnimation(_Name, Index_);
		Weapon_Back1->SetChangeAnimation(_Name, Index_);
		Weapon_Back2->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Title:
		Title->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Hair:
		Avatar_HairA->SetChangeAnimation(_Name, Index_);
		Avatar_HairD->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Cap:
		Avatar_CapA->SetChangeAnimation(_Name, Index_);
		Avatar_CapB->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Face:
		Avatar_FaceB->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Aura:
		if (true == GameManager::GetInst().GetPlayer()->GetIsDungeon())
		{
			return;
		}

		Avatar_AuraBack->SetChangeAnimation(_Name);
		Avatar_AuraFront->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Neck:
		Avatar_NeckA->SetChangeAnimation(_Name, Index_);
		Avatar_NeckC->SetChangeAnimation(_Name, Index_);
		Avatar_NeckD->SetChangeAnimation(_Name, Index_);
		Avatar_NeckX->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Coat:
		Avatar_CoatA->SetChangeAnimation(_Name, Index_);
		Avatar_CoatB->SetChangeAnimation(_Name, Index_);
		Avatar_CoatC->SetChangeAnimation(_Name, Index_);
		Avatar_CoatD->SetChangeAnimation(_Name, Index_);
		Avatar_CoatX->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Skin:
		Avatar_Skin->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Pants:
		Avatar_PantsA->SetChangeAnimation(_Name, Index_);
		Avatar_PantsB->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Belt:
		Avatar_BeltA->SetChangeAnimation(_Name, Index_);
		Avatar_BeltC->SetChangeAnimation(_Name, Index_);
		Avatar_BeltD->SetChangeAnimation(_Name, Index_);
		break;
	case ItemType::Avatar_Shoes:
		Avatar_ShoesA->SetChangeAnimation(_Name, Index_);
		Avatar_ShoesB->SetChangeAnimation(_Name, Index_);
		break;
	}
}

void Avatar::SetReverse(bool _Reverse)
{
	std::vector<GameEngineImageRenderer*>::iterator Iter = List_.begin();
	std::vector<GameEngineImageRenderer*>::iterator IterEnd = List_.end();

	for (; Iter != IterEnd; Iter++)
	{
		(*Iter)->SetReverse(_Reverse);
	}

	float Pivot;

	if (true == _Reverse)
	{
		Pivot = -15.f;
	}
	else
	{
		Pivot = 15.f;
	}

	Iter = List_.begin();

	for (; Iter != IterEnd; Iter++)
	{
		(*Iter)->GetTransform()->SetLocalPosition(float4(Pivot, 75.f));
	}
}

void Avatar::SetRotation(float4 _Value)
{
	std::vector<GameEngineImageRenderer*>::iterator Iter = List_.begin();
	std::vector<GameEngineImageRenderer*>::iterator IterEnd = List_.end();

	for (; Iter != IterEnd; Iter++)
	{
		(*Iter)->GetTransform()->SetLocalRotationDegree(_Value);
	}
}
