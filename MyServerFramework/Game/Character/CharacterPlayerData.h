#pragma once

#include "CharacterGameData.h"
#include "GameEnum.h"

class CharacterPlayerData : public CharacterGameData
{
	BASE(CharacterPlayerData, CharacterGameData);
public:
	~CharacterPlayerData() override;
	void resetProperty() override;
	void refreshHumanLucky() { mHumanLucky = GD::BASE_HUMAN_LUCKY + mLifeItemHumanLucky + mExtraHumanLucky + mActivityHumanLucky + mBossKillingHumanLucky; }
	// 刷新属性值,角色初始化,更换装备,等级改变,装备属性改变,基础属性改变时调用
	void refreshPlayerProperty();
	void setPKMode(PK_MODE mode);
	bool isMapUnlock(int mapID) const { return mUnlockMap != nullptr && mUnlockMap->contains(mapID); }
	void setUnlockMap(const string& unlockList)
	{
		if (mUnlockMap == nullptr)
		{
			mUnlockMap = new Vector<int>();
		}
		SToIs(unlockList, *mUnlockMap);
	}
	void unlockMap(const int mapID)	
	{
		if (mUnlockMap == nullptr)
		{
			mUnlockMap = new Vector<int>();
		}
		mUnlockMap->addUnique(mapID);
	}
public:
	Vector<int>* mUnlockMap = nullptr;			// 已经解锁的地图ID列表
	CharacterPlayer* mPlayer = nullptr;			// 所属角色
	TCPServerClient* mClient = nullptr;			// 玩家所在客户端
	PKMode* mPKModeInstance = nullptr;			// PK模式实例
	string mSecondaryPassword;					// 二级密码
	ullong mPropertyDirtyFlag = 0;				// 这一帧改变的属性,只是为了告诉客户端需要同步哪些属性,使用位标记,属性的类型就是下标,暂定类型不超过63个
	llong mLoginTimeInWeek = 0;					// 本次登录角色的时间,用于本周累计在线计时,可能会由于跨了一周而在在线中修改此时间
	llong mLastLogout = 0;						// 上一次退出角色登录的时间
	llong mCreateTime = 0;						// 角色创建的时间
	llong mLastLogin = 0;						// 上一次登录角色的时间,不包含本次登录
	llong mLoginTime = 0;						// 本次登录角色的时间
	llong mLastSearchAuctionTime = 0;			// 上一次搜索拍卖品的时间,用于限制不允许频繁进行搜索
	llong mLastSecondaryPasswordTime = 0;		// 上一次验证二级密码的时间,用于限制频率,单位秒
	llong mLastReceiveGuildSalaryTime = 0;		// 上一次领取行会酬劳的时间,单位秒
	llong mPayExpiration = 0;					// 赞助的过期时间,0表示未赞助
	llong mExp = 0;								// 当前经验值
	llong mMaxExp = 0;							// 当前等级升级所需的经验
	llong mLastWriteExp = 0;					// 上一次写入数据库的经验值
	float mHumanLucky = 0.0f;					// 总的人品值
	float mLifeItemHumanLucky = 0.0f;			// 本命物品带来的人品值
	float mExtraHumanLucky = 0.0f;				// 临时增加的人品值
	float mActivityHumanLucky = 0.0f;			// 活动增加的人品值
	float mBossKillingHumanLucky = 0.0f;		// 击杀boss增加的人品值
	float mRealtimeSpeed = 0.0f;				// 当前实时的移动速度,由客户端发送过来的
	float mDigMineSpeed = 0.0f;					// 挖矿速度
	float mMultiMonsterExp = 1.0f;				// 获得的怪物经验的倍数
	float mMultiSkillExp = 1.0f;				// 获得的技能经验的倍数
	float mMultiPickGold = 1.0f;				// 拾取金币的倍数
	float mMultiPickGoldIngot = 1.0f;			// 拾取元宝的倍数
	float mSellGoldDecrease = 0.0f;				// 出售或者分解物品时获得的收益减少百分比
	float mMineTime = 0.0f;						// 剩余的挖矿时间,秒
	float mCrime = 0.0f;						// 罪恶值
	int mAccountGUID = 0;						// 账号ID
	int mQuickSellCount = 0;					// 快速出售的剩余次数
	int mQuickDecomposeCount = 0;				// 快速分解的剩余次数
	int mAuctionDiscount = 0;					// 拍卖行优惠次数
	int mBossDropProtectCount = 0;				// boss掉落物品保护剩余次数
	int mExchangeID = 0;						// 当前交易ID,如果为0,则表示没有正在交易
	llong mMaxGold = 0;							// 当前等级的金币上限
	int mMaxCreateGold = 0;						// 当前等级的当日产出金币上限,达到上限后产出金币将减少
	int mLifeItem = 0;							// 本命物品ID
	int mPKArenaScore = 0;						// PK竞技场积分
	short mDefaultMapID = 0;					// 登录游戏时默认进入的地图
	ushort mNotAllowUseConsumable = 0;			// 是否禁止使用消耗品,大于0表示禁止使用
	ushort mInverseHPMedicine = 0;				// 是否在使用回血药剂时为掉血效果,大于0表示变为掉血效果
	byte mMoveDirection = 0;					// 当前的移动方向,由客户端发送过来的,一般都与朝向一致,但是并不保证完全与朝向一致
	SUPER_PERMISSION mSuperPermission = SUPER_PERMISSION::NO;	// 超级权限
	PLAYER_SEX mSex = PLAYER_SEX::MALE;			// 性别
	PLAYER_JOB mJob = PLAYER_JOB::FIGHTER;		// 职业
	PK_MODE mPKMode = PK_MODE::MAX;				// pk模式
	bool mSecondaryPasswordVerify = false;		// 本次登录是否已经验证了二级密码
	bool mEnterMapFinish = false;				// 是否已经完成进入地图
	bool mLoginFinish = false;					// 玩家是否已经登录完毕
	bool mPackChanged = false;					// 这一帧背包是否有变化,用于判断是否需要同步到客户端
	bool mPackOnlyPropertyChanged = false;		// 背包变化时是否只是物品属性变化,比如数量变化
};