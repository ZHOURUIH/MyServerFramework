#pragma once

// 能够被其他组件中断的组件
class MICRO_LEGEND_FRAME_API IComponentBreakable
{
public:
	virtual void notifyBreak() = 0;
};

// 会修改位置的组件
class MICRO_LEGEND_FRAME_API IComponentModifyPosition : public IComponentBreakable {};

// 会修改旋转的组件
class MICRO_LEGEND_FRAME_API IComponentModifyRotation : public IComponentBreakable {};

// 会修改缩放的组件
class MICRO_LEGEND_FRAME_API IComponentModifyScale : public IComponentBreakable {};