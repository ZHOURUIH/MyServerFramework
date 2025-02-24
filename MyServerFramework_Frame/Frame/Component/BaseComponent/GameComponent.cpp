#include "FrameHeader.h"

void GameComponent::resetProperty()
{
	base::resetProperty();
	mComponentOwner = nullptr;
	mComponentType = 0;
	mActive = true;
	mDefaultActive = true;
}

void GameComponent::setActive(const bool active, const bool isInit)
{
	if (mActive == active && !isInit)
	{
		return;
	}
	mActive = active;
	// 如果是初始化为false的组件,则不加入列表,也不需要执行后续的判断,提高效率
	if (isInit && !mActive)
	{
		return;
	}
	if (mActive)
	{
		registeTick();
	}
	else
	{
		unregisteTick();
	}
}