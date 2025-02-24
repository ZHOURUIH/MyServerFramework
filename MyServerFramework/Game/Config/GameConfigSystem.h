#pragma once

#include "FrameComponent.h"
#include "Utility.h"
#include "GameEnum.h"

class GameConfigSystem : public FrameComponent
{
	BASE(GameConfigSystem, FrameComponent);
public:
	~GameConfigSystem() override { destory(); }
	void init() override;
	void destory() {}
	bool isTestServer() const { return mTestServer; }
	bool isSeasonServer() const { return mSeasonServer; }
protected:
	void readConfig(const Vector<pair<string, string>>& valueList);
protected:
	bool mTestServer = false;		// 是否为测试服,测试服只允许测试客户端可见,true表示测试服,false表示正式服,仅linux使用
	bool mSeasonServer = false;		// 是否为赛季服,赛季服会固定的时间清空数据,true表示赛季服,false表示普通服务器
};