#include "FrameHeader.h"

void COMMonsterDecisionTree::destroy()
{
	base::destroy();
	clearTree();
}

void COMMonsterDecisionTree::clearTree()
{
	mDTNodePool->destroyClassList(mNodeList);
	mNodeList.clear();
	mMutexConditionList.clear();
	mConditionGroup.clear();
	mRoot = nullptr;
}

void COMMonsterDecisionTree::onFrameTick(const float elapsedTime)
{
	if (mRoot == nullptr)
	{
		return;
	}
	TASK_STATE state = mRoot->getState();
	if (state == TASK_STATE::NONE || state == TASK_STATE::RUNNING)
	{
		if (state != TASK_STATE::RUNNING)
		{
			state = mRoot->start();
		}
		if (state == TASK_STATE::RUNNING)
		{
			mRoot->update(elapsedTime);
		}
	}
	// 可能在上面节点的update中禁用了当前组件
	if (!mActive)
	{
		return;
	}

	// 更新所有需要一直监听的条件
	for (const auto& conditionList : mConditionGroup)
	{
		for (DTConditionGroup* conditionGroup : conditionList)
		{
			bool conditionSuccess = false;
			FOR_ONCE
			{
				if (conditionGroup->start() != TASK_STATE::SUCCESS)
				{
					break;
				}
				conditionSuccess = true;
				// 只有条件的父节点不处于运行中状态,才会执行后续逻辑
				// 这样是为了避免条件的判断会中断父节点中正在运行的子节点的运行
				DTNode* conditionParent = conditionGroup->getParent();
				if (conditionParent == nullptr || conditionParent->getState() == TASK_STATE::RUNNING)
				{
					break;
				}
				// 执行之前需要终止被互斥的节点
				if (auto* mutexNodeList = mMutexConditionList.getPtr(conditionGroup))
				{
					for (DTNode* node : *mutexNodeList)
					{
						node->abort();
					}
				}

				// 条件执行成功,则从该条件的下一个兄弟节点开始执行
				if (DTNode* sibling = conditionParent->getNextSibling(conditionGroup))
				{
					conditionParent->startExecute(sibling);
				}
			}
			// 如果有条件执行成功,则不再判断该组中后续的条件
			if (conditionSuccess)
			{
				break;
			}
		}
	}
}

void COMMonsterDecisionTree::setRelation(DTNode* parent, DTNode* child)
{
	parent->addChild(child);
	child->setParent(parent);
}

void COMMonsterDecisionTree::addListenCondition(DTConditionGroup* condition0, DTConditionGroup* condition1)
{
	ArrayList<2, DTConditionGroup*> list;
	list.add(condition0, condition1);
	addListenCondition(list);
}

void COMMonsterDecisionTree::addListenCondition(DTConditionGroup* condition)
{
	ArrayList<1, DTConditionGroup*> list;
	list.add(condition);
	addListenCondition(list);
}

void COMMonsterDecisionTree::createRoot()
{
	// 根节点默认为循环节点
	mRoot = static_cast<DTRepeater*>(createNode<DTRepeater>(nullptr, "行为树根节点"));
}

void COMMonsterDecisionTree::resetProperty()
{
	base::resetProperty();
	mMutexConditionList.clear();
	mConditionGroup.clear();
	mNodeList.clear();
	mRoot = nullptr;
}