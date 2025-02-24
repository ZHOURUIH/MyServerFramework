#include "FrameHeader.h"

void CommandSystem::quit()
{
	{
		THREAD_LOCK(mDelayCommandListLock);
		for (const DelayCommand* delayInfo : mDelayCommandList)
		{
			destroyCmd(delayInfo->mCommand);
		}
		mDelayCommandPool->destroyClassList(mDelayCommandList);
		mDelayCommandList.clear();
	}

	{
		THREAD_LOCK(mThreadCommandListLock);
		for (auto& item : mThreadCommandList)
		{
			for (const DelayCommand* delayInfo : item.second)
			{
				destroyCmd(delayInfo->mCommand);
			}
			mDelayCommandPool->destroyClassList(item.second);
			item.second.clear();
		}
		mThreadCommandList.clear();
	}
}

DelayCommand* CommandSystem::createDelayCommand(CommandReceiver* receiver, CustomThread* thread, GameCommand* cmd, const float delayTime)
{
	DelayCommand* delayCommand = mDelayCommandPool->newClass();
	delayCommand->init(receiver, thread, cmd, delayTime);
	return delayCommand;
}

void CommandSystem::destroyDelayCommand(DelayCommand* delayCommand)
{
	mDelayCommandPool->destroyClass(delayCommand);
}

void CommandSystem::destroyCmd(GameCommand* cmd)
{
	if (cmd == nullptr)
	{
		return;
	}
	if (!cmd->isThreadCommand())
	{
		mCommandPool->destroyClass(cmd);
	}
	else
	{
		mCommandThreadPool->destroyClass(cmd);
	}
}

void CommandSystem::update(const float elapsedTime)
{
	const llong time0 = getRealTimeMS();
	// 执行之前需要先清空列表,为了避免在执行命令的过程中又向mDelayCommandList添加了命令而造成错误,所以需要单独使用一个列表存储
	mExecuteList.clear();
	// 开始处理命令处理列表
	if (mDelayCommandList.size() > 0)
	{
		THREAD_LOCK(mDelayCommandListLock);
		for (int i = 0; i < mDelayCommandList.size(); ++i)
		{
			DelayCommand* delayCmd = mDelayCommandList[i];
			if (delayCmd == nullptr)
			{
				continue;
			}
			if (delayCmd->isDestroy())
			{
				ERROR_PROFILE((string("0正在处理已经被销毁的命令:") + typeid(*delayCmd).name()).c_str());
				continue;
			}
			delayCmd->mDelayTime -= elapsedTime;
			if (delayCmd->mDelayTime <= 0.0f)
			{
				// 命令的延迟执行时间到了,则执行命令
				mExecuteList.push_back(delayCmd);
				mDelayCommandList.eraseAt(i--);
			}
		}
	}
	const llong time1 = getRealTimeMS();
	if (time1 - time0 > 1)
	{
		INT_STR(timeStr, (int)(time1 - time0));
		LOG(string("处理命令延迟列表耗时:") + timeStr.str() + "毫秒");
	}

	// 执行所有需要执行的命令
	int executeCount = 0;
	for (DelayCommand* delayCmd : mExecuteList)
	{
		if (delayCmd == nullptr)
		{
			continue;
		}
		if (delayCmd->isDestroy())
		{
			ERROR_PROFILE((string("1正在执行一个已经被销毁的命令:") + typeid(*delayCmd).name()).c_str());
			continue;
		}
		GameCommand* cmd = delayCmd->mCommand;
		cmd->setDelayCommand(false);
		if (delayCmd->mReceiver != nullptr)
		{
			++executeCount;
			executeCommand(cmd, delayCmd->mReceiver);
		}
		else
		{
			// 销毁回收命令
			destroyCmd(cmd);
		}
	}
	mDelayCommandPool->destroyClassList(mExecuteList);
	mExecuteList.clear();
	const llong time2 = getRealTimeMS();
	if (time2 - time1 > 10)
	{
		INT_STR(countStr, executeCount);
		INT_STR(timeStr, (int)(time2 - time1));
		LOG(string("处理所有命令执行耗时:") + timeStr.str() + "毫秒, 命令个数:" + countStr.str());
	}

	// 子线程命令列表
	const llong time3 = getRealTimeMS();
	{
		THREAD_LOCK(mThreadCommandListLock);
		for (auto& item : mThreadCommandList)
		{
			auto& cmdList = item.second;
			for (int i = 0; i < cmdList.size(); ++i)
			{
				DelayCommand* delayCmd = cmdList[i];
				if (delayCmd->mReceiver != nullptr)
				{
					delayCmd->mDelayTime -= elapsedTime;
					if (delayCmd->mDelayTime <= 0.0f)
					{
						item.first->addCmd(delayCmd);
						cmdList.eraseAt(i--);
					}
				}
				// 被中断的命令
				else
				{
					destroyCmd(delayCmd->mCommand);
					destroyDelayCommand(delayCmd);
					cmdList.eraseAt(i--);
				}
			}
		}
	}
	const llong time4 = getRealTimeMS();
	if (time4 - time3 > 3)
	{
		INT_STR(timeStr, (int)(time4 - time3));
		LOG(string("处理子线程命令延迟列表耗时:") + timeStr.str() + "毫秒");
	}
}

