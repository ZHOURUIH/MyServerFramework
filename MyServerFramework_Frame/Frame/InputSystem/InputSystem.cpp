#include "FrameHeader.h"

InputSystem::InputSystem()
{
	KeyState state;
	state.mStateTime = 0.0f;
	state.mState = KEY_STATE::KEEP_UP;
	mMouseState.insert(MOUSE_BUTTON::LEFT, state);
	mMouseState.insert(MOUSE_BUTTON::RIGHT, state);
	mMouseState.insert(MOUSE_BUTTON::MIDDLE, state);
}

void InputSystem::quit()
{
	mMouseState.clear();
	mKeyState.clear();
	mCurDownKeyList.clear();
	mCurCharList.clear();
}

void InputSystem::lateUpdate(const float elapsedTime)
{
	for (auto& iter : mKeyState)
	{
		KeyState& state = iter.second;
		state.mStateTime += elapsedTime;
		if (state.mState == KEY_STATE::CURRENT_UP)
		{
			state.mState = KEY_STATE::KEEP_UP;
		}
		else if (state.mState == KEY_STATE::CURRENT_DOWN)
		{
			state.mState = KEY_STATE::KEEP_DOWN;
		}
	}

	for (auto& iterMouse : mMouseState)
	{
		KeyState& state = iterMouse.second;
		state.mStateTime += elapsedTime;
		if (state.mState == KEY_STATE::CURRENT_UP)
		{
			state.mState = KEY_STATE::KEEP_UP;
		}
		else if (state.mState == KEY_STATE::CURRENT_DOWN)
		{
			state.mState = KEY_STATE::KEEP_DOWN;
		}
	}

	// 清空当前按下的按键列表和字符列表
	mCurDownKeyList.clear();
	mCurCharList.clear();
	mCurMouseWheelDelta = 0.0f;
	mLastMousePosition = mCurMousePosition;
}

void InputSystem::keyDown(const int key)
{
	if (KeyState* state = mKeyState.getPtr(key))
	{
		if (state->mState == KEY_STATE::CURRENT_UP || state->mState == KEY_STATE::KEEP_UP)
		{
			state->mState = KEY_STATE::CURRENT_DOWN;
			state->mStateTime = 0.0f;
			mCurDownKeyList.insert(key);
		}
	}
	else
	{
		KeyState keyState;
		keyState.mStateTime = 0.0f;
		keyState.mState = KEY_STATE::CURRENT_DOWN;
		mKeyState.insert(key, keyState);
		mCurDownKeyList.insert(key);
	}
}

void InputSystem::keyUp(const int key)
{
	if (KeyState* state = mKeyState.getPtr(key))
	{
		if (state->mState == KEY_STATE::CURRENT_DOWN || state->mState == KEY_STATE::KEEP_DOWN)
		{
			state->mState = KEY_STATE::CURRENT_UP;
			state->mStateTime = 0.0f;
		}
	}
	else
	{
		KeyState keyState;
		keyState.mStateTime = 0.0f;
		keyState.mState = KEY_STATE::CURRENT_UP;
		mKeyState.insert(key, keyState);
	}
}

bool InputSystem::isKeyDown(const int key) const
{
	const KEY_STATE state = mKeyState.tryGet(key).mState;
	return state == KEY_STATE::CURRENT_DOWN || state == KEY_STATE::KEEP_DOWN;
}

bool InputSystem::isKeyCurrentDown(const int key) const
{
	return mKeyState.tryGet(key).mState == KEY_STATE::CURRENT_DOWN;
}

bool InputSystem::isKeyCurrentUp(const int key) const
{
	return mKeyState.tryGet(key).mState == KEY_STATE::CURRENT_UP;
}

float InputSystem::getKeyTime(const int key) const
{
	return mKeyState.tryGet(key).mStateTime;
}

void InputSystem::mouseLeftDown(const Vector2& position)
{
	mMouseState[MOUSE_BUTTON::LEFT].mStateTime = 0.0f;
	mMouseState[MOUSE_BUTTON::LEFT].mState = KEY_STATE::CURRENT_DOWN;
	setCurMousePosition(position);
}

void InputSystem::mouseRightDown(const Vector2& position)
{
	mMouseState[MOUSE_BUTTON::RIGHT].mStateTime = 0.0f;
	mMouseState[MOUSE_BUTTON::RIGHT].mState = KEY_STATE::CURRENT_DOWN;
	setCurMousePosition(position);
}

void InputSystem::mouseMiddleDown(const Vector2& position)
{
	mMouseState[MOUSE_BUTTON::MIDDLE].mStateTime = 0.0f;
	mMouseState[MOUSE_BUTTON::MIDDLE].mState = KEY_STATE::CURRENT_DOWN;
	setCurMousePosition(position);
}

void InputSystem::mouseLeftUp(const Vector2& position)
{
	mMouseState[MOUSE_BUTTON::LEFT].mStateTime = 0.0f;
	mMouseState[MOUSE_BUTTON::LEFT].mState = KEY_STATE::CURRENT_UP;
	setCurMousePosition(position);
}

void InputSystem::mouseRightUp(const Vector2& position)
{
	mMouseState[MOUSE_BUTTON::RIGHT].mStateTime = 0.0f;
	mMouseState[MOUSE_BUTTON::RIGHT].mState = KEY_STATE::CURRENT_UP;
	setCurMousePosition(position);
}

void InputSystem::mouseMiddleUp(const Vector2& position)
{
	mMouseState[MOUSE_BUTTON::MIDDLE].mStateTime = 0.0f;
	mMouseState[MOUSE_BUTTON::MIDDLE].mState = KEY_STATE::CURRENT_UP;
	setCurMousePosition(position);
}
