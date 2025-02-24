#pragma once

#include "GameComponent.h"
#include "IComponentInterface.h"

class MICRO_LEGEND_FRAME_API ComponentKeyframe : public GameComponent
{
	BASE(ComponentKeyframe, GameComponent);
public:
	void setActive(const bool active, const bool isInit = false) override
	{
		base::setActive(active, isInit);
		if (!active)
		{
			stop();
		}
	}
	void play(int keyframeID, bool loop = false, float onceLength = 1.0f);
	void stop(bool force = false);
	void pause() { mPlayState = PLAY_STATE::PAUSE; }
	void setState(PLAY_STATE state);
	float getTremblingPercent() const { return divide(mCurrentTime, mOnceLength); }
	void setDoingCallback(KeyFrameCallback callback, void* userData, bool callLast = true)
	{
		if (callLast)
		{
			setCallback(callback, userData, mDoingCallback, mDoingUserData, this);
		}
		else
		{
			mDoingCallback = callback;
			mDoingUserData = userData;
		}
	}
	void setDoneCallback(KeyFrameCallback callback, void* userData, bool callLast = true)
	{
		if (callLast)
		{
			setCallback(callback, userData, mDoneCallback, mDoneUserData, this);
		}
		else
		{
			mDoneCallback = callback;
			mDoneUserData = userData;
		}
	}
	bool getLoop() const					{ return mLoop; }
	float getOnceLength() const				{ return mOnceLength; }
	PLAY_STATE getState() const				{ return mPlayState; }
	float getCurrentTime() const			{ return mCurrentTime; }
	AnimationCurve* getKeyFrame() const		{ return mKeyframe; }
	int getKeyframeID() const				{ return mKeyframeID; }
	void setLoop(const bool loop)			{ mLoop = loop; }
	void setOnceLength(const float length)	{ mOnceLength = length; mInverseOnceLength = divide(1.0f, mOnceLength); }
	void setCurrentTime(const float time)	{ mCurrentTime = time; }
	void setKeyframeID(const int keyframeID){ mKeyframeID = keyframeID; }
	void resetProperty() override;
protected:
	void clearCallback() override;
	void afterApplyTrembling(bool done);
	static void doneCallback(const KeyFrameCallback& curDoneCallback, void*& userData, ComponentKeyframe* component);
	static void setCallback(KeyFrameCallback callback, void* userData, KeyFrameCallback& curCallback, void*& curUserData, ComponentKeyframe* component);
	virtual void applyTrembling(float value) { }
protected:
	void registeTick() override { mComponentOwner->registeFrameTick(this, frameTick_ComponentKeyframe); }
	void unregisteTick() override { mComponentOwner->unregisteFrameTick(this, frameTick_ComponentKeyframe); }
	static void frameTick_ComponentKeyframe(GameComponent* component, float elapsedTime) { static_cast<This*>(component)->onFrameTick(elapsedTime); }
private:
	void onFrameTick(float elapsedTime);
protected:
	KeyFrameCallback mDoingCallback = nullptr;	// 运行中回调函数
	KeyFrameCallback mDoneCallback = nullptr;	// 运行结束时回调函数
	void* mDoingUserData = nullptr;				// 运行中回调函数的参数
	void* mDoneUserData = nullptr;				// 运行结束时回调函数的参数
	AnimationCurve*	mKeyframe = nullptr;		// 当前使用的关键帧
	float mCurrentTime = 0.0f;					// 从上一次从头开始播放到现在的时长
	float mPlayedTime = 0.0f;					// 本次震动已经播放的时长,从上一次开始播放到现在的累计时长
	float mOnceLength = 1.0f;					// 关键帧长度默认为1秒
	float mInverseOnceLength = 1.0f;			// 关键帧长度的倒数,方便做除法
	float mPlayLength = 0.0f;					// 小于0表示无限播放, 大于等于0表示播放length时长
	float mStopValue = 0.0f;					// 当组件停止时,需要应用的关键帧值
	short mKeyframeID = 0;						// 关键帧ID,用于查询关键帧曲线
	bool mLoop = true;							// 是否循环
	PLAY_STATE mPlayState = PLAY_STATE::STOP;	// 当前播放状态
};