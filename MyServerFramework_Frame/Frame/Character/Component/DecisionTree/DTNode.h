#pragma once

#include "ClassPooledObject.h"
#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API DTNode : public ClassPooledObject
{
	BASE(DTNode, ClassPooledObject);
public:
	~DTNode() override;
	// 节点创建时执行一次init
	virtual void init() {}
	// 节点由非运行切换为运行状态时调用一次start
	virtual TASK_STATE start();
	// 节点处于运行状态时每帧调用一次update
	virtual TASK_STATE update(const float elapsedTime);
	// 从指定的子节点开始执行
	virtual TASK_STATE startExecute(DTNode* startChild);
	void setParent(DTNode* parent);
	void setFailed();
	void setSuccess();
	void setType(const char* type)					{ mType = type; }
	void setCharacter(Character* character)			{ mCharacter = character; }
	DTNode* getParent() const						{ return mParent; }
	const char* getType() const						{ return mType; }
	TASK_STATE getState() const						{ return mState; }
#ifdef WINDOWS
	void setDebugString(const char* debugString)	{ mDebugString = debugString; }
	const char* getDebugString() const 				{ return mDebugString; }
	int getExecuteCount() const						{ return mExecuteCount; }
#endif
	const Vector<DTNode*>* getChildList() const		{ return mChildList; }
	virtual void notifyChildStart(DTNode* child);
	DTNode* getNextSibling(DTNode* node);
	virtual void addChild(DTNode* child);
	virtual void clearChild();
	virtual void reset();
	void resetProperty() override;
	virtual bool isCondition() { return false; }
	virtual void abort();
	void setNotifyParentRunning(bool notify) { mNotifyParentRunning = notify; }
protected:
	Vector<DTNode*>* mChildList = nullptr;
	Character* mCharacter = nullptr;
	DTNode* mParent = nullptr;
	const char* mType = nullptr;
#ifdef WINDOWS
	const char* mDebugString = nullptr;
	int mExecuteCount = 0;
#endif
	bool mNotifyParentRunning = true;
	TASK_STATE mState = TASK_STATE::NONE;
};