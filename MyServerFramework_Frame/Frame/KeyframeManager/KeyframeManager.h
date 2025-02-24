#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API KeyframeManager : public FrameComponent
{
	BASE(KeyframeManager, FrameComponent);
public:
	void init() override;
	void quit() override;
	AnimationCurve* getKeyFrame(const int id) const { return mCurveList.tryGet(id); }
	void loadAll();
protected:
	template<typename T, typename TypeCheck = typename IsSubClassOf<AnimationCurve, T>::mType>
	void createCurve(const int id)
	{
		mCurveList.insert(id, new T());
	}
protected:
	HashMap<int, AnimationCurve*> mCurveList;
};