bool CommandSystem::interruptCommand(const llong assignID)
{
	if (!isMainThread())
	{
		ERROR("不能在子线程中中断命令");
		return true;
	}
	// 如果命令系统已经销毁了,则不能再中断命令
	if (mDestroied)
	{
		return true;
	}
	if (assignID < 0)
	{
		ERROR("assignID invalid! : " + LLToS(assignID));
		return false;
	}
	bool ret = false;

	// 主线程命令列表
	{
		THREAD_LOCK(mDelayCommandListLock);
		FOR_VECTOR(mDelayCommandList)
		{
			DelayCommand* delayInfo = mDelayCommandList[i];
			GameCommand* cmd = delayInfo->mCommand;
			if (cmd->getAssignID() == assignID)
			{
				MyString<1024> infoStr;
				cmd->debugInfo(infoStr);
				LLONG_STR(assignIDStr, assignID);
				MyString<512> totalInfo;
				strcat_t(totalInfo, "CMD: interrupt:", assignIDStr.str(), ", ", infoStr.str(), ", receiver:", delayInfo->mReceiver->getPrintName().c_str());
				LOG(totalInfo.str());

				cmd->onInterrupted();
				// 销毁回收命令
				destroyCmd(cmd);
				mDelayCommandList.eraseAt(i);
				destroyDelayCommand(delayInfo);
				ret = true;
				break;
			}
		}
	}

	// 子线程命令列表
	if (!ret)
	{
		THREAD_LOCK(mThreadCommandListLock);
		for (auto& iterCmd : mThreadCommandList)
		{
			auto& cmdList = iterCmd.second;
			for (DelayCommand* delayInfo : cmdList)
			{
				GameCommand* cmd = delayInfo->mCommand;
				if (cmd->getAssignID() == assignID)
				{
					cmd->onInterrupted();
					delayInfo->mReceiver = nullptr;
					ret = true;
					break;
				}
			}
			if (ret)
			{
				break;
			}
		}
	}

	// 在即将执行的列表中查找
	if (!ret)
	{
		for (DelayCommand* delayInfo : mExecuteList)
		{
			GameCommand* cmd = delayInfo->mCommand;
			if (cmd->getAssignID() == assignID)
			{
				cmd->onInterrupted();
				delayInfo->mReceiver = nullptr;
				ret = true;
				break;
			}
		}
	}
	if (!ret)
	{
		ERROR("not find cmd with assignID! " + LLToS(assignID));
	}
	return ret;
}

