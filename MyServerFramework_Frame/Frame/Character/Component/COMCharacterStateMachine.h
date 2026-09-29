#pragma once

#include "GameComponent.h"
#include "FrameStringDefine.h"
#include "FrameDefine.h"
#include "CharacterState.h"
#include "Ref.h"

class CharacterStateListView
{
public:
	class Iterator
	{
	public:
		Iterator(CharacterState* singleState, const Vector<CharacterState*>* stateList, int index):
			mSingleState(singleState),
			mStateList(stateList),
			mIndex(index)
		{}
		CharacterState* operator*() const { return mStateList != nullptr ? (*mStateList)[mIndex] : mSingleState; }
		Iterator& operator++() { ++mIndex; return *this; }
		bool operator==(const Iterator& other) const { return mIndex == other.mIndex; }
		bool operator!=(const Iterator& other) const { return mIndex != other.mIndex; }
	protected:
		CharacterState* mSingleState = nullptr;
		const Vector<CharacterState*>* mStateList = nullptr;
		int mIndex = 0;
	};
public:
	CharacterStateListView() = default;
	explicit CharacterStateListView(CharacterState* singleState):mSingleState(singleState) {}
	explicit CharacterStateListView(const Vector<CharacterState*>* stateList):mStateList(stateList) {}
	int size() const { return mStateList != nullptr ? mStateList->size() : (mSingleState != nullptr ? 1 : 0); }
	bool isEmpty() const { return size() == 0; }
	Iterator begin() const { return Iterator(mSingleState, mStateList, 0); }
	Iterator end() const { return Iterator(mSingleState, mStateList, size()); }
protected:
	CharacterState* mSingleState = nullptr;
	const Vector<CharacterState*>* mStateList = nullptr;
};
class StateTypeStateListView
{
public:
	StateTypeStateListView() = default;
	explicit StateTypeStateListView(CharacterState* singleState):mSingleState(singleState) {}
	explicit StateTypeStateListView(const SafeVector<CharacterState*>* coexistStateList):mCoexistStateList(coexistStateList) {}
	void setSingle(CharacterState* state)
	{
		mSingleState = state;
		mCoexistStateList = nullptr;
	}
	void setCoexist(const SafeVector<CharacterState*>* stateList)
	{
		mSingleState = nullptr;
		mCoexistStateList = stateList;
	}
	CharacterStateListView getMainList() const
	{
		return mCoexistStateList != nullptr ? CharacterStateListView(&mCoexistStateList->getMainList()) : CharacterStateListView(mSingleState);
	}
	int size() const { return mCoexistStateList != nullptr ? mCoexistStateList->size() : (mSingleState != nullptr ? 1 : 0); }
	bool isEmpty() const { return size() == 0; }
protected:
	CharacterState* mSingleState = nullptr;
	const SafeVector<CharacterState*>* mCoexistStateList = nullptr;
};
struct StateTypeListViewItem
{
	ushort first = 0;
	StateTypeStateListView second;
	StateTypeListViewItem(ushort type, CharacterState* state):first(type), second(state) {}
	StateTypeListViewItem(ushort type, const SafeVector<CharacterState*>* stateList):first(type), second(stateList) {}
};
class StateTypeListView
{
public:
	typedef HashMap<ushort, CharacterState*> SingleStateList;
	typedef HashMap<ushort, SafeVector<CharacterState*>> CoexistStateList;
	class Iterator
	{
	public:
		typedef SingleStateList::const_iterator SingleIterator;
		typedef CoexistStateList::const_iterator CoexistIterator;
	public:
		Iterator(const SingleStateList& singleStateList, const CoexistStateList& coexistStateList, bool end):
			mSingleIter(end ? singleStateList.end() : singleStateList.begin()),
			mSingleEnd(singleStateList.end()),
			mCoexistIter(end ? coexistStateList.end() : coexistStateList.begin()),
			mCoexistEnd(coexistStateList.end()),
			mInSingle(!end)
		{
			moveToValid();
		}
		StateTypeListViewItem operator*() const
		{
			return mInSingle ? StateTypeListViewItem(mSingleIter->first, mSingleIter->second) : StateTypeListViewItem(mCoexistIter->first, &mCoexistIter->second);
		}
		Iterator& operator++()
		{
			if (mInSingle)
			{
				++mSingleIter;
			}
			else
			{
				++mCoexistIter;
			}
			moveToValid();
			return *this;
		}
		bool operator==(const Iterator& other) const
		{
			if (mInSingle != other.mInSingle)
			{
				return false;
			}
			return mInSingle ? mSingleIter == other.mSingleIter : mCoexistIter == other.mCoexistIter;
		}
		bool operator!=(const Iterator& other) const { return !(*this == other); }
	protected:
		void moveToValid()
		{
			if (mInSingle)
			{
				while (mSingleIter != mSingleEnd && mSingleIter->second == nullptr)
				{
					++mSingleIter;
				}
				if (mSingleIter == mSingleEnd)
				{
					mInSingle = false;
				}
			}
			if (!mInSingle)
			{
				while (mCoexistIter != mCoexistEnd && mCoexistIter->second.isEmpty())
				{
					++mCoexistIter;
				}
			}
		}
	protected:
		SingleIterator mSingleIter;
		SingleIterator mSingleEnd;
		CoexistIterator mCoexistIter;
		CoexistIterator mCoexistEnd;
		bool mInSingle = false;
	};
public:
	StateTypeListView(const SingleStateList& singleStateList, const CoexistStateList& coexistStateList):
		mSingleStateList(singleStateList),
		mCoexistStateList(coexistStateList)
	{}
	Iterator begin() const { return Iterator(mSingleStateList, mCoexistStateList, false); }
	Iterator end() const { return Iterator(mSingleStateList, mCoexistStateList, true); }
protected:
	const SingleStateList& mSingleStateList;
	const CoexistStateList& mCoexistStateList;
};

