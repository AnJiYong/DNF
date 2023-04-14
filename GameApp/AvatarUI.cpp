#include "Precompile.h"
#include "AvatarUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineProgressBarRenderer.h"

AvatarUI::AvatarUI()
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
{
}

AvatarUI::~AvatarUI()
{
}

void AvatarUI::Start()
{
	Avatar_Skin = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_Skin->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));
	Avatar_Skin->CreateAnimationFolder("default", "Avatar_Skin00", FLT_MAX, 176, 176);
	Avatar_Skin->CreateAnimationFolder("Avatar_Skin01", "Avatar_Skin01", FLT_MAX, 176, 176);
	Avatar_Skin->CreateAnimationFolder("Avatar_Skin02", "Avatar_Skin02", FLT_MAX, 176, 176);
	Avatar_Skin->SetChangeAnimation("default");

	Avatar_Shoes();
	Avatar_Pants();
	Avatar_Coat();
	Avatar_Belt();
	Avatar_Face();
	Avatar_Neck();
	Avatar_Hair();
	Avatar_Cap();

	Weapon_Front1 = CreateTransformComponent<GameEngineProgressBarRenderer>();
	Weapon_Front1->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.1f));
	Weapon_Front1->CreateAnimationFolder("default", "sswd0000c", FLT_MAX, 176, 176);
	Weapon_Front1->CreateAnimationFolder("Weapon01", "beamswd00c1", FLT_MAX, 176, 176);
	Weapon_Front1->CreateAnimationFolder("Weapon02", "beamswd59c1", FLT_MAX, 176, 176);
	Weapon_Front1->SetChangeAnimation("default");
	Weapon_Front1->SetPercent(0.7f);

	Weapon_Front2 = CreateTransformComponent<GameEngineProgressBarRenderer>();
	Weapon_Front2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.2f));
	Weapon_Front2->CreateAnimationFolder("Weapon01", "beamswd00c2", FLT_MAX, 176, 176);
	Weapon_Front2->CreateAnimationFolder("Weapon02", "beamswd59c2", FLT_MAX, 176, 176);
	Weapon_Front2->SetChangeAnimation("default");
	Weapon_Front2->SetPercent(0.7f);

	Weapon_Back1 = CreateTransformComponent<GameEngineProgressBarRenderer>();
	Weapon_Back1->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.2f));
	Weapon_Back1->CreateAnimationFolder("default", "sswd0000b", FLT_MAX, 176, 176);
	Weapon_Back1->CreateAnimationFolder("Weapon01", "beamswd00b1", FLT_MAX, 176, 176);
	Weapon_Back1->CreateAnimationFolder("Weapon02", "beamswd59b1", FLT_MAX, 176, 176);
	Weapon_Back1->SetChangeAnimation("default");
	Weapon_Back1->SetPercent(0.7f);

	Weapon_Back2 = CreateTransformComponent<GameEngineProgressBarRenderer>(1);
	Weapon_Back2->GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.1f));
	Weapon_Back2->CreateAnimationFolder("Weapon01", "beamswd00b2", FLT_MAX, 176, 176);
	Weapon_Back2->CreateAnimationFolder("Weapon02", "beamswd59b2", FLT_MAX, 176, 176);
	Weapon_Back2->SetChangeAnimation("default");
	Weapon_Back2->SetPercent(0.7f);
}

void AvatarUI::Update(float _DeltaTime)
{

}