void CommandSystem::pushCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const
{
#ifdef WINDOWS
	if (!checkCmd(cmd, cmdReceiver))
	{
		destroyCmd(cmd);
		return;
	}
#endif
	if (cmdReceiver == nullptr)
	{
		destroyCmd(cmd);
		return;
	}
	cmd->setCommandState(COMMAND_STATE::PUSHED);
	cmd->onPush();
	executeCommand(cmd, cmdReceiver);
}

void CommandSystem::pushCommandDelay(GameCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute)
{
#ifdef WINDOWS
	if (!checkDelayCmd(cmd))
	{
		destroyCmd(cmd);
		return;
	}
#endif
	cmd->setCommandState(COMMAND_STATE::PUSHED);
	cmd->onPush();
	clampMinRef(delayExecute);
	if (cmd->getShowDebugInfo())
	{
		MyString<1024> info;
		cmd->debugInfo(info);
		LLONG_STR(assignIDStr, cmd->getAssignID());
		FLOAT_STR(delayTimeStr, delayExecute);
		MyString<2048> allInfo;
		strcat_t(allInfo, "CMD : delay cmd:", assignIDStr.str(), ", ", delayTimeStr.str(), ", info:", info.str(), "receiver:", cmdReceiver->getPrintName().c_str());
		LOG(allInfo.str());
	}
	{
		THREAD_LOCK(mDelayCommandListLock);
		mDelayCommandList.push_back(createDelayCommand(cmdReceiver, nullptr, cmd, delayExecute));
	}
}

void CommandSystem::pushCommandThread(GameCommand* cmd, CommandReceiver* cmdReceiver, CustomThread* thread)
{
	if (thread == nullptr)
	{
		destroyCmd(cmd);
		return;
	}
#ifdef WINDOWS
	if (!checkCmd(cmd, cmdReceiver))
	{
		destroyCmd(cmd);
		return;
	}
#endif
	cmd->setCommandState(COMMAND_STATE::PUSHED);
	cmd->onPush();
	{
		THREAD_LOCK(mThreadCommandListLock);
		mThreadCommandList.insertOrGet(thread).push_back(createDelayCommand(cmdReceiver, thread, cmd, 0.0f));
	}
}

void CommandSystem::pushCommandThreadDelay(GameCommand* cmd, CommandReceiver* cmdReceiver, CustomThread* thread, const float delayExecute)
{
#ifdef WINDOWS
	if (!checkDelayCmd(cmd))
	{
		destroyCmd(cmd);
		return;
	}
#endif
	cmd->setCommandState(COMMAND_STATE::PUSHED);
	cmd->onPush();
	{
		THREAD_LOCK(mThreadCommandListLock);
		mThreadCommandList.insertOrGet(thread).push_back(createDelayCommand(cmdReceiver, thread, cmd, delayExecute));
	}
}

void CommandSystem::notifyReceiverDestroied(CommandReceiver* receiver)
{
	if (!isMainThread())
	{
		ERROR("不能在子线程中销毁命令接收者");
		return;
	}
	if (mDestroied)
	{
		return;
	}
	// 延迟命令列表中
	{
		THREAD_LOCK(mDelayCommandListLock);
		for (int i = 0; i < mDelayCommandList.size(); ++i)
		{
			DelayCommand* delayInfo = mDelayCommandList[i];
			if (delayInfo->mReceiver == receiver)
			{
				destroyCmd(delayInfo->mCommand);
				destroyDelayCommand(delayInfo);
				mDelayCommandList.eraseAt(i--);
			}
		}
	}
	// 执行列表中
	for (DelayCommand* delayInfo : mExecuteList)
	{
		// 已执行或正在执行的命令不作判断,该列表无法删除元素,只能将命令销毁,然后在执行时判断
		if (delayInfo != nullptr && 
			delayInfo->mReceiver == receiver && 
			delayInfo->mCommand->getCommandState() == COMMAND_STATE::PUSHED)
		{
			delayInfo->mReceiver = nullptr;
		}
	}
}

