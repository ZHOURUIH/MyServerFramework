#ifdef WINDOWS
#include "FrameHeader.h"

namespace RandomSystemTestLocal
{
	class RandomSystemTestProxy : public RandomSystem
	{
	public:
		using RandomSystem::mCurIndex;
		using RandomSystem::mUpdateIndex;
		using RandomSystem::mRandomSequence;
	};
}

void RandomSystemTest::assertRandomSystem(bool condition, const char* message)
{
	using namespace RandomSystemTestLocal;
		if (!condition)
		{
			ERROR(string("RandomSystemTest failed: ") + message);
		}
	}

void RandomSystemTest::testConstructionAndSize()
{
	using namespace RandomSystemTestLocal;
		RandomSystemTestProxy system;
		assertRandomSystem(system.mRandomSequence.size() == 1024 * 100, "sequence size");
		assertRandomSystem(system.mCurIndex == 0, "cur index init");
		assertRandomSystem(system.mUpdateIndex == 0, "update index init");
	}

void RandomSystemTest::testRandomSequenceRead()
{
	using namespace RandomSystemTestLocal;
		RandomSystemTestProxy system;
		FOR(16)
		{
			system.mRandomSequence[i] = i * 10;
		}
		system.mCurIndex = 0;
		assertRandomSystem(system.random() == 0, "random read 0");
		assertRandomSystem(system.random() == 10, "random read 1");
		assertRandomSystem(system.random() == 20, "random read 2");
		assertRandomSystem(system.mCurIndex == 3, "cur index advanced");
	}

void RandomSystemTest::testRandomSequenceWrap()
{
	using namespace RandomSystemTestLocal;
		RandomSystemTestProxy system;
		const int size = system.mRandomSequence.size();
		for (int i = 0; i < size; ++i)
		{
			system.mRandomSequence[i] = i;
		}
		system.mCurIndex = size - 1;
		assertRandomSystem(system.random() == size - 1, "wrap read tail");
		assertRandomSystem(system.mCurIndex == size, "wrap pre-reset");
		assertRandomSystem(system.random() == 0, "wrap read head");
		assertRandomSystem(system.mCurIndex == 1, "wrap reset");
	}

void RandomSystemTest::testUpdateTouchesOneSlot()
{
	using namespace RandomSystemTestLocal;
		RandomSystemTestProxy system;
		FOR(8)
		{
			system.mRandomSequence[i] = 100 + i;
		}
		system.mUpdateIndex = 0;
		system.update(0.016f);
		assertRandomSystem(system.mUpdateIndex == 1, "update index advanced");
		system.update(0.016f);
		assertRandomSystem(system.mUpdateIndex == 2, "update index advanced 2");
	}

void RandomSystemTest::test()
{
	testConstructionAndSize();
	testRandomSequenceRead();
	testRandomSequenceWrap();
	testUpdateTouchesOneSlot();
}
#endif
