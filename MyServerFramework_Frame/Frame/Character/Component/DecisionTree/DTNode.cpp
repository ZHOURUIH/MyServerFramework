#include "FrameHeader.h"

DTNode::~DTNode()
{
	if (mChildList != nullptr)
	{
		delete mChildList;
		mChildList = nullptr;
	}
}

TASK_STATE DTNode::start()
{
#ifdef WINDOWS
	++mExecuteCount;
#endif
	mState = TASK_STATE::RUNNING;
	if (mNotifyParentRunning && mParent != nullptr)
	{
		mParent->notifyChildStart(this);
	}
	return mState;
}

TASK_STATE DTNode::update(const float elapsedTime) 
{
	if (mState != TASK_STATE::RUNNING)
	{
		ERROR("当前节点不处于运行状态,无法执行update");
		return TASK_STATE::FAILED;
	}
	return TASK_STATE::SUCCESS; 
}

void DTNode::notifyChildStart(DTNode* child)
{
	// 子节点开始执行时,如果当前节点还未执行,则设置为执行状态
	if (mState != TASK_STATE::RUNNING)
	{
		mState = TASK_STATE::RUNNING;
		if (mNotifyParentRunning && mParent != nullptr)
		{
			mParent->notifyChildStart(this);
		}
	}
}

TASK_STATE DTNode::startExecute(DTNode* startChild)
{
	// 执行之前会终止自己的所有节点
	abort();
	// 子节点正在运行,则当前节点也会进入运行状态,并且需要通知父节点自己开始执行
	start();
	return mState;
}

void DTNode::addChild(DTNode* child)
{
	if (mDestroy)
	{
		ERROR("当前对象已经销毁,无法再访问");
	}
	if (mChildList == nullptr)
	{
		mChildList = new Vector<DTNode*>();
	}
	mChildList->push_back(child);
}

void DTNode::clearChild() 
{
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			child->setParent(nullptr);
		}
		mChildList->clear();
	}
}

void DTNode::reset()
{
	if (mState == TASK_STATE::NONE)
	{
		return;
	}
	// 重置所有子节点
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			child->reset();
		}
	}
	mState = TASK_STATE::NONE;
}

void DTNode::resetProperty()
{
	base::resetProperty();
	if (mChildList != nullptr)
	{
		mChildList->clear();
	}
	mCharacter = nullptr;
	mType = nullptr;
	mParent = nullptr;
	mState = TASK_STATE::NONE;
#ifdef WINDOWS
	mDebugString = nullptr;
	mExecuteCount = 0;
#endif
	mNotifyParentRunning = true;
}

void DTNode::setParent(DTNode* parent)
{
	if (parent != nullptr && mParent != nullptr && mParent != parent)
	{
		ERROR("已经有父节点了,不能修改父节点");
		return;
	}
	mParent = parent; 
}

void DTNode::setFailed()
{
	mState = TASK_STATE::FAILED;
	// 将子节点设置为失败
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			child->setFailed();
		}
	}
}

void DTNode::setSuccess()
{
	mState = TASK_STATE::SUCCESS;
	// 将子节点设置为失败
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			child->setSuccess();
		}
	}
}

DTNode* DTNode::getNextSibling(DTNode* node)
{
	if (mChildList == nullptr)
	{
		return nullptr;
	}
	const int index = mChildList->findFirstIndex(node);
	if (index < 0 || index == mChildList->size() - 1)
	{
		return nullptr;
	}
	return (*mChildList)[index + 1];
}

void DTNode::abort()
{
	// 中断所有子节点
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			child->abort();
		}
	}

	// 中断后还需要进行重置
	reset();
}