#include "FrameHeader.h"

CharacterState::CharacterState()
{
	mID = makeIDMain();
}

CharacterState::~CharacterState()
{
	if (mWillRemoveCallbackList != nullptr)
	{
		delete mWillRemoveCallbackList;
		mWillRemoveCallbackList = nullptr;
	}
	if (mParamMemberList != nullptr)
	{
		delete mParamMemberList;
		mParamMemberList = nullptr;
	}
}

void CharacterState::enter(StateParam* param)
{
	if (param == nullptr || param->getParamMemberList().size() == 0)
	{
		return;
	}
	if (mParamMemberList == nullptr)
	{
		mParamMemberList = new Vector<StateParamMemberValue>();
	}
	if (mParamMemberList->size() > 0)
	{
		ERROR_PROFILE("参数列表不为空");
	}
	for (const StateParamMember& sourceMember : param->getParamMemberList())
	{
		StateParamMemberValue member;
		member.mTypeHash = sourceMember.getTypeHash();
		if (sourceMember.canBeFloat())
		{
			member.mFloatValue = sourceMember.asFloat();
		}
		else
		{
			member.mStringValue = sourceMember.asString();
		}
		mParamMemberList->push_back(member);
	}
}

void CharacterState::update(const float elapsedTime)
{
	if (tickTimerOnce(mStateTime, elapsedTime))
	{
		removeSelf();
	}
}

void CharacterState::resetProperty()
{
	base::resetProperty();
	if (mWillRemoveCallbackList != nullptr)
	{
		mWillRemoveCallbackList->clear();
	}
	if (mParamMemberList != nullptr)
	{
		mParamMemberList->clear();
	}
	mID = makeIDMain();
	mCharacter = nullptr;
	mType = 0;
	mOriginStateTime = -1.0f;
	mStateTime = -1.0f;
	mActive = true;
	mLeaving = false;
	// 以下的都不重置
	// mIsBuff = false;
	// mSuperposition = STATE_MUTEX::REMOVE_OLD;
	// mNeedTick = true;
	// mLine = 0;
}

void CharacterState::removeSelf(const bool isBreak, const bool removeByDie)
{
	if (mDestroy || mLeaving)
	{
		return;
	}
	mCharacter->getComponentAtIndex<COMCharacterStateMachine>()->removeState(this, isBreak, removeByDie);
}

void CharacterState::callWillRemoveCallback()
{
	if (mWillRemoveCallbackList == nullptr)
	{
		return;
	}
	for (const auto& item : *mWillRemoveCallbackList)
	{
		for (const auto& callback : item.second)
		{
			callback(this);
		}
	}
	mWillRemoveCallbackList->clear();
}