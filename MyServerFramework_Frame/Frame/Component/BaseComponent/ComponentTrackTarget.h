#pragma once

#include "GameComponent.h"
#include "IComponentInterface.h"

class MICRO_LEGEND_FRAME_API ComponentTrackTarget : public GameComponent, public IComponentModifyPosition
{
	BASE(ComponentTrackTarget, GameComponent);
public:
	virtual void setMoveDoneTrack(TrackCallback doneCallback, void* userData);
	void setTrackingCallback(TrackCallback callback, void* userData) 
	{
		mDoingCallback = callback; 
		mDoingUserData = userData;
	}
	void setSpeed(const float speed) { mSpeed = speed; }
	void setTargetOffset(const Vector3& offset) { mTargetOffset = offset; }
	virtual void setTrackTarget(void* target)
	{
		mTarget = target;
		if (mTarget == nullptr)
		{
			setActive(false);
		}
	}
	void* getTrackTarget() const { return mTarget; }
	void notifyBreak() override
	{
		setMoveDoneTrack(nullptr, nullptr);
		setTrackingCallback(nullptr, nullptr);
		setTrackTarget(nullptr);
	}
	void resetProperty() override;
	virtual Vector3 getPosition() const = 0;
	virtual void setPosition(const Vector3& pos) = 0;
	virtual Vector3 getTargetPosition() const = 0;
protected:
	void registeTick() override { mComponentOwner->registeFrameTick(this, frameTick_ComponentTrackTarget); }
	void unregisteTick() override { mComponentOwner->unregisteFrameTick(this, frameTick_ComponentTrackTarget); }
	static void frameTick_ComponentTrackTarget(GameComponent* component, float elapsedTime) { static_cast<This*>(component)->onFrameTick(elapsedTime); }
private:
	void onFrameTick(float elapsedTime);
protected:
	void* mTarget = nullptr;
	float mSpeed = 0.0f;
	Vector3 mTargetOffset;
	TrackCallback mDoneCallback = nullptr;
	TrackCallback mDoingCallback = nullptr;
	void* mDoneUserData = nullptr;
	void* mDoingUserData = nullptr;
};