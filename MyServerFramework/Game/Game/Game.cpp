#include "GameHeader.h"

// 下面包含自动生成的代码,以FrameSystem Register为起始关键字,以后面的第一个空行或者}为结束
void Game::registeComponent()
{
	base::registeComponent();
	// FrameSystem Register
	registeSystem<AccountManager>(STR(AccountManager));
	registeSystem<AchivementManager>(STR(AchivementManager));
	registeSystem<ActivityTerritorySystem>(STR(ActivityTerritorySystem));
	registeSystem<MonsterWarSystem>(STR(MonsterWarSystem));
	registeSystem<ShabakeBattleSystem>(STR(ShabakeBattleSystem));
	registeSystem<WorldBossSystem>(STR(WorldBossSystem));
	registeSystem<AuctionSystem>(STR(AuctionSystem));
	registeSystem<DTBuilderManager>(STR(DTBuilderManager));
	registeSystem<StateManager>(STR(StateManager));
	registeSystem<ChatSystem>(STR(ChatSystem));
	registeSystem<ConditionManager>(STR(ConditionManager));
	registeSystem<GameConfigSystem>(STR(GameConfigSystem));
	registeSystem<DebugSystem>(STR(DebugSystem));
	registeSystem<EnvironmentSystem>(STR(EnvironmentSystem));
	registeSystem<ExchangeManager>(STR(ExchangeManager));
	registeSystem<GameVerifySystem>(STR(GameVerifySystem));
	registeSystem<GuildManager>(STR(GuildManager));
	registeSystem<LoginGateSystem>(STR(LoginGateSystem));
	registeSystem<LotteryTicketSystem>(STR(LotteryTicketSystem));
	registeSystem<MailManager>(STR(MailManager));
	registeSystem<MissionManager>(STR(MissionManager));
	registeSystem<MonsterManager>(STR(MonsterManager));
#ifdef _MYSQL
	registeSystem<MySQLCheckSystem>(STR(MySQLCheckSystem));
#endif
	registeSystem<NPCManager>(STR(NPCManager));
	registeSystem<ObjectItemManager>(STR(ObjectItemManager));
	registeSystem<OfferRewardManager>(STR(OfferRewardManager));
	registeSystem<PKArenaSystem>(STR(PKArenaSystem));
	registeSystem<PKModeManager>(STR(PKModeManager));
	registeSystem<QQMailSystem>(STR(QQMailSystem));
	registeSystem<RankSystem>(STR(RankSystem));
	registeSystem<RedEnvelopeSystem>(STR(RedEnvelopeSystem));
	registeSystem<SceneCopyManager>(STR(SceneCopyManager));
	registeSystem<SceneMapManager>(STR(SceneMapManager));
	registeSystem<SceneTriggerManager>(STR(SceneTriggerManager));
	registeSystem<SeasonSystem>(STR(SeasonSystem));
	registeSystem<ServerGlobalSystem>(STR(ServerGlobalSystem));
	registeSystem<ShoppingMallSystem>(STR(ShoppingMallSystem));
	registeSystem<GameNetServerMiscSystem>(STR(GameNetServerMiscSystem));
	registeSystem<TeamManager>(STR(TeamManager));
	registeSystem<TreasureHouseSystem>(STR(TreasureHouseSystem));
	registeSystem<AccountPool>(STR(AccountPool));
	registeSystem<AccountSellInfoPool>(STR(AccountSellInfoPool));
	registeSystem<AchivementPool>(STR(AchivementPool));
	registeSystem<ActivityGuildDataPool>(STR(ActivityGuildDataPool));
	registeSystem<AuctionItemPool>(STR(AuctionItemPool));
	registeSystem<AuctionPriceInfoPool>(STR(AuctionPriceInfoPool));
	registeSystem<AuctionSearchParamPool>(STR(AuctionSearchParamPool));
	registeSystem<CharacterBehaviourPool>(STR(CharacterBehaviourPool));
	registeSystem<CharacterSkillPool>(STR(CharacterSkillPool));
	registeSystem<ConditionPool>(STR(ConditionPool));
	registeSystem<DamageInfoPool>(STR(DamageInfoPool));
	registeSystem<DamageTokenBulletPool>(STR(DamageTokenBulletPool));
	registeSystem<DamageTokenSkillPool>(STR(DamageTokenSkillPool));
	registeSystem<EquipCollectionPool>(STR(EquipCollectionPool));
	registeSystem<ExchangeInfoPool>(STR(ExchangeInfoPool));
	registeSystem<ExchangePlayerPool>(STR(ExchangePlayerPool));
	registeSystem<FunctionParamPool>(STR(FunctionParamPool));
	registeSystem<GuildPool>(STR(GuildPool));
	registeSystem<GuildRequestInfoPool>(STR(GuildRequestInfoPool));
	registeSystem<GuildShopGoodsPool>(STR(GuildShopGoodsPool));
	registeSystem<GuildTerritoryDataPool>(STR(GuildTerritoryDataPool));
	registeSystem<LotteryTicketPlayerPool>(STR(LotteryTicketPlayerPool));
	registeSystem<MailPool>(STR(MailPool));
	registeSystem<MissionPool>(STR(MissionPool));
	registeSystem<MonsterFireSkillInfoPool>(STR(MonsterFireSkillInfoPool));
	registeSystem<MonsterGeneratePool>(STR(MonsterGeneratePool));
	registeSystem<MonsterMoveInfoPool>(STR(MonsterMoveInfoPool));
	registeSystem<MonsterWarPool>(STR(MonsterWarPool));
	registeSystem<NPCGoodsItemPool>(STR(NPCGoodsItemPool));
	registeSystem<ObjectItemPool>(STR(ObjectItemPool));
	registeSystem<ObjectItemSpecificPool>(STR(ObjectItemSpecificPool));
	registeSystem<OfferRewardPool>(STR(OfferRewardPool));
	registeSystem<PKArenaSingleInstancePool>(STR(PKArenaSingleInstancePool));
	registeSystem<PKSingleMatchInfoPool>(STR(PKSingleMatchInfoPool));
	registeSystem<PKSinglePlayerInfoPool>(STR(PKSinglePlayerInfoPool));
	registeSystem<PlayerServerDataPool>(STR(PlayerServerDataPool));
	registeSystem<PlayerVerifyDataPool>(STR(PlayerVerifyDataPool));
	registeSystem<QQVerifyInfoPool>(STR(QQVerifyInfoPool));
	registeSystem<RankInfoPool>(STR(RankInfoPool));
	registeSystem<RedEnvelopePool>(STR(RedEnvelopePool));
	registeSystem<SceneCopyPool>(STR(SceneCopyPool));
	registeSystem<SceneMapPool>(STR(SceneMapPool));
	registeSystem<SceneSkillInfoPool>(STR(SceneSkillInfoPool));
	registeSystem<SceneTriggerPool>(STR(SceneTriggerPool));
	registeSystem<ShabakeBattleGuildInfoPool>(STR(ShabakeBattleGuildInfoPool));
	registeSystem<ShabakeBattleMemberDataPool>(STR(ShabakeBattleMemberDataPool));
	registeSystem<SkillBulletParamPool>(STR(SkillBulletParamPool));
	registeSystem<SkillBulletPool>(STR(SkillBulletPool));
	registeSystem<TeamInfoPool>(STR(TeamInfoPool));
	registeSystem<TerritoryMapDataPool>(STR(TerritoryMapDataPool));
	registeSystem<TriggerModifierPool>(STR(TriggerModifierPool));
	registeSystem<WorldBossPlayerInfoPool>(STR(WorldBossPlayerInfoPool));
	registeSystem<AchivementFactoryManager>(STR(AchivementFactoryManager));
	registeSystem<CharacterBehaviourFactoryManager>(STR(CharacterBehaviourFactoryManager));
	registeSystem<CharacterSkillFactoryManager>(STR(CharacterSkillFactoryManager));
	registeSystem<ConditionFactoryManager>(STR(ConditionFactoryManager));
	registeSystem<MonsterGenerateFactoryManager>(STR(MonsterGenerateFactoryManager));
	registeSystem<ObjectItemFactoryManager>(STR(ObjectItemFactoryManager));
	registeSystem<ObjectItemSpecificFactoryManager>(STR(ObjectItemSpecificFactoryManager));
	registeSystem<SceneTriggerFactoryManager>(STR(SceneTriggerFactoryManager));
	registeSystem<TriggerModifierFactoryManager>(STR(TriggerModifierFactoryManager));
}

