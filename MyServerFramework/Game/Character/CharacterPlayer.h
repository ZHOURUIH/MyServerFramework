#pragma once

#include "CharacterGame.h"
#include "CharacterPlayerData.h"

class CharacterPlayer : public CharacterGame
{
	BASE(CharacterPlayer, CharacterGame);
public:
	CharacterPlayer();
	~CharacterPlayer() override;
	void initComponents() override;
	void initBuff();
	void update(float elapsedTime) override;
	void applyPackChange();
	void refreshPlayerAllProperty();
	void checkEquipSkillBuff(bool isFromDead);
	CharacterSkill* getSkill(int skillID) override;
	ObjectItemEquip* getEquipInPackOrDress(llong instanceID) const;
	void resetProperty() override;
	void addPropertyValue(CHARACTER_PROPERTY type, float value) override;
	void addPropertyPercent(CHARACTER_PROPERTY type, float percent) override;
	void markPropertySyncDirty(CHARACTER_PROPERTY prop)		{ setBitOne(mPlayerData->mPropertyDirtyFlag, (byte)prop); }
	void markAllPropertySyncDirty()							{ mPlayerData->mPropertyDirtyFlag = 0xFFFFFFFFFFFFFFFF; }
	CharacterPlayerData* getPlayerData() const				{ return mPlayerData; }
	TCPServerClient* getClient() const						{ return mPlayerData->mClient; }
	COMPlayerBaseProperty* getComBaseProperty() const		{ return mComBaseProperty; }
	COMPlayerGameSetting* getComGameSetting() const			{ return mComGameSetting; }
	COMPlayerGhostFollow* getComGhostFollow() const			{ return mComGhostFollow; }
	COMPlayerSpeedCheck* getComSpeedCheck() const			{ return mComSpeedCheck; }
	COMPlayerAddupData* getComAddup() const					{ return mComAddup; }
	COMPlayerCrime* getComCrime() const						{ return mComCrime; }
	COMPlayerEquip* getComEquip() const						{ return mComEquip; }
	COMPlayerSkill* getComSkill() const						{ return mComSkill; }
	COMPlayerPack* getComPack() const						{ return mComPack; }
	COMPlayerEquipCollection* getComEquipCollection()const	{ return mComEquipCollection; }
	COMPlayerGameAddupData* getComGameAddupData() const		{ return mComGameAddupData; }
	COMPlayerOfferReward* getComOfferReward() const			{ return mComOfferReward; }
	COMPlayerPrivateChat* getComPrivateChat() const			{ return mComPrivateChat; }
	COMPlayerBatchNotify* getComBatchNotify() const			{ return mComBatchNotify; }
	COMPlayerAchivement* getComAchivement() const			{ return mComAchivement; }
	COMPlayerWarehouse* getComWarehouse() const				{ return mComWarehouse; }
	COMPlayerBlackList* getComBlackList() const				{ return mComBlackList; }
	COMPlayerGamePlay* getComGamePlay() const				{ return mComGamePlay; }
	COMPlayerMission* getComMission() const					{ return mComMission; }
	COMPlayerAuction* getComAuction() const					{ return mComAuction; }
	COMPlayerFriend* getComFriend() const					{ return mComFriend; }
	COMPlayerGuild* getComGuild() const						{ return mComGuild; }
	COMPlayerTeam* getComTeam() const						{ return mComTeam; }
	COMPlayerMail* getComMail() const						{ return mComMail; }
	bool check(bool value, ushort errorCode) const;
	bool check(ushort errorCode) const;
protected:
	CharacterPlayerData* mPlayerData = nullptr;				// 角色所有数据
	COMPlayerEquipCollection* mComEquipCollection = nullptr;// 装备收藏组件
	COMPlayerGameAddupData* mComGameAddupData = nullptr;	// 游戏性累计信息组件
	COMPlayerBaseProperty* mComBaseProperty = nullptr;		// 角色基础属性组件,可通过神石永久提升的属性
	COMPlayerGameSetting* mComGameSetting = nullptr;		// 设置组件
	COMPlayerGhostFollow* mComGhostFollow = nullptr;		// 幽灵状态跟随组件
	COMPlayerSpeedCheck* mComSpeedCheck = nullptr;			// 速度检测组件
	COMPlayerOfferReward* mComOfferReward = nullptr;		// 悬赏组件
	COMPlayerPrivateChat* mComPrivateChat = nullptr;		// 私聊组件
	COMPlayerBatchNotify* mComBatchNotify = nullptr;		// 合并通知消息的组件
	COMPlayerAchivement* mComAchivement = nullptr;			// 成就组件
	COMPlayerWarehouse* mComWarehouse = nullptr;			// 仓库组件
	COMPlayerAddupData* mComAddup = nullptr;				// 角色累计信息组件
	COMPlayerBlackList* mComBlackList = nullptr;			// 黑名单组件
	COMPlayerGamePlay* mComGamePlay = nullptr;				// 游戏信息组件
	COMPlayerMission* mComMission = nullptr;				// 任务组件
	COMPlayerAuction* mComAuction = nullptr;				// 拍卖组件
	COMPlayerFriend* mComFriend = nullptr;					// 好友组件
	COMPlayerGuild* mComGuild = nullptr;					// 行会组件
	COMPlayerCrime* mComCrime = nullptr;					// 罪恶值组件
	COMPlayerEquip* mComEquip = nullptr;					// 装备信息组件
	COMPlayerSkill* mComSkill = nullptr;					// 技能信息组件
	COMPlayerTeam* mComTeam = nullptr;						// 队伍组件
	COMPlayerMail* mComMail = nullptr;						// 邮件组件
	COMPlayerPack* mComPack = nullptr;						// 背包组件
};