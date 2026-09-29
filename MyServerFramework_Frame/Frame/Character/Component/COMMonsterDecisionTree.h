#pragma once

#include "GameComponent.h"
#include "FrameStringDefine.h"
#include "FrameDefine.h"
#include "DTConditionGroup.h"
#include "DTRepeater.h"
#include "DTNodePool.h"

class MICRO_LEGEND_FRAME_API COMMonsterDecisionTree : public GameComponent
{
	BASE(COMMonsterDecisionTree, GameComponent);
public:
	static ushort staticType() { return FrameStringDefine::COMMonsterDecisionTree; }
	void destroy() override;
	void clearTree();
	void resetProperty() override;
	static void setRelation(DTNode* parent, DTNode* child);
	template<typename T, typename TypeCheck = typename IsSubClassOf<DTNode, T>::mType>
	DTNode* createNode(DTNode* parent, const char* debugString)
	{
		DTNode* node = mDTNodePool->newClass<T>();
		node->setCharacter(static_cast<Character*>(mComponentOwner));
		node->init();
		mNodeList.add(node);
		if (parent == nullptr)
		{
			parent = mRoot;
		}
		if (parent != nullptr)
		{
			setRelation(parent, node);
		}
#ifdef WINDOWS
		node->setDebugString(debugString);
#endif
		return node;
	}
	void addConditionMutex(DTConditionGroup* condition, DTNode* mutexNode)
	{
		mMutexConditionList.addOrGet(condition).add(mutexNode);
	}
	template<int Length>
	void addListenCondition(const ArrayList<Length, DTConditionGroup*>& conditionList)
	{
		// 只作为条件监听的条件组是不需要向父节点通知开始执行的
		mConditionGroup.add();
		auto& lastGroup = mConditionGroup[mConditionGroup.size() - 1];
		lastGroup.reserve(conditionList.size());
		for (DTConditionGroup* conditionGroup : conditionList)
		{
			conditionGroup->setNotifyParentRunning(false);
			lastGroup.add(conditionGroup);
		}
	}
	void addListenCondition(DTConditionGroup* condition0, DTConditionGroup* condition1);
	void addListenCondition(DTConditionGroup* condition);
	DTRepeater* getRoot() const { return mRoot; }
	const Vector<DTNode*>& getNodeList() const { return mNodeList; }
	void createRoot();
protected:
	void registeTick() override { setFrameTick([this](float elapsedTime) {onFrameTick(elapsedTime); }); }
private:
	void onFrameTick(float elapsedTime);
protected:
	HashMap<DTConditionGroup*, Vector<DTNode*>> mMutexConditionList;// 互斥节点,当条件满足时,互斥的节点如果正在运行中,会被立即终止
	Vector<Vector<DTConditionGroup*>> mConditionGroup;				// 始终处于监听状态的条件,每个列表是一个条件组,当有一个条件满足时,后面的都不会再判断
	Vector<DTNode*> mNodeList;										// 存储该行为树的所有节点
	DTRepeater* mRoot = nullptr;									// 根节点
};