#pragma once

#include "GameComponent.h"
#include "FrameStringDefine.h"
#include "FrameDefine.h"
#include "DTConditionGroup.h"
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
		mNodeList.push_back(node);
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
	void addConditionMutex(DTConditionGroup* condition, DTNode* mutexNodex)
	{
		mMutexConditionList.insertOrGet(condition).push_back(mutexNodex);
	}
	template<int Length>
	void addListenCondition(const ArrayList<Length, DTConditionGroup*>& conditionList)
	{
		// 只作为条件监听的条件组是不需要向父节点通知开始执行的
		mConditionGroup.push_back();
		auto& lastGroup = mConditionGroup[mConditionGroup.size() - 1];
		const int count = conditionList.size();
		lastGroup.reserve(count);
		FOR_I(count)
		{
			DTConditionGroup* conditionGroup = conditionList[i];
			conditionGroup->setNotifyParentRunning(false);
			lastGroup.push_back(conditionGroup);
		}
	}
	void addListenCondition(DTConditionGroup* condition0, DTConditionGroup* condition1);
	void addListenCondition(DTConditionGroup* condition);
	DTRepeater* getRoot() const { return mRoot; }
	const Vector<DTNode*>& getNodeList() const { return mNodeList; }
	void createRoot();
protected:
	void registeTick() override { mComponentOwner->registeFrameTick(this, frameTick); }
	void unregisteTick() override { mComponentOwner->unregisteFrameTick(this, frameTick); }
	static void frameTick(GameComponent* component, float elapsedTime) { static_cast<This*>(component)->onFrameTick(elapsedTime); }
private:
	void onFrameTick(float elapsedTime);
protected:
	HashMap<DTConditionGroup*, Vector<DTNode*>> mMutexConditionList;// 互斥节点,当条件满足时,互斥的节点如果正在运行中,会被立即终止
	Vector<Vector<DTConditionGroup*>> mConditionGroup;				// 始终处于监听状态的条件,每个列表是一个条件组,当有一个条件满足时,后面的都不会再判断
	Vector<DTNode*> mNodeList;										// 存储该行为树的所有节点
	DTRepeater* mRoot = nullptr;									// 根节点
};