#include "Precompile.h"
#include "MonsterBase.h"
#include "DamageFont.h"
#include "GameManager.h"
#include "Player.h"
#include "GlobalEnum.h"
#include "TopUI.h"
#include "MapBase.h"

#include "GameEngine\GameEngineCollision.h"
#include "GameEngine\GameEngineImageRenderer.h"
#include "GameEngineBase\GameEngineSoundPlayer.h"
#include "GameEngineBase\GameEngineSoundManager.h"

MonsterBase::MonsterBase()
	: BodyCollision_(nullptr)
	, TraceCollision_(nullptr)
	, AttackCollision_(nullptr)
	, Sound_(nullptr)
	, CurMap_(nullptr)
	, MonsterNumber_(0)
	, TargetCheck_(false)
	, PlayerPos_(float4::ZERO)
	, PrevPos_(float4::ZERO)
	, State_()
	, DirLeft_(false)
	, DirLock_(false)
	, IsBoss_(false)
	, Invincibility_(false)
	, SuperArmor_(false)
	, IsHitAnimation_(false)
	, CriticalHit_(false)
	, ZPos_(0.f)
	, ZSize_(100.f)
	, DeltaTime_(0.f)
	, MonsterName_("")
	, SoundCount_(0)
{
}

MonsterBase::~MonsterBase()
{
}

void MonsterBase::Start()
{
	BodyCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::Monster));
	TraceCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::MonsterTrace));
	AttackCollision_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionOrder::MonsterAttack));

	Sound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();

	State_.HP_ = 500;
	State_.AttackTime_ = 0.f;
	State_.MoveTime_ = 0.f;
	State_.IdleTime_ = 0.f;
	State_.Damage_ = 200;
}

void MonsterBase::Update(float _DeltaTime)
{
	PlayerPos_ = GameManager::GetInst().GetPlayerPos();
	DeltaTime_ = GameEngineTime::GetInst().GetDeltaTime();

#ifdef _DEBUG
	GetLevel()->PushDebugRender(BodyCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushDebugRender(TraceCollision_->GetTransform(), CollisionType::Rect);
	GetLevel()->PushDebugRender(AttackCollision_->GetTransform(), CollisionType::Rect);
#endif // _DEBUG

	GetTransform()->SetWorldPositionZ(BodyCollision_->GetTransform()->GetWorldPosition().y);

	TraceCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&MonsterBase::Attack, this));

	AttackCollision_->Collision(CollisionType::Rect, CollisionType::Rect, static_cast<int>(CollisionOrder::Player)
		, std::bind(&MonsterBase::Damage, this, std::placeholders::_1), CollisionCheck::Begin);

	State_.AttackTime_ -= _DeltaTime;
}

void MonsterBase::Damage(GameEngineCollision* _Other)
{
	if (GameManager::GetInst().GetPlayerZPos() >= ZSize_)
	{
		return;
	}

	int Damage = rand() % 201;
	Damage += 900;
	Damage *= State_.Damage_;
	Damage /= 100;

	Player* player = GameManager::GetInst().GetPlayer();

	if (player->BodyCollision_->GetTransform()->GetWorldPosition().x <= BodyCollision_->GetTransform()->GetWorldPosition().x)
	{
		player->Hit(500, false);
	}
	else
	{
		player->Hit(500, true);
	}

	AttackCollision_->GetTransform()->SetLocalPosition(float4(10000.f, 10000.f));
}

void MonsterBase::HitDamage(AttackInfo _AttackInfo, PlayerInfo _PlayerInfo)
{
	//if (GameManager::GetInst().GetPlayerZPos() >= ZPos_ || true == Invincibility_)
	//{
	//	return;
	//}

	if (true == Invincibility_)
	{
		return;
	}

	std::vector<AttackInfo>::iterator Iter = HitInfo_.begin();
	std::vector<AttackInfo>::iterator IterEnd = HitInfo_.end();

	for (; Iter != IterEnd; ++Iter)
	{
		if ((*Iter).GetName() == _AttackInfo.GetName() &&
			(*Iter).GetCount() == _AttackInfo.GetCount())
		{
			return;
		}
	}

	AttackInfo Info = _AttackInfo;
	HitInfo_.push_back(Info);

	int Damage = rand() % 21;
	Damage += 90;
	Damage *= _AttackInfo.GetDamage();
	Damage /= 100;

	int Critical = rand() % 101;

	if (_PlayerInfo.Critical >= static_cast<float>(Critical))
	{
		Damage += (Damage / 2);
		CriticalHit_ = true;
	}
	else
	{
		CriticalHit_ = false;
	}

	DamageFont* font = GetLevel()->CreateActor<DamageFont>();
	font->GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition() + float4(0.f, 150.f));
	font->DamageSetting(Damage, CriticalHit_);

	switch (SoundCount_)
	{
	case 0:
		Sound_->PlayOverLap("mon_sword_hit_08.ogg", 0);
		break;
	case 1:
		Sound_->PlayOverLap("mon_sword_hit_09.ogg", 0);
		break;
	case 2:
		Sound_->PlayOverLap("mon_sword_hit_10.ogg", 0);
		break;
	}

	++SoundCount_;

	if (SoundCount_ == 3)
	{
		SoundCount_ = 0;
	}

	Hit();
	State_.HP_ -= Damage;

	GameManager::GetInst().GetUI()->MonsterHPBarOn(this, IsBoss_);

	if (State_.HP_ <= 0)
	{
		MonsterDeath();
	}
}

void MonsterBase::RoomSetting(MapBase* _Map, bool _BossRoom)
{
	CurMap_ = _Map;
	CurMap_->MonsterCountPlus();

	if (true == _BossRoom)
	{
		CurMap_->IsBossRoom();
	}
}