void AvatarUI::Avatar_Belt()
{
	Avatar_BeltA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_BeltA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_BeltA->CreateAnimationFolder("Avatar_Belt02", "Avatar_Belt02A", FLT_MAX, 176, 176);
	Avatar_BeltA->SetChangeAnimation("default");

	Avatar_BeltC = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_BeltC->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.02f));
	Avatar_BeltC->CreateAnimationFolder("Avatar_Belt01", "Avatar_Belt01C", FLT_MAX, 176, 176);
	Avatar_BeltC->SetChangeAnimation("default");

	Avatar_BeltD = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_BeltD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_BeltD->CreateAnimationFolder("Avatar_Belt01", "Avatar_Belt01D", FLT_MAX, 176, 176);
	Avatar_BeltD->CreateAnimationFolder("Avatar_Belt02", "Avatar_Belt02D", FLT_MAX, 176, 176);
	Avatar_BeltD->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Cap()
{
	Avatar_CapA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CapA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_CapA->CreateAnimationFolder("Avatar_Cap01", "Avatar_Cap01A", FLT_MAX, 176, 176);
	Avatar_CapA->CreateAnimationFolder("Avatar_Cap02", "Avatar_Cap02A", FLT_MAX, 176, 176);
	Avatar_CapA->SetChangeAnimation("default");

	Avatar_CapB = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CapB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_CapB->CreateAnimationFolder("Avatar_Cap01", "Avatar_Cap01B", FLT_MAX, 176, 176);
	Avatar_CapB->CreateAnimationFolder("Avatar_Cap02", "Avatar_Cap02B", FLT_MAX, 176, 176);
	Avatar_CapB->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Coat()
{
	Avatar_CoatA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CoatA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_CoatA->CreateAnimationFolder("default", "Avatar_Coat00A", FLT_MAX, 176, 176);
	Avatar_CoatA->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01A", FLT_MAX, 176, 176);
	Avatar_CoatA->CreateAnimationFolder("Avatar_Coat02", "Avatar_Coat02A", FLT_MAX, 176, 176);
	Avatar_CoatA->SetChangeAnimation("default");

	Avatar_CoatB = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CoatB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_CoatB->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01B", FLT_MAX, 176, 176);
	Avatar_CoatB->CreateAnimationFolder("Avatar_Coat02", "Avatar_Coat02B", FLT_MAX, 176, 176);
	Avatar_CoatB->SetChangeAnimation("default");

	Avatar_CoatC = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CoatC->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.02f));
	Avatar_CoatC->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat02C", FLT_MAX, 176, 176);
	Avatar_CoatC->CreateAnimationFolder("Avatar_Coat02", "Avatar_Coat02C", FLT_MAX, 176, 176);
	Avatar_CoatC->SetChangeAnimation("default");

	Avatar_CoatD = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CoatD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_CoatD->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01D", FLT_MAX, 176, 176);
	Avatar_CoatD->SetChangeAnimation("default");

	Avatar_CoatX = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_CoatX->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.05f));
	Avatar_CoatX->CreateAnimationFolder("Avatar_Coat01", "Avatar_Coat01X", FLT_MAX, 176, 176);
	Avatar_CoatX->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Face()
{
	Avatar_FaceB = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_FaceB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_FaceB->CreateAnimationFolder("Avatar_Face01", "Avatar_Face01B", FLT_MAX, 176, 176);
	Avatar_FaceB->CreateAnimationFolder("Avatar_Face02", "Avatar_Face02B", FLT_MAX, 176, 176);
	Avatar_FaceB->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Hair()
{
	Avatar_HairA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_HairA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_HairA->CreateAnimationFolder("default", "Avatar_Hair00A", FLT_MAX, 176, 176);
	Avatar_HairA->CreateAnimationFolder("Avatar_Hair01", "Avatar_Hair01A", FLT_MAX, 176, 176);
	Avatar_HairA->CreateAnimationFolder("Avatar_Hair02", "Avatar_Hair02A", FLT_MAX, 176, 176);
	Avatar_HairA->SetChangeAnimation("default");

	Avatar_HairD = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_HairD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_HairD->CreateAnimationFolder("Avatar_Hair02", "Avatar_Hair02D", FLT_MAX, 176, 176);
	Avatar_HairD->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Neck()
{
	Avatar_NeckA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_NeckA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_NeckA->CreateAnimationFolder("Avatar_Neck02", "Avatar_Neck02A", FLT_MAX, 176, 176);
	Avatar_NeckA->SetChangeAnimation("default");

	Avatar_NeckC = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_NeckC->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.02f));
	Avatar_NeckC->CreateAnimationFolder("Avatar_Neck01", "Avatar_Neck01C", FLT_MAX, 176, 176);
	Avatar_NeckC->SetChangeAnimation("default");

	Avatar_NeckD = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_NeckD->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.01f));
	Avatar_NeckD->CreateAnimationFolder("Avatar_Neck01", "Avatar_Neck01D", FLT_MAX, 176, 176);
	Avatar_NeckD->SetChangeAnimation("default");

	Avatar_NeckX = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_NeckX->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.05f));
	Avatar_NeckX->CreateAnimationFolder("Avatar_Neck01", "Avatar_Neck01X", FLT_MAX, 176, 176);
	Avatar_NeckX->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Pants()
{
	Avatar_PantsA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_PantsA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_PantsA->CreateAnimationFolder("default", "Avatar_Pants00A", FLT_MAX, 176, 176);
	Avatar_PantsA->CreateAnimationFolder("Avatar_Pants01", "Avatar_Pants01A", FLT_MAX, 176, 176);
	Avatar_PantsA->CreateAnimationFolder("Avatar_Pants02", "Avatar_Pants02A", FLT_MAX, 176, 176);
	Avatar_PantsA->SetChangeAnimation("default");

	Avatar_PantsB = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_PantsB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_PantsB->CreateAnimationFolder("default", "Avatar_Pants00B", FLT_MAX, 176, 176);
	Avatar_PantsB->CreateAnimationFolder("Avatar_Pants01", "Avatar_Pants01B", FLT_MAX, 176, 176);
	Avatar_PantsB->CreateAnimationFolder("Avatar_Pants02", "Avatar_Pants02B", FLT_MAX, 176, 176);
	Avatar_PantsB->SetChangeAnimation("default");
}

