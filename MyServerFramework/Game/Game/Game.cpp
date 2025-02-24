#include "GameHeader.h"

// 下面包含自动生成的代码,以FrameSystem Register为起始关键字,以后面的第一个空行或者}为结束
void Game::registeComponent()
{
	base::registeComponent();
	// FrameSystem Register
	registeSystem<GameConfigSystem>(STR(GameConfigSystem));
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
	GamePacketRegister::registeAll();
}

void Game::launch()
{
	base::launch();
	mTCPServerSystem->setServerCheckPingCallback([](TCPServerClient* client, const int index)
	{
		SCServerCheckPing::send(client, index);
	});
	CmdNetServerLogoutAccount::mLogoutPlayer = ([](TCPServerClient* client)
	{
		auto* player = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(client->getPlayerGUID()));
		if (player != nullptr)
		{
			LOG("玩家退出登录：" + player->getPrintName() + ", ID:" + LLToS(player->getGUID()));
			// 将角色销毁
			CmdCharacterManagerDestroyCharacter::execute(player);
		}
		// 通知网络客户端角色退出登录
		client->setPlayerGUID(0);
	});
	CmdNetServerLogoutAccount::mLogoutAccount = ([](TCPServerClient* client)
	{
		client->setAccountGUID(0);
	});
}