void Game::constructDone()
{
	base::constructDone();
	constructGameDone();
	// 由于其他系统组件的初始化可能会用到列表对象池,所以需要在初始化之前就注册对象池
	GameSTLPoolRegister::registeAll();
	// SQLite和MySQL的注册只能在此处写,因为在其他系统组件的初始化里面会用到这些
	SQLiteRegister::registeAll();
	MySQLRegister::registeAll();
	PKModeRegister::registeAll();
	// 等待所有表格都注册完毕后才能检查表格数据
	mSQLiteManager->checkAll();
}

void Game::clearSystem()
{
	base::clearSystem();
	clearGameSystem();
}

void Game::registe()
{
	base::registe();
	CharacterRegister::registeAll();
	CharacterSkillRegister::registeAll();
	SceneTriggerRegister::registeAll();
	GamePacketRegister::registeAll();
	GameComponentRegister::registeAll();
	CharacterBehaviourRegister::registeAll();
	MailPacketRegister::registeAll();
	ObjectItemRegister::registeAll();
	ConditionRegister::registeAll();
	AchivementRegister::registeAll();
	GatePacketRegister::registeAll();
	MonsterGenerateRegister::registeAll();
	TimeTaskRegister::registerAll();

	mMySQLDataPool->initDefault<MDObjectItemMaterial>(50000);
	mMySQLDataPool->initDefault<MDObjectItemConsumable>(50000);
	mMySQLDataPool->initDefault<MDObjectItemEquip>(50000);
	mMySQLDataPool->initDefault<MDObjectItemBox>(50000);
	mMySQLDataPool->initDefault<MDObjectItemID>(100000);
	mMySQLDataPool->initDefault<MDObjectItemSkillBook>(50000);
	mCharacterPool->initDefault(CHARACTER_TYPE::MONSTER, 100000);
	mGameComponentPool->initDefault(GameStringDefine::COMMonsterSkill, 100000);
	mGameComponentPool->initDefault(FrameStringDefine::COMCharacterStateMachine, 100000);
	mGameComponentPool->initDefault(FrameStringDefine::COMMonsterDecisionTree, 50000);
	mGameComponentPool->initDefault(GameStringDefine::COMCharacterHPRecover, 100000);
	mStatePool->initDefault(CHARACTER_STATE::StateActionStand, 100000);
}

void Game::launch()
{
	base::launch();
	mTCPServerSystem->setServerCheckPingCallback([](TCPServerClient* client, const int index)
	{
		SCServerCheckPing::send(client, index);
	});
	FrameMySQLUtility::mOnErrorProfileEvent = [](string&& log, string&& file) { MySQLUtility::writeErrorProfile(move(log), move(file)); };
	CmdNetServerLogoutAccount::mLogoutPlayer = CmdCharacterManagerPlayerLogout::execute;
	CmdNetServerLogoutAccount::mLogoutAccount = ([](TCPServerClient* client)
	{
		mAccountManager->accountLogout(client->getAccountGUID());
		client->setAccountGUID(0);
	});
	mTCPServerSystem->setFreezeAccountCallback([](const llong accountGUID, const llong timeSecond, const char* reason)
	{
		CMD_THREAD_DELAY(CmdNetServerFreezeAccount, cmd);
		cmd->mAccountGUID = accountGUID;
		cmd->mUnfreezeTime = timeSecond;
		cmd->mReason = reason;
		mCommandSystem->pushCommandDelay(cmd, mTCPServerSystem);
	});
	mQQMailSystem->connect();
	mLoginGateSystem->connect();
}