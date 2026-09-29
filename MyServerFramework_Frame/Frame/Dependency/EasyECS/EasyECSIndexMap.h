#pragma once
#include "EasyECS.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>
#undef max
#undef min

struct EasyECSIndexMapStats
{
	uint64_t mFindCount = 0;
	uint64_t mFindProbeCount = 0;
	uint64_t mInsertCount = 0;
	uint64_t mInsertProbeCount = 0;
	uint64_t mEraseCount = 0;
	uint64_t mTombstoneProbeCount = 0;
	uint64_t mRehashCount = 0;
	uint64_t mMaxFindProbe = 0;
	uint64_t mMaxInsertProbe = 0;
	double getAverageFindProbe() const { return mFindCount == 0 ? 0.0 : static_cast<double>(mFindProbeCount) / static_cast<double>(mFindCount); }
	double getAverageInsertProbe() const { return mInsertCount == 0 ? 0.0 : static_cast<double>(mInsertProbeCount) / static_cast<double>(mInsertCount); }
};

template<typename TKey, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, bool TEnableStats = false, int TMaxLoadPercent = 75>
class EasyECSIndexMap
{
	static_assert(TMaxLoadPercent >= 50 && TMaxLoadPercent <= 90, "EasyECSIndexMap TMaxLoadPercent must be between 50 and 90");
public:
	explicit EasyECSIndexMap(int capacity = 4)
	{
		if (capacity < 1) capacity = 4;
		reserve(capacity);
	}
	~EasyECSIndexMap() { releaseTable(); }
	EasyECSIndexMap(const EasyECSIndexMap& other) : mHash(other.mHash), mEqual(other.mEqual), mStats(other.mStats) { copyTableFrom(other); }
	EasyECSIndexMap& operator=(const EasyECSIndexMap& other)
	{
		if (this == &other) return *this;
		EasyECSIndexMap copy(other);
		swap(copy);
		return *this;
	}
	EasyECSIndexMap(EasyECSIndexMap&& other) : mHash(other.mHash), mEqual(other.mEqual), mStats(other.mStats) { moveTableFrom(other); }
	EasyECSIndexMap& operator=(EasyECSIndexMap&& other)
	{
		if (this == &other) return *this;
		EasyECSIndexMap moved(std::move(other));
		swap(moved);
		return *this;
	}
	int size() const { return mCount; }
	bool empty() const { return mCount == 0; }
	int capacity() const { return static_cast<int>(mCapacity); }
	int tombstoneCount() const { return mDeletedCount; }
	static constexpr int maxLoadPercent() { return TMaxLoadPercent; }
	size_t memoryUsageBytes() const { return mCapacity * (sizeof(Slot) + sizeof(uint8_t)) + mIndexToSlot.capacity() * sizeof(uint32_t); }
	const EasyECSIndexMapStats& getStats() const { return mStats; }
	void resetStats() { mStats = EasyECSIndexMapStats{}; }
	EASY_ECS_FORCE_INLINE bool contains(const TKey& key) const { return findSlot(key) != INVALID_SLOT; }
	EASY_ECS_FORCE_INLINE int* findIndex(const TKey& key)
	{
		uint32_t slotIndex = findSlot(key);
		return slotIndex == INVALID_SLOT ? nullptr : &mSlots[slotIndex].mIndex;
	}
	EASY_ECS_FORCE_INLINE const int* findIndex(const TKey& key) const
	{
		uint32_t slotIndex = findSlot(key);
		return slotIndex == INVALID_SLOT ? nullptr : &mSlots[slotIndex].mIndex;
	}
	const TKey& getKeyByIndex(int index) const
	{
		assert(index >= 0 && index < mCount);
		return keyAt(mSlots[mIndexToSlot[static_cast<size_t>(index)]]);
	}
	bool tryAdd(const TKey& key, int index)
	{
		assert(index == mCount);
		ensureInsertCapacity();
		size_t hash = mHash(key);
		uint8_t hashTag = getHashTag(hash);
		uint32_t slotIndex = findInsertSlot(key, hash, hashTag);
		if (slotIndex == INVALID_SLOT) return false;
		bool reuseDeleted = mControl[slotIndex] == DELETED;
		new (&mSlots[slotIndex].mKey) TKey(key);
		mSlots[slotIndex].mIndex = index;
		mControl[slotIndex] = hashTag;
		mIndexToSlot.push_back(slotIndex);
		++mCount;
		if (reuseDeleted) --mDeletedCount;
		return true;
	}
	EASY_ECS_FORCE_INLINE int getOrAddIndex(const TKey& key, int newIndex, bool& added)
	{
		assert(newIndex == mCount);
		if (mCapacity == 0) ensureInsertCapacity();
		size_t hash = mHash(key);
		uint8_t hashTag = getHashTag(hash);
		size_t slotIndex = hash & mMask;
		uint32_t firstDeleted = INVALID_SLOT;
		uint32_t insertSlot = INVALID_SLOT;
		uint64_t probeCount = 0;
		uint64_t tombstoneProbeCount = 0;
		for (size_t probe = 0; probe < mCapacity; ++probe)
		{
			++probeCount;
			uint8_t control = mControl[slotIndex];
			if (control == EMPTY)
			{
				insertSlot = firstDeleted != INVALID_SLOT ? firstDeleted : static_cast<uint32_t>(slotIndex);
				break;
			}
			if (control == DELETED)
			{
				++tombstoneProbeCount;
				if (firstDeleted == INVALID_SLOT) firstDeleted = static_cast<uint32_t>(slotIndex);
			}
			else if (control == hashTag && mEqual(keyAt(mSlots[slotIndex]), key))
			{
				recordInsertProbe(probeCount, tombstoneProbeCount);
				added = false;
				return mSlots[slotIndex].mIndex;
			}
			slotIndex = (slotIndex + 1) & mMask;
		}
		if (insertSlot == INVALID_SLOT) insertSlot = firstDeleted;
		recordInsertProbe(probeCount, tombstoneProbeCount);
		bool rehashed = ensureInsertCapacity();
		if (rehashed)
		{
			insertSlot = findInsertSlot(key, hash, hashTag);
		}
		assert(insertSlot != INVALID_SLOT);
		if (insertSlot == INVALID_SLOT)
		{
			added = false;
			return -1;
		}
		insertNewSlot(insertSlot, key, newIndex, hashTag);
		added = true;
		return newIndex;
	}
	bool tryBuild(const TKey* keys, int count)
	{
		if (count < 0 || (count > 0 && keys == nullptr) || !empty()) return false;
		if (count == 0) return true;
		reserve(count);
		mIndexToSlot.reserve(static_cast<size_t>(count));
		for (int index = 0; index < count; ++index)
		{
			const TKey& key = keys[index];
			size_t hash = mHash(key);
			uint8_t hashTag = getHashTag(hash);
			uint32_t slotIndex = findBuildInsertSlot(key, hash, hashTag);
			if (slotIndex == INVALID_SLOT)
			{
				clearKeepCapacity();
				return false;
			}
			insertNewSlot(slotIndex, key, index, hashTag);
		}
		return true;
	}
	EASY_ECS_FORCE_INLINE bool erase(const TKey& key, int& removedIndex)
	{
		uint32_t slotIndex = findSlot(key);
		if (slotIndex == INVALID_SLOT) return false;
		removedIndex = mSlots[slotIndex].mIndex;
		eraseSlotSwapDense(slotIndex);
		return true;
	}
	EASY_ECS_FORCE_INLINE bool eraseByIndex(int index)
	{
		if (index < 0 || index >= mCount) return false;
		eraseSlotSwapDense(mIndexToSlot[static_cast<size_t>(index)]);
		return true;
	}
	void compactRemove(const uint8_t* removed, int oldSize)
	{
		if (removed == nullptr || oldSize <= 0 || mCount == 0) return;
		assert(oldSize == mCount);
		int writeIndex = 0;
		for (int readIndex = 0; readIndex < oldSize; ++readIndex)
		{
			uint32_t slotIndex = mIndexToSlot[static_cast<size_t>(readIndex)];
			if (removed[static_cast<size_t>(readIndex)] != 0)
			{
				keyAt(mSlots[slotIndex]).~TKey();
				mControl[slotIndex] = DELETED;
				++mDeletedCount;
				continue;
			}
			mSlots[slotIndex].mIndex = writeIndex;
			mIndexToSlot[static_cast<size_t>(writeIndex)] = slotIndex;
			++writeIndex;
		}
		mIndexToSlot.resize(static_cast<size_t>(writeIndex));
		mCount = writeIndex;
		if (mCount == 0) resetEmptyTable();
	}
	void clear() { clearKeepCapacity(); }
	void clearKeepCapacity()
	{
		if (mControl != nullptr)
		{
			for (size_t i = 0; i < mCapacity; ++i) if (isOccupied(mControl[i])) keyAt(mSlots[i]).~TKey();
			std::memset(mControl, EMPTY, mCapacity);
		}
		mIndexToSlot.clear();
		mCount = 0;
		mDeletedCount = 0;
	}
	void clearAndRelease()
	{
		releaseTable();
		std::vector<uint32_t>().swap(mIndexToSlot);
		mCount = 0;
		mDeletedCount = 0;
	}
	void reserve(int capacity)
	{
		if (capacity <= 0) return;
		size_t requiredCapacity = tableCapacityForCount(static_cast<size_t>(capacity));
		if (requiredCapacity > mCapacity) rehash(requiredCapacity);
	}
	void shrinkToFit()
	{
		size_t requiredCapacity = shrinkCapacityForCount(static_cast<size_t>(mCount));
		if (requiredCapacity < mCapacity) rehash(requiredCapacity);
		else if (mDeletedCount > 0) rehash(mCapacity);
	}
	void cleanupTombstones()
	{
		if (mDeletedCount > 0) rehash(mCapacity);
	}
private:
	using KeyStorage = typename std::aligned_storage<sizeof(TKey), alignof(TKey)>::type;
	struct Slot
	{
		int32_t mIndex;
		KeyStorage mKey;
	};
	static constexpr uint8_t EMPTY = 0x80;
	static constexpr uint8_t DELETED = 0xFE;
	static constexpr uint32_t INVALID_SLOT = std::numeric_limits<uint32_t>::max();
	static constexpr size_t MIN_CAPACITY = 8;
	static TKey& keyAt(Slot& slot) { return *std::launder(reinterpret_cast<TKey*>(&slot.mKey)); }
	static const TKey& keyAt(const Slot& slot) { return *std::launder(reinterpret_cast<const TKey*>(&slot.mKey)); }
	static bool isOccupied(uint8_t control) { return control < EMPTY; }
	static uint8_t getHashTag(size_t hash)
	{
		if constexpr (sizeof(size_t) > sizeof(uint32_t)) hash ^= hash >> 32;
		hash ^= hash >> 16;
		return static_cast<uint8_t>(hash & 0x7Fu);
	}
	static size_t maxCountForCapacity(size_t capacity)
	{
		return (capacity / 100) * static_cast<size_t>(TMaxLoadPercent) + ((capacity % 100) * static_cast<size_t>(TMaxLoadPercent)) / 100;
	}
	static size_t tableCapacityForCount(size_t count)
	{
		size_t capacity = MIN_CAPACITY;
		while (maxCountForCapacity(capacity) < count) capacity <<= 1;
		assert(capacity <= static_cast<size_t>(INVALID_SLOT));
		return capacity;
	}
	static size_t shrinkCapacityForCount(size_t count)
	{
		size_t capacity = MIN_CAPACITY;
		while ((capacity >> 1) < count) capacity <<= 1;
		assert(capacity <= static_cast<size_t>(INVALID_SLOT));
		return capacity;
	}
	void copyTableFrom(const EasyECSIndexMap& other)
	{
		std::vector<uint32_t> newIndexToSlot(other.mIndexToSlot);
		if (other.mCapacity == 0)
		{
			mIndexToSlot.swap(newIndexToSlot);
			mCapacity = 0;
			mMask = 0;
			mGrowThreshold = 0;
			mCount = 0;
			mDeletedCount = 0;
			return;
		}
		Slot* newSlots = new Slot[other.mCapacity];
		uint8_t* newControl = nullptr;
		try
		{
			newControl = new uint8_t[other.mCapacity];
			std::memset(newControl, EMPTY, other.mCapacity);
			for (size_t i = 0; i < other.mCapacity; ++i)
			{
				uint8_t control = other.mControl[i];
				if (isOccupied(control))
				{
					new (&newSlots[i].mKey) TKey(keyAt(other.mSlots[i]));
					newSlots[i].mIndex = other.mSlots[i].mIndex;
				}
				newControl[i] = control;
			}
		}
		catch (...)
		{
			if (newControl != nullptr)
			{
				for (size_t i = 0; i < other.mCapacity; ++i) if (isOccupied(newControl[i])) keyAt(newSlots[i]).~TKey();
			}
			delete[] newControl;
			delete[] newSlots;
			throw;
		}
		mSlots = newSlots;
		mControl = newControl;
		mIndexToSlot.swap(newIndexToSlot);
		mCapacity = other.mCapacity;
		mMask = other.mMask;
		mGrowThreshold = other.mGrowThreshold;
		mCount = other.mCount;
		mDeletedCount = other.mDeletedCount;
	}
	void moveTableFrom(EasyECSIndexMap& other)
	{
		mSlots = other.mSlots;
		mControl = other.mControl;
		mIndexToSlot = std::move(other.mIndexToSlot);
		mCapacity = other.mCapacity;
		mMask = other.mMask;
		mGrowThreshold = other.mGrowThreshold;
		mCount = other.mCount;
		mDeletedCount = other.mDeletedCount;
		other.mSlots = nullptr;
		other.mControl = nullptr;
		std::vector<uint32_t>().swap(other.mIndexToSlot);
		other.mCapacity = 0;
		other.mMask = 0;
		other.mGrowThreshold = 0;
		other.mCount = 0;
		other.mDeletedCount = 0;
		other.mStats = EasyECSIndexMapStats{};
	}
	void swap(EasyECSIndexMap& other)
	{
		using std::swap;
		swap(mHash, other.mHash);
		swap(mEqual, other.mEqual);
		swap(mStats, other.mStats);
		swap(mSlots, other.mSlots);
		swap(mControl, other.mControl);
		mIndexToSlot.swap(other.mIndexToSlot);
		swap(mCapacity, other.mCapacity);
		swap(mMask, other.mMask);
		swap(mGrowThreshold, other.mGrowThreshold);
		swap(mCount, other.mCount);
		swap(mDeletedCount, other.mDeletedCount);
	}
	void recordFindProbe(uint64_t probeCount, uint64_t tombstoneProbeCount) const
	{
		if constexpr (TEnableStats)
		{
			++mStats.mFindCount;
			mStats.mFindProbeCount += probeCount;
			mStats.mTombstoneProbeCount += tombstoneProbeCount;
			if (probeCount > mStats.mMaxFindProbe) mStats.mMaxFindProbe = probeCount;
		}
	}
	void recordInsertProbe(uint64_t probeCount, uint64_t tombstoneProbeCount)
	{
		if constexpr (TEnableStats)
		{
			++mStats.mInsertCount;
			mStats.mInsertProbeCount += probeCount;
			mStats.mTombstoneProbeCount += tombstoneProbeCount;
			if (probeCount > mStats.mMaxInsertProbe) mStats.mMaxInsertProbe = probeCount;
		}
	}
	bool ensureInsertCapacity()
	{
		if (mCapacity == 0)
		{
			rehash(MIN_CAPACITY);
			return true;
		}
		if (static_cast<size_t>(mDeletedCount) * 4 > static_cast<size_t>(mCount) * 3 && mDeletedCount >= 64)
		{
			rehash(mCapacity);
			return true;
		}
		if (static_cast<size_t>(mCount + mDeletedCount + 1) <= mGrowThreshold) return false;
		if (static_cast<size_t>(mCount + 1) <= mGrowThreshold) rehash(mCapacity);
		else rehash(mCapacity << 1);
		return true;
	}
	EASY_ECS_FORCE_INLINE uint32_t findSlot(const TKey& key) const
	{
		if (mCount == 0 || mCapacity == 0)
		{
			recordFindProbe(0, 0);
			return INVALID_SLOT;
		}
		size_t hash = mHash(key);
		uint8_t hashTag = getHashTag(hash);
		size_t slotIndex = hash & mMask;
		uint64_t probeCount = 0;
		uint64_t tombstoneProbeCount = 0;
		for (size_t probe = 0; probe < mCapacity; ++probe)
		{
			++probeCount;
			uint8_t control = mControl[slotIndex];
			if (control == EMPTY)
			{
				recordFindProbe(probeCount, tombstoneProbeCount);
				return INVALID_SLOT;
			}
			if (control == DELETED) ++tombstoneProbeCount;
			else if (control == hashTag && mEqual(keyAt(mSlots[slotIndex]), key))
			{
				recordFindProbe(probeCount, tombstoneProbeCount);
				return static_cast<uint32_t>(slotIndex);
			}
			slotIndex = (slotIndex + 1) & mMask;
		}
		recordFindProbe(probeCount, tombstoneProbeCount);
		return INVALID_SLOT;
	}
	uint32_t findBuildInsertSlot(const TKey& key, size_t hash, uint8_t hashTag)
	{
		size_t slotIndex = hash & mMask;
		uint64_t probeCount = 0;
		for (size_t probe = 0; probe < mCapacity; ++probe)
		{
			++probeCount;
			uint8_t control = mControl[slotIndex];
			if (control == EMPTY)
			{
				recordInsertProbe(probeCount, 0);
				return static_cast<uint32_t>(slotIndex);
			}
			assert(control != DELETED);
			if (control == hashTag && mEqual(keyAt(mSlots[slotIndex]), key))
			{
				recordInsertProbe(probeCount, 0);
				return INVALID_SLOT;
			}
			slotIndex = (slotIndex + 1) & mMask;
		}
		recordInsertProbe(probeCount, 0);
		return INVALID_SLOT;
	}
	uint32_t findInsertSlot(const TKey& key, size_t hash, uint8_t hashTag)
	{
		size_t slotIndex = hash & mMask;
		uint32_t firstDeleted = INVALID_SLOT;
		uint64_t probeCount = 0;
		uint64_t tombstoneProbeCount = 0;
		for (size_t probe = 0; probe < mCapacity; ++probe)
		{
			++probeCount;
			uint8_t control = mControl[slotIndex];
			if (control == EMPTY)
			{
				recordInsertProbe(probeCount, tombstoneProbeCount);
				return firstDeleted != INVALID_SLOT ? firstDeleted : static_cast<uint32_t>(slotIndex);
			}
			if (control == DELETED)
			{
				++tombstoneProbeCount;
				if (firstDeleted == INVALID_SLOT) firstDeleted = static_cast<uint32_t>(slotIndex);
			}
			else if (control == hashTag && mEqual(keyAt(mSlots[slotIndex]), key))
			{
				recordInsertProbe(probeCount, tombstoneProbeCount);
				return INVALID_SLOT;
			}
			slotIndex = (slotIndex + 1) & mMask;
		}
		recordInsertProbe(probeCount, tombstoneProbeCount);
		return firstDeleted;
	}
	EASY_ECS_FORCE_INLINE void insertNewSlot(uint32_t slotIndex, const TKey& key, int index, uint8_t hashTag)
	{
		bool reuseDeleted = mControl[slotIndex] == DELETED;
		new (&mSlots[slotIndex].mKey) TKey(key);
		mSlots[slotIndex].mIndex = index;
		mControl[slotIndex] = hashTag;
		mIndexToSlot.push_back(slotIndex);
		++mCount;
		if (reuseDeleted) --mDeletedCount;
	}
	EASY_ECS_FORCE_INLINE void eraseSlotSwapDense(uint32_t slotIndex)
	{
		if constexpr (TEnableStats) ++mStats.mEraseCount;
		int removeIndex = mSlots[slotIndex].mIndex;
		int lastIndex = mCount - 1;
		if (removeIndex != lastIndex)
		{
			uint32_t movedSlotIndex = mIndexToSlot[static_cast<size_t>(lastIndex)];
			mSlots[movedSlotIndex].mIndex = removeIndex;
			mIndexToSlot[static_cast<size_t>(removeIndex)] = movedSlotIndex;
		}
		mIndexToSlot.pop_back();
		keyAt(mSlots[slotIndex]).~TKey();
		mControl[slotIndex] = DELETED;
		--mCount;
		++mDeletedCount;
		if (mCount == 0) resetEmptyTable();
	}
	void resetEmptyTable()
	{
		if (mControl != nullptr) std::memset(mControl, EMPTY, mCapacity);
		mDeletedCount = 0;
		mIndexToSlot.clear();
	}
	void rehash(size_t newCapacity)
	{
		if (newCapacity < MIN_CAPACITY) newCapacity = MIN_CAPACITY;
		assert(newCapacity <= static_cast<size_t>(INVALID_SLOT));
		Slot* newSlots = new Slot[newCapacity];
		uint8_t* newControl = new uint8_t[newCapacity];
		std::memset(newControl, EMPTY, newCapacity);
		std::vector<uint32_t> newIndexToSlot;
		newIndexToSlot.reserve(maxCountForCapacity(newCapacity));
		newIndexToSlot.resize(static_cast<size_t>(mCount));
		size_t newMask = newCapacity - 1;
		try
		{
			for (size_t i = 0; i < mCapacity; ++i)
			{
				if (!isOccupied(mControl[i])) continue;
				const Slot& oldSlot = mSlots[i];
				size_t hash = mHash(keyAt(oldSlot));
				size_t newSlotIndex = hash & newMask;
				while (isOccupied(newControl[newSlotIndex])) newSlotIndex = (newSlotIndex + 1) & newMask;
				new (&newSlots[newSlotIndex].mKey) TKey(keyAt(oldSlot));
				newSlots[newSlotIndex].mIndex = oldSlot.mIndex;
				newControl[newSlotIndex] = getHashTag(hash);
				newIndexToSlot[static_cast<size_t>(oldSlot.mIndex)] = static_cast<uint32_t>(newSlotIndex);
			}
		}
		catch (...)
		{
			for (size_t i = 0; i < newCapacity; ++i) if (isOccupied(newControl[i])) keyAt(newSlots[i]).~TKey();
			delete[] newControl;
			delete[] newSlots;
			throw;
		}
		releaseTable();
		mSlots = newSlots;
		mControl = newControl;
		mCapacity = newCapacity;
		mMask = newMask;
		mGrowThreshold = maxCountForCapacity(newCapacity);
		mDeletedCount = 0;
		mIndexToSlot.swap(newIndexToSlot);
		if constexpr (TEnableStats) ++mStats.mRehashCount;
	}
	void releaseTable()
	{
		if (mControl != nullptr)
		{
			for (size_t i = 0; i < mCapacity; ++i) if (isOccupied(mControl[i])) keyAt(mSlots[i]).~TKey();
		}
		delete[] mControl;
		delete[] mSlots;
		mControl = nullptr;
		mSlots = nullptr;
		mCapacity = 0;
		mMask = 0;
		mGrowThreshold = 0;
		mDeletedCount = 0;
	}
	Slot* mSlots = nullptr;
	uint8_t* mControl = nullptr;
	std::vector<uint32_t> mIndexToSlot;
	size_t mCapacity = 0;
	size_t mMask = 0;
	size_t mGrowThreshold = 0;
	int mCount = 0;
	int mDeletedCount = 0;
	THash mHash;
	TEqual mEqual;
	mutable EasyECSIndexMapStats mStats;
};
