#pragma once

#include "FrameComponent.h"
#include "KeyState.h"

class MICRO_LEGEND_FRAME_API InputSystem : public FrameComponent
{
	BASE(InputSystem, FrameComponent);
public:
	InputSystem();
	void quit() override;
	void update(float elapsedTime) override;
	void lateUpdate(float elapsedTime) override;
	void keyDown(int key);
	void keyUp(int key);
	void onChar(const char key) { mCurCharList.add(key); }
	bool isKeyDown(int key) const;
	bool isKeyCurrentDown(int key) const;
	bool isKeyCurrentUp(int key) const;
	float getKeyTime(int key) const;
	void mouseLeftDown(Vector2 position);
	void mouseRightDown(Vector2 position);
	void mouseMiddleDown(Vector2 position);
	void mouseLeftUp(Vector2 position);
	void mouseRightUp(Vector2 position);
	void mouseMiddleUp(Vector2 position);
	void mouseMove(Vector2 position)									{ setCurMousePosition(position); }
	void mouseWheel(const float delta)									{ mCurMouseWheelDelta = delta; }
	void setCurMousePosition(Vector2 curPos)							{ mLastMousePosition = mCurMousePosition; mCurMousePosition = curPos; }
	const KeyState* getMouseButtonState(const MOUSE_BUTTON mouse) const	{ return mMouseState.getPtrConst(mouse); }
	const Set<int>& getCurDownKeyList() const							{ return mCurDownKeyList; }
	const Set<int>& getCurCharList() const								{ return mCurCharList; }
	Vector2 getMousePosition() const									{ return mCurMousePosition; }
	Vector2 getLastMousePosition() const								{ return mLastMousePosition; }
	float getMouseWheelDelta() const									{ return mCurMouseWheelDelta; }
protected:
	HashMap<MOUSE_BUTTON, KeyState> mMouseState;	// 鼠标的按下状态
	HashMap<int, KeyState> mKeyState;				// 键盘的按下状态
	Set<int> mCurDownKeyList;						// 所有处于当前按下状态的键
	Set<int> mCurCharList;							// 当前按下的字符列表
	Vector2 mLastMousePosition;
	Vector2 mCurMousePosition;
	float mCurMouseWheelDelta = 0.0f;
};