void CommandSystem::executeCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const
{
	// 如果命令系统已经销毁,则直接销毁命令对象
	if (mDestroied)
	{
		destroyCmd(cmd);
		return;
	}

#ifdef WINDOWS
	if (!checkExecuteCmd(cmd, cmdReceiver))
	{
		destroyCmd(cmd);
		return;
	}
#endif
	cmd->setReceiver(cmdReceiver);
	if (cmd->getShowDebugInfo())
	{
		MyString<1024> info;
		cmd->debugInfo(info);
		LLONG_STR(assignIDStr, cmd->getAssignID());
		MyString<2048> allInfo;
		strcat_t(allInfo, "CMD: ", assignIDStr.str(), ", ", info.str(), ", receiver:", cmdReceiver->getPrintName().c_str());
		LOG(allInfo.str());
	}
	cmd->runStartCallBack();
	cmd->setCommandState(COMMAND_STATE::EXECUTING);
	const llong time0 = getRealTimeMS();
	cmd->execute();
	const llong time1 = getRealTimeMS();
	if (time1 - time0 > 10)
	{
		INT_STR(timeStr, (int)(time1 - time0));
		LOG("执行时长超过10毫秒的命令:" + string(timeStr.str()) + ", 命令:" + typeid(*cmd).name());
	}
	cmd->setCommandState(COMMAND_STATE::EXECUTED);
	cmd->runEndCallBack();
	// 销毁回收命令
	destroyCmd(cmd);
}

bool CommandSystem::checkExecuteCmd(const GameCommand* cmd, CommandReceiver* cmdReceiver) const
{
	if (cmd == nullptr)
	{
		const string& receiverName = (cmdReceiver != nullptr) ? cmdReceiver->getPrintName() : FrameDefine::EMPTY;
		ERROR("cmd is null! receiver : " + receiverName);
		return false;
	}
	if (cmdReceiver == nullptr)
	{
		ERROR("receiver is null! cmd : " + string(typeid(*cmd).name()));
		return false;
	}
	if (cmd->isDestroy())
	{
		ERROR("cmd is invalid! make sure create cmd use CommandSystem.newCmd! cmd type : " + string(typeid(*cmd).name()) + ", assign id : " + LLToS(cmd->getAssignID()));
		return false;
	}
	if (cmd->getCommandState() != COMMAND_STATE::PUSHED)
	{
		ERROR("cmd state is not PUSHED, can not execute");
		return false;
	}
	return true;
}

bool CommandSystem::checkCmd(GameCommand* cmd, CommandReceiver* cmdReceiver) const
{
	if (cmd->getCommandState() != COMMAND_STATE::NONE)
	{
		ERROR("cmd state is not NONE");
		return false;
	}
	if (cmd->isDelayCommand())
	{
		MyString<1024> info;
		cmd->debugInfo(info);
		LLONG_STR(assignIDStr, cmd->getAssignID());
		MyString<2048> allInfo;
		strcat_t(allInfo, "cmd is a delay cmd! can not use pushCommand! assign id: ", assignIDStr.str(), ", info:", info.str());
		ERROR(allInfo.str());
		return false;
	}
	return true;
}

bool CommandSystem::checkDelayCmd(GameCommand* cmd) const
{
	if (cmd->getCommandState() != COMMAND_STATE::NONE)
	{
		ERROR("cmd state is not NONE");
		return false;
	}
	if (!cmd->isDelayCommand())
	{
		MyString<1024> info;
		cmd->debugInfo(info);
		LLONG_STR(assignIDStr, cmd->getAssignID());
		MyString<2048> allInfo;
		strcat_t(allInfo, "cmd is not a delay command, Command: ", assignIDStr.str(), ", info:", info.str());
		ERROR(allInfo.str());
		return false;
	}
	return true;
}