class MICRO_LEGEND_FRAME_API COMCharacterStateMachine : public GameComponent
{
	BASE(COMCharacterStateMachine, GameComponent);
public:
	static ushort staticType() { return FrameStringDefine::COMCharacterStateMachine; }
	~COMCharacterStateMachine() override;
	void destroy() override;
	void resetProperty() override;
	bool canAddState(ushort type, StateParam* param) const;
	Ref<CharacterState> addState(ushort type, StateParam* param, float stateTime, bool removeByDie);
	template<typename T, typename TypeCheck = typename IsSubClassOf<CharacterState, T>::mType>
	bool removeState(Ref<T>&& stateRef, bool isBreak, bool removeByDie)
	{
		if (!stateRef.isValid())
		{
			return false;
		}
		bool result = removeStateInternal(stateRef.mPtr, isBreak, removeByDie);
		stateRef.clear();
		return result;
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<CharacterState, T>::mType>
	bool removeState(Ref<T>& stateRef, bool isBreak, bool removeByDie)
	{
		if (!stateRef.isValid())
		{
			return false;
		}
		bool result = removeStateInternal(stateRef.mPtr, isBreak, removeByDie);
		stateRef.clear();
		return result;
	}
	void removeStateType(ushort type, bool isBreak, bool removeByDie);
	// 移除一个状态组中的所有状态
	void removeStateInGroup(int group, bool isBreak, bool removeByDie);
	// 移除指定类型的第一个状态
	bool removeFirstState(int buffID, bool isBreak, bool removeByDie);
	CharacterState* getFirstState(ushort type) const;
	// 是否拥有该组的任意一个状态
	bool hasStateGroup(int group)const;
	bool hasState(ushort type) const;
	// 获取指定类型的状态列表视图,不复制状态数据
	StateTypeStateListView* getState(ushort type);
	const StateTypeStateListView* getState(ushort type) const;
	// 获取全部状态的组合视图,外部仍然可以使用iter.first和iter.second.getMainList()进行遍历
	StateTypeListView getStateList() const { return StateTypeListView(mSingleStateTypeList, mCoexistStateTypeList); }
protected:
	// isBreak表示是否是因为与要添加的状态冲突,所以才移除的状态
	bool removeStateInternal(CharacterState* stateRef, bool isBreak, bool removeByDie);
	static void leaveStateInternal(CharacterState* state, bool isBreak, bool willDestroy, bool removeByDie);
	static void destroyState(CharacterState* state, int line);
	static void destroyStateList(Vector<CharacterState*>& stateList, int line);
	bool canAddState(CharacterState* state, StateParam* param, CharacterState** needRemoveState) const;
	// 是否因为与当前已存在的同类型的互斥而无法添加此状态,返回值表示是否可以添加新的状态,如果可添加,则needRemoveState返回需要被移除的状态
	bool mutexWithExistState(CharacterState* state, CharacterState** needRemoveState) const;
	// 添加了newState以后是否还会保留existState
	static bool allowKeepStateByGroup(ushort newState, ushort existState);
	static bool allowKeepStateByGroupInternal(ushort newState, ushort existState);
	// 当存在existState时,是否允许添加newState
	static bool allowAddStateByGroup(ushort newState, ushort existState);
	static bool allowAddStateByGroupInternal(ushort newState, ushort existState);
	static void initAllowKeepTypeList();
	static void initAllowAddTypeList();
	void registeTick() override
	{
		// 没有需要每帧更新的状态时不注册FrameTick,避免角色创建时为一个空状态机创建Tick容器
		if (mStateTickCount > 0)
		{
			setFrameTick([this](float elapsedTime) { onFrameTick(elapsedTime); });
		}
	}
private:
	void onFrameTick(float elapsedTime);
protected:
	HashMap<ushort, CharacterState*> mSingleStateTypeList;				// 同类型不能共存的状态,每种类型最多保存一个状态
	HashMap<ushort, SafeVector<CharacterState*>> mCoexistStateTypeList;	// STATE_MUTEX::COEXIST状态,同类型允许保存多个状态
	mutable StateTypeStateListView mGetStateView;						// getState使用的轻量临时视图,不保存状态数据
	SafeVector<CharacterState*>* mStateTickList = nullptr;				// 用来更新遍历,只存储需要调用update的状态,没有Tick状态时不创建
	HashMap<short, short> mGroupStateCountList;							// 每个状态组中的状态数量
	short mStateTickCount = 0;											// mStateTickList的元素个数,单独使用一个变量记录,比mStateTickList.size()访问更快
private:
	static ushort* mAllowAddTypeCountList;								// 以类型为下标,记录此类型有多少个互斥的类型
	static ushort* mAllowKeepTypeCountList;								// 以类型为下标,记录此类型有多少个允许共存的类型
	static HashMap<uint, bool> mAllowAddTypeList;						// allowAddStateByGroupInternal预先计算出来的列表,可以根据类型索引,避免运行时做过多的判断,key是两个类型的字节组合
	static HashMap<uint, bool> mAllowKeepTypeList;						// allowKeepStateByGroupInternal预先计算出来的列表,可以根据类型索引,避免运行时做过多的判断,key是两个类型的字节组合
};