void AvatarUI::Avatar_Shoes()
{
	Avatar_ShoesA = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_ShoesA->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.04f));
	Avatar_ShoesA->CreateAnimationFolder("default", "Avatar_Shoes00A", FLT_MAX, 176, 176);
	Avatar_ShoesA->CreateAnimationFolder("Avatar_Shoes01", "Avatar_Shoes01A", FLT_MAX, 176, 176);
	Avatar_ShoesA->CreateAnimationFolder("Avatar_Shoes02", "Avatar_Shoes02A", FLT_MAX, 176, 176);
	Avatar_ShoesA->SetChangeAnimation("default");

	Avatar_ShoesB = CreateTransformComponent<GameEngineUIRenderer>();
	Avatar_ShoesB->GetTransform()->SetLocalPosition(float4(0.f, 0.f, -0.03f));
	Avatar_ShoesB->CreateAnimationFolder("default", "Avatar_Shoes00B", FLT_MAX, 176, 176);
	Avatar_ShoesB->CreateAnimationFolder("Avatar_Shoes01", "Avatar_Shoes01B", FLT_MAX, 176, 176);
	Avatar_ShoesB->CreateAnimationFolder("Avatar_Shoes02", "Avatar_Shoes02B", FLT_MAX, 176, 176);
	Avatar_ShoesB->SetChangeAnimation("default");
}

void AvatarUI::ChangeAvatar(const std::string& _Name, ItemType _Type)
{
	switch (_Type)
	{
	case ItemType::Weapon:
		Weapon_Front1->SetChangeAnimation(_Name);
		Weapon_Front2->SetChangeAnimation(_Name);
		Weapon_Back1->SetChangeAnimation(_Name);
		Weapon_Back2->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Hair:
		Avatar_HairA->SetChangeAnimation(_Name);
		Avatar_HairD->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Cap:
		Avatar_CapA->SetChangeAnimation(_Name);
		Avatar_CapB->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Face:
		Avatar_FaceB->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Aura:
		break;
	case ItemType::Avatar_Neck:
		Avatar_NeckA->SetChangeAnimation(_Name);
		Avatar_NeckC->SetChangeAnimation(_Name);
		Avatar_NeckD->SetChangeAnimation(_Name);
		Avatar_NeckX->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Coat:
		Avatar_CoatA->SetChangeAnimation(_Name);
		Avatar_CoatB->SetChangeAnimation(_Name);
		Avatar_CoatC->SetChangeAnimation(_Name);
		Avatar_CoatD->SetChangeAnimation(_Name);
		Avatar_CoatX->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Skin:
		Avatar_Skin->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Pants:
		Avatar_PantsA->SetChangeAnimation(_Name);
		Avatar_PantsB->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Belt:
		Avatar_BeltA->SetChangeAnimation(_Name);
		Avatar_BeltC->SetChangeAnimation(_Name);
		Avatar_BeltD->SetChangeAnimation(_Name);
		break;
	case ItemType::Avatar_Shoes:
		Avatar_ShoesA->SetChangeAnimation(_Name);
		Avatar_ShoesB->SetChangeAnimation(_Name);
		break;
	}
}