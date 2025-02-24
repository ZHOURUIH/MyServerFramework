#include "GameHeader.h"

CharacterPlayer::CharacterPlayer()
{
	mPlayerData = new CharacterPlayerData();
	mPlayerData->mPlayer = this;
	mData = mPlayerData;
}

CharacterPlayer::~CharacterPlayer()
{
	delete mPlayerData;
	mPlayerData = nullptr;
}

void CharacterPlayer::initComponents()
{
	base::initComponents();
	// 初始化默认组件
	mComHPRecover = addComponent<COMCharacterHPRecover>();
	mComGameSetting = addComponent<COMPlayerGameSetting>();
	mComGhostFollow = addComponent<COMPlayerGhostFollow>();
	mComCrime = addComponent<COMPlayerCrime>();
	mComMPRecover = addComponent<COMCharacterMPRecover>();
	mComSpeedCheck = addComponent<COMPlayerSpeedCheck>();
	mComBaseProperty = addComponentAtIndex<COMPlayerBaseProperty>();
	mComAddup = addComponentAtIndex<COMPlayerAddupData>();
	mComEquip = addComponentAtIndex<COMPlayerEquip>();
	mComPack = addComponentAtIndex<COMPlayerPack>();
	mComSkill = addComponentAtIndex<COMPlayerSkill>();
	mComEquipCollection = addComponent<COMPlayerEquipCollection>();
	mComGameAddupData = addComponent<COMPlayerGameAddupData>();
	mComOfferReward = addComponent<COMPlayerOfferReward>();
	mComPrivateChat = addComponent<COMPlayerPrivateChat>();
	mComBatchNotify = addComponent<COMPlayerBatchNotify>();
	mComAchivement = addComponent<COMPlayerAchivement>();
	mComWarehouse = addComponent<COMPlayerWarehouse>();
	mComBlackList = addComponent<COMPlayerBlackList>();
	mComGamePlay = addComponent<COMPlayerGamePlay>();
	mComMission = addComponent<COMPlayerMission>();
	mComAuction = addComponent<COMPlayerAuction>();
	mComFriend = addComponent<COMPlayerFriend>();
	mComGuild = addComponent<COMPlayerGuild>();
	mComTeam = addComponent<COMPlayerTeam>();
	mComMail = addComponent<COMPlayerMail>();
}

void CharacterPlayer::initBuff()
{
	if (mComponentTypeList != nullptr)
	{
		for (GameComponent* com : *mComponentTypeList)
		{
			if (auto* playerCom = dynamic_cast<COMPlayer*>(com))
			{
				playerCom->initBuff();
			}
		}
	}
}

void CharacterPlayer::update(const float elapsedTime)
{
	base::update(elapsedTime);

	if (mData->mPropertyDirty)
	{
		mPlayerData->refreshPlayerProperty();
	}

	// 角色属性有改变时,通知客户端
	if (mPlayerData->mPropertyDirtyFlag > 0)
	{
		SCCharacterProperty::send(this, mPlayerData->mPropertyDirtyFlag);
		mPlayerData->mPropertyDirtyFlag = 0;
	}
	if (mPlayerData->mPackChanged)
	{
		applyPackChange();
	}

	// 在最后同步上一次的血量,确保在其他组件中访问是正常的状态
	if (mPlayerData->mLastHP != mPlayerData->mHP)
	{
		mPlayerData->mLastHP = mPlayerData->mHP;
	}
}

void CharacterPlayer::applyPackChange()
{
	if (mPlayerData->mPackOnlyPropertyChanged)
	{
		SCPackItem::send(this);
	}
	else
	{
		const auto& itemList = mComPack->getItems();
		const auto& quickItemList = mComPack->getQuickItems();
		SCPackItem::send(this, itemList, quickItemList, mComPack->getSize());
		// 背包数据写入数据库,包括快捷栏
		MySQLUtility::writePackItem(getGUID(), LLsToS(itemList));
		MySQLUtility::writeQuickPackItem(getGUID(), LLsToS(quickItemList));
	}
	mPlayerData->mPackChanged = false;
	mPlayerData->mPackOnlyPropertyChanged = false;
}

inline void CharacterPlayer::refreshPlayerAllProperty()
{
	mPlayerData->refreshPlayerProperty();
	markAllPropertySyncDirty();
}

void CharacterPlayer::checkEquipSkillBuff(bool isFromDead)
{
	if (mComponentTypeList != nullptr)
	{
		for (GameComponent* com : *mComponentTypeList)
		{
			if (auto* playerCom = dynamic_cast<COMPlayer*>(com))
			{
				playerCom->checkBuff(isFromDead);
			}
		}
	}
}

CharacterSkill* CharacterPlayer::getSkill(const int skillID)
{
	return mComSkill->getSkill(skillID);
}

ObjectItemEquip* CharacterPlayer::getEquipInPackOrDress(const llong instanceID) const
{
	// 先从身上已穿戴的装备中查找
	if (ObjectItemEquip* equip = mComEquip->getEquip(instanceID))
	{
		return equip;
	}
	// 再从背包中查找
	ObjectItem* item = mComPack->getItem(instanceID);
	if (item == nullptr || item->getType() != OBJECT_ITEM::EQUIP)
	{
		return nullptr;
	}
	return toEquip(item);
}

void CharacterPlayer::addPropertyValue(const CHARACTER_PROPERTY type, const float value)
{
	base::addPropertyValue(type, value);
	markPropertySyncDirty(type);
}

void CharacterPlayer::addPropertyPercent(const CHARACTER_PROPERTY type, const float percent)
{
	base::addPropertyPercent(type, percent);
	markPropertySyncDirty(type);
}

void CharacterPlayer::resetProperty()
{
	base::resetProperty();
	mPlayerData->resetProperty();
	mComEquipCollection = nullptr;
	mComGameAddupData = nullptr;
	mComBaseProperty = nullptr;
	mComGameSetting = nullptr;
	mComGhostFollow = nullptr;
	mComSpeedCheck = nullptr;
	mComOfferReward = nullptr;
	mComPrivateChat = nullptr;
	mComBatchNotify = nullptr;
	mComAchivement = nullptr;
	mComWarehouse = nullptr;
	mComBlackList = nullptr;
	mComAddup = nullptr;
	mComGamePlay = nullptr;
	mComMission = nullptr;
	mComFriend = nullptr;
	mComAuction = nullptr;
	mComGuild = nullptr;
	mComCrime = nullptr;
	mComEquip = nullptr;
	mComSkill = nullptr;
	mComTeam = nullptr;
	mComMail = nullptr;
	mComPack = nullptr;
}

inline bool CharacterPlayer::check(const bool value, const ushort errorCode) const
{
	if (value)
	{
		SCErrorCode::send(this, errorCode);
	}
	return value;
}

inline bool CharacterPlayer::check(const ushort errorCode) const
{
	if (errorCode != 0)
	{
		SCErrorCode::send(this, errorCode);
	}
	return errorCode != 0;
}