#include "GameHeader.h"
#ifdef WINDOWS
#pragma comment(lib, "MyServerFramework_Frame.lib")
#endif

#ifdef WINDOWS
BOOL ConsoleEventHandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		auto* mServerFramework = ServerFramework::getSingleton();
		if (mServerFramework != nullptr)
		{
			mServerFramework->stop();
		}
	}
	return FALSE;
}
#endif

Array<52, int> KEY_ARRAY{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U' ,'V', 'W', 'X', 'Y', 'Z',
						  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u' ,'v', 'w', 'x', 'y', 'z' };

int main()
{
	// 记录主线程ID
	setMainThread(getThreadID());
	setTimeMS(getRealTimeMS());
	setTimeSecond((llong)time(nullptr));
	// 设置随机数种子
	srand((int)time(nullptr));

#ifdef WINDOWS
	// 设置窗口事件回调
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleEventHandler, TRUE);
#endif

	Game* game = new Game();
	game->init();
	// 创建完窗口后启动客户端
	game->launch();
	TimeLock timeLock(33);
	while (!game->isStop())
	{
#ifdef WINDOWS
		if (mInputSystem != nullptr)
		{
			FOR_I(KEY_ARRAY.size())
			{
				if (GetAsyncKeyState(KEY_ARRAY[i]) & 0x8000)
				{
					mInputSystem->keyDown(KEY_ARRAY[i]);
				}
				else
				{
					mInputSystem->keyUp(KEY_ARRAY[i]);
				}
			}
		}
#endif
		// 每帧获取一次当前时间
		setTimeMS(getRealTimeMS());
		setTimeSecond((llong)time(nullptr));
		const float frameTime = timeLock.update() * 0.001f;
		const llong startRealMS = getRealTimeMS();
		game->update(frameTime);
		game->addFrameTime(getRealTimeMS() - startRealMS);
	}
	game->destroy();
	delete game;

#ifdef WINDOWS
	// 移除窗口事件回调
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleEventHandler, FALSE);
#endif
	return 0;
}