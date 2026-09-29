#ifdef WINDOWS
#include "FrameHeader.h"

namespace DoubleBufferTestSupport
{
#define DoubleBufferTestSupport_DBT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("DoubleBufferTest FAILED: ") + (msg)); }
}

void DoubleBufferTest::testDoubleBuffer_BasicAddAndRead()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	// 初始状态
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "init getAllDataCount==0");
	DoubleBufferTestSupport_DBT_ASSERT(!buf.tryCheckDataToRead(), "init tryCheckDataToRead==false");

	// add 单个元素
	buf.add(10);
	buf.add(20);
	buf.add(30);

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 3, "after 3 adds, getAllDataCount==3");
	DoubleBufferTestSupport_DBT_ASSERT(buf.tryCheckDataToRead(), "after adds, tryCheckDataToRead==true");

	// startRead / endRead
	Vector<int>* readList = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(readList != nullptr, "startRead returns non-null");
	DoubleBufferTestSupport_DBT_ASSERT(readList->size() == 3, "readList size==3");
	DoubleBufferTestSupport_DBT_ASSERT((*readList)[0] == 10, "readList[0]==10");
	DoubleBufferTestSupport_DBT_ASSERT((*readList)[1] == 20, "readList[1]==20");
	DoubleBufferTestSupport_DBT_ASSERT((*readList)[2] == 30, "readList[2]==30");
	buf.endRead();

	// 读完后继续写
	buf.add(100);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() >= 1, "after read+add, count>=1");
}

void DoubleBufferTest::testDoubleBuffer_AddVector()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	Vector<int> src;
	src.add(1);
	src.add(2);
	src.add(3);
	src.add(4);
	src.add(5);

	buf.add(src);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5, "addVector getAllDataCount==5");

	Vector<int>* list = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(list != nullptr, "addVector startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(list->size() == 5, "addVector readList size==5");
	int sum = 0;
	for (int v : *list)
	{
	    sum += v;
	}
	DoubleBufferTestSupport_DBT_ASSERT(sum == 15, "addVector sum==15");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_AddEmptyVector()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	Vector<int> empty;
	buf.add(empty);  // 应不崩溃，且不写入任何数据
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "addEmptyVector count==0");
	DoubleBufferTestSupport_DBT_ASSERT(!buf.tryCheckDataToRead(), "addEmptyVector tryCheck==false");
}

void DoubleBufferTest::testDoubleBuffer_AddCopyString()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<string> buf;
	Vector<string> src;
	src.add("hello");
	src.add("world");
	src.add("frame");

	buf.addCopy(src);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 3, "addCopy getAllDataCount==3");

	Vector<string>* list = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(list != nullptr, "addCopy startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(list->size() == 3, "addCopy size==3");
	DoubleBufferTestSupport_DBT_ASSERT((*list)[0] == "hello", "addCopy[0]==hello");
	DoubleBufferTestSupport_DBT_ASSERT((*list)[1] == "world", "addCopy[1]==world");
	DoubleBufferTestSupport_DBT_ASSERT((*list)[2] == "frame", "addCopy[2]==frame");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_AddCopyEmpty()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<string> buf;
	Vector<string> empty;
	buf.addCopy(empty);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "addCopyEmpty count==0");
}

void DoubleBufferTest::testDoubleBuffer_ClearAfterAdd()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.add(1);
	buf.add(2);
	buf.add(3);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 3, "beforeClear count==3");
	buf.clear();
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "afterClear count==0");
	DoubleBufferTestSupport_DBT_ASSERT(!buf.tryCheckDataToRead(), "afterClear tryCheck==false");
}

void DoubleBufferTest::testDoubleBuffer_ClearEmpty()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.clear();  // 对空 buffer 调用 clear 不应崩溃
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "clearEmpty count==0");
}

void DoubleBufferTest::testDoubleBuffer_WriteLimit()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(3);

	buf.add(1);
	buf.add(2);
	buf.add(3);
	buf.add(4); // 超过上限，不写入
	buf.add(5); // 超过上限，不写入

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 3, "writeLimit count==3");

	Vector<int>* list = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(list != nullptr, "writeLimit startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(list->size() == 3, "writeLimit readList size==3");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_WriteLimitVector()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(4);

	Vector<int> src;
	src.add(1);
	src.add(2);
	src.add(3);
	buf.add(src); // 3个，未超限

	Vector<int> src2;
	src2.add(10);
	src2.add(20);
	src2.add(30);
	buf.add(src2); // 3+3=6 > 4，整批不写入

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 3, "writeLimitVec count==3");
}

void DoubleBufferTest::testDoubleBuffer_WriteLimitZeroMeansNoLimit()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(0); // 0 = 无上限
	for (int i = 0; i < 1000; ++i)
	{
	    buf.add(i);
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 1000, "noLimit count==1000");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_MultipleSwaps()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	// 第一轮写/读
	buf.add(1);
	buf.add(2);
	Vector<int>* r1 = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r1 != nullptr && r1->size() == 2, "multiSwap round1 size==2");
	buf.endRead();

	// 第二轮写/读
	buf.add(3);
	buf.add(4);
	buf.add(5);
	Vector<int>* r2 = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r2 != nullptr && r2->size() == 3, "multiSwap round2 size==3");
	DoubleBufferTestSupport_DBT_ASSERT((*r2)[0] == 3, "multiSwap round2[0]==3");
	buf.endRead();

	// 第三轮：上一轮 read 侧已 clear，write 侧只有新写入的 100
	buf.add(100);
	Vector<int>* r3 = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r3 != nullptr && r3->size() == 1, "multiSwap round3 size==1");
	DoubleBufferTestSupport_DBT_ASSERT((*r3)[0] == 100, "multiSwap round3[0]==100");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_ReadAfterClear()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.add(1);
	buf.add(2);
	buf.add(3);
	buf.clear();
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "readAfterClear count==0");

	// clear 后仍然可以正常读
	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr, "readAfterClear startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(r->isEmpty(), "readAfterClear readList empty");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_TryCheckAfterRead()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.add(10);
	DoubleBufferTestSupport_DBT_ASSERT(buf.tryCheckDataToRead(), "tryCheck before read == true");

	buf.startRead();
	buf.endRead();

	// 交换后写侧清空，读侧（原写侧）现在为读
	// tryCheckDataToRead 检查的是当前 writeIndex 侧
	// 读完之后 writeIndex 换了，检测值可能为 false
	DoubleBufferTestSupport_DBT_ASSERT(!buf.tryCheckDataToRead(), "tryCheck after read == false");
}

void DoubleBufferTest::testDoubleBuffer_GetAllDataCountBothBuffers()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	// 写入 3 个
	buf.add(1);
	buf.add(2);
	buf.add(3);
	// startRead 会把 write->read swap，write 侧清空
	buf.startRead();
	// 此时 read 侧有 3 个，write 侧有 0 个 → total = 3
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 3, "getAllDataCount both buffers == 3");
	// 在 reading 期间再写入
	buf.add(100);
	buf.add(200);
	// read 侧仍 3，write 侧新增 2 → total = 5
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5, "getAllDataCount during read == 5");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_LargeVolume()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	const int N = 10000;
	for (int i = 0; i < N; ++i)
	{
	    buf.add(i);
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == N, "largeVolume count==10000");

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr, "largeVolume startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(r->size() == N, "largeVolume readList size==10000");

	// 验证数据完整性（前 100 个）
	for (int i = 0; i < 100; ++i)
	{
	    DoubleBufferTestSupport_DBT_ASSERT((*r)[i] == i, "largeVolume data[i]==i");
	}
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_LargeVolumeVector()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	Vector<int> batch;
	for (int i = 0; i < 500; ++i)
	{
	    batch.add(i);
	}

	for (int b = 0; b < 20; ++b)
	{
	    buf.add(batch);
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 10000, "largeVolumeVec count==10000");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_StringLargeVolume()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<string> buf;
	Vector<string> src;
	for (int i = 0; i < 100; ++i)
	{
	    src.add("item_" + IToS(i));
	}
	for (int b = 0; b < 10; ++b)
	{
	    buf.addCopy(src);
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 1000, "stringLarge count==1000");

	Vector<string>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr, "stringLarge startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(r->size() == 1000, "stringLarge readList size==1000");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_AddSingleAfterLimit()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(2);
	buf.add(1);
	buf.add(2);
	buf.add(3); // 超限，不写入

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r->size() == 2, "addSingleAfterLimit size==2");
	buf.endRead();

	// 读完切换后，write 侧已清空，现在可以再写 2 个
	buf.add(10);
	buf.add(20);
	buf.add(30); // 超限
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 2, "addSingleAfterLimit2 count==2");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_EnableStackTrace()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int>::setEnableStackTrace(true);
	DoubleBufferTestSupport_DBT_ASSERT(DoubleBuffer<int>::isEnableStackTrace(), "stackTrace enabled==true");

	DoubleBuffer<int>::setEnableStackTrace(false);
	DoubleBufferTestSupport_DBT_ASSERT(!DoubleBuffer<int>::isEnableStackTrace(), "stackTrace disabled==false");
}

void DoubleBufferTest::testDoubleBuffer_AddBatchThenReadMultipleRounds()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	// 多轮批量写入 + 读取
	for (int round = 0; round < 5; ++round)
	{
	    Vector<int> batch;
	    for (int i = 0; i < 10; ++i)
	    {
	        batch.add(round * 10 + i);
	    }
	    buf.add(batch);

	    Vector<int>* r = buf.startRead();
	    DoubleBufferTestSupport_DBT_ASSERT(r != nullptr, "batchRound startRead non-null");
	    DoubleBufferTestSupport_DBT_ASSERT(r->size() == 10, "batchRound size==10");
	    // 验证第一个元素
	    DoubleBufferTestSupport_DBT_ASSERT((*r)[0] == round * 10, "batchRound [0]==round*10");
	    buf.endRead();
	}
}

void DoubleBufferTest::testDoubleBuffer_MixAddModes()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	// 混合 add(T) 和 add(Vector)
	buf.add(1);
	Vector<int> v;
	v.add(2);
	v.add(3);
	buf.add(v);
	buf.add(4);

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 4, "mixAdd count==4");

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r->size() == 4, "mixAdd readList size==4");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_WriteAfterStartReadBeforeEndRead()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.add(1);
	buf.add(2);

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr, "writeInReading startRead non-null");

	// 在 reading 期间仍然可以写入（写到 write 侧）
	buf.add(100);
	buf.add(200);

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 4, "writeInReading count==4");
	buf.endRead();

	// 再次读取，拿到新写入的数据
	Vector<int>* r2 = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r2 != nullptr && r2->size() == 2, "writeInReading round2 size==2");
	DoubleBufferTestSupport_DBT_ASSERT((*r2)[0] == 100, "writeInReading round2[0]==100");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_ZeroLimitWithAddVector()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(0); // 无限制

	Vector<int> big;
	for (int i = 0; i < 5000; ++i)
	{
	    big.add(i);
	}
	buf.add(big);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5000, "zeroLimitVec count==5000");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_RepeatedClear()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	for (int i = 0; i < 5; ++i)
	{
	    buf.add(i);
	}
	buf.clear();
	buf.clear(); // 重复 clear 不应崩溃
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "repeatedClear count==0");
}

void DoubleBufferTest::testDoubleBuffer_AddAfterClear()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.add(1);
	buf.add(2);
	buf.clear();

	buf.add(99);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 1, "addAfterClear count==1");

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 1, "addAfterClear readList size==1");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[0] == 99, "addAfterClear [0]==99");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_FloatType()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<float> buf;
	buf.add(1.5f);
	buf.add(2.5f);
	buf.add(3.5f);

	Vector<float>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 3, "floatType size==3");
	DoubleBufferTestSupport_DBT_ASSERT(fabsf((*r)[0] - 1.5f) < 1e-5f, "floatType[0]==1.5");
	DoubleBufferTestSupport_DBT_ASSERT(fabsf((*r)[1] - 2.5f) < 1e-5f, "floatType[1]==2.5");
	DoubleBufferTestSupport_DBT_ASSERT(fabsf((*r)[2] - 3.5f) < 1e-5f, "floatType[2]==3.5");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_NegativeValues()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.add(-1);
	buf.add(-100);
	buf.add(-999999);

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 3, "negVal size==3");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[0] == -1, "negVal[0]==-1");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[1] == -100, "negVal[1]==-100");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[2] == -999999, "negVal[2]==-999999");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_LlongType()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<llong> buf;
	buf.add(1LL);
	buf.add(9999999999999LL);
	buf.add(-1234567890123LL);

	Vector<llong>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 3, "llongType size==3");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[0] == 1LL, "llongType[0]==1");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[1] == 9999999999999LL, "llongType[1]==large");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[2] == -1234567890123LL, "llongType[2]==negative");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_SetWriteLimitChangesMidway()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	// 先无限制写 5 个
	for (int i = 0; i < 5; ++i)
	{
	    buf.add(i);
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5, "limitChangeMidway before==5");

	// 读完
	buf.startRead();
	buf.endRead();

	// 现在设置上限为 2
	buf.setWriteListLimit(2);
	buf.add(10);
	buf.add(20);
	buf.add(30); // 只有 2 个写入

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 2, "limitChangeMidway after==2");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_AllDataCountConsistency()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	// 验证 getAllDataCount = bufList[0].size() + bufList[1].size()
	for (int i = 0; i < 7; ++i)
	{
	    buf.add(i);
	}
	int total = buf.getAllDataCount();
	DoubleBufferTestSupport_DBT_ASSERT(total == 7, "consistency total==7");

	buf.startRead();
	// 在读中，read 侧持有 7 个
	total = buf.getAllDataCount();
	DoubleBufferTestSupport_DBT_ASSERT(total == 7, "consistency during read total==7");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_AddVectorLimitExact()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(5);

	Vector<int> v;
	v.add(1);
	v.add(2);
	v.add(3);
	v.add(4);
	v.add(5);
	buf.add(v); // 恰好等于上限

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5, "vectorLimitExact count==5");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_IntegrationFlow()
{
	using namespace DoubleBufferTestSupport;
	// 模拟完整的生产者/消费者流程
	DoubleBuffer<int> buf;

	// 生产者批量写入
	for (int batch = 0; batch < 10; ++batch)
	{
	    for (int item = 0; item < 50; ++item)
	    {
	        buf.add(batch * 50 + item);
	    }

	    // 消费者读取
	    Vector<int>* r = buf.startRead();
	    if (r)
	    {
	        DoubleBufferTestSupport_DBT_ASSERT(r->size() == 50, "integrationFlow batch size==50");
	    }
	    buf.endRead();
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "integrationFlow final count==0");
}

void DoubleBufferTest::testDoubleBuffer_AddVectorMultipleTimes()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;

	Vector<int> v1;
	v1.add(1);
	v1.add(2);
	v1.add(3);

	Vector<int> v2;
	v2.add(4);
	v2.add(5);

	buf.add(v1);
	buf.add(v2);

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5, "addVecMulti count==5");

	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r->size() == 5, "addVecMulti readList size==5");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[3] == 4, "addVecMulti[3]==4");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[4] == 5, "addVecMulti[4]==5");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_UShortType()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<ushort> buf;
	buf.add((ushort)0);
	buf.add((ushort)65535);
	buf.add((ushort)1000);

	Vector<ushort>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 3, "ushortType size==3");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[0] == 0, "ushortType[0]==0");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[1] == 65535, "ushortType[1]==65535");
	DoubleBufferTestSupport_DBT_ASSERT((*r)[2] == 1000, "ushortType[2]==1000");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_ByteType()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<byte> buf;
	for (int i = 0; i < 256; ++i)
	{
	    buf.add((byte)i);
	}
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 256, "byteType count==256");

	Vector<byte>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 256, "byteType readList size==256");
	for (int i = 0; i < 256; ++i)
	{
	    DoubleBufferTestSupport_DBT_ASSERT((*r)[i] == (byte)i, "byteType[i]==i");
	}
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_VectorStringAddCopyMultiRounds()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<string> buf;

	for (int round = 0; round < 5; ++round)
	{
	    Vector<string> src;
	    for (int i = 0; i < 20; ++i)
	    {
	        src.add("r" + IToS(round) + "_" + IToS(i));
	    }
	    buf.addCopy(src);

	    Vector<string>* r = buf.startRead();
	    DoubleBufferTestSupport_DBT_ASSERT(r != nullptr && r->size() == 20, "strMultiRound size==20");
	    DoubleBufferTestSupport_DBT_ASSERT((*r)[0] == "r" + IToS(round) + "_0", "strMultiRound[0] correct");
	    buf.endRead();
	}
}

void DoubleBufferTest::testDoubleBuffer_AddVectorAfterLimitReset()
{
	using namespace DoubleBufferTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(3);

	Vector<int> v;
	v.add(1);
	v.add(2);
	v.add(3);
	v.add(4);
	buf.add(v); // 超限 4 > 3，不写入

	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 0, "addVecLimit no write==0");

	// 重置上限
	buf.setWriteListLimit(10);
	buf.add(v);
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 4, "addVecLimitReset count==4");
	buf.clear();
}

void DoubleBufferTest::testDoubleBuffer_ClearResetsReadThread()
{
	using namespace DoubleBufferTestSupport;
	// clear() 应该重置 mReadThreadID，允许重新绑定读线程
	DoubleBuffer<int> buf;
	buf.add(1);
	buf.add(2);

	buf.startRead();
	buf.endRead();

	buf.clear();

	buf.add(10);
	Vector<int>* r = buf.startRead();
	DoubleBufferTestSupport_DBT_ASSERT(r != nullptr, "clearResetsReadThread startRead non-null");
	DoubleBufferTestSupport_DBT_ASSERT(!r->isEmpty(), "clearResetsReadThread not empty");
	buf.endRead();
}

void DoubleBufferTest::testDoubleBuffer_DoubleBufferIntVector()
{
	using namespace DoubleBufferTestSupport;
	// 测试 DoubleBuffer 内部的 getAllDataCount 综合场景
	DoubleBuffer<int> buf;

	buf.add(10);
	buf.add(20);
	buf.add(30);
	// 此时 write 侧有 3 个

	buf.startRead();
	// swap: read 侧现在有 3，write 侧有 0
	buf.add(40);
	buf.add(50);
	// write 侧 2，read 侧 3 → total == 5
	DoubleBufferTestSupport_DBT_ASSERT(buf.getAllDataCount() == 5, "intVector allCount==5");
	buf.endRead();

	Vector<int>* r2 = buf.startRead();
	// swap: read 侧 2，write 侧 0（上次 read 侧 3 已 clear）
	DoubleBufferTestSupport_DBT_ASSERT(r2 != nullptr, "intVector r2 non-null");
	DoubleBufferTestSupport_DBT_ASSERT(r2->size() == 2, "intVector r2 size==2");
	buf.endRead();
}

namespace DoubleBufferExt2TestSupport
{
#define DoubleBufferExt2TestSupport_DBE2_ASSERT(expr) do { if (!(expr)) { ERROR("DoubleBufferExt2Test FAIL: " #expr); } } while(0)
#define DoubleBufferExt2TestSupport_DBE2_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

    struct DBE2_Point
    {
        int x;
        int y;
    };
}

void DoubleBufferTest::testIntWriteRead()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(42);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0] == 42);
	db.endRead();
}

void DoubleBufferTest::testIntMultiRound()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	for (int round = 0; round < 3; ++round)
	{
	    db.add(round * 10);
	    auto* list = db.startRead();
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0] == round * 10);
	    db.endRead();
	}
}

void DoubleBufferTest::testIntSwapEmpty()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	auto* list = db.startRead();
	bool isEmpty = (list == nullptr || list->size() == 0);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(isEmpty);
	db.endRead();
}

void DoubleBufferTest::testFloatWriteRead()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<float> db;
	db.add(3.14f);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(DoubleBufferExt2TestSupport_DBE2_NEAR((*list)[0], 3.14f));
	db.endRead();
}

void DoubleBufferTest::testFloatMultipleValues()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<float> db;
	db.add(1.1f);
	db.add(2.2f);
	db.add(3.3f);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 3);
	db.endRead();
}

void DoubleBufferTest::testStringWriteRead()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<string> db;
	db.add(string("hello"));
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0] == "hello");
	db.endRead();
}

void DoubleBufferTest::testStringMultipleValues()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<string> db;
	db.add(string("a"));
	db.add(string("b"));
	db.add(string("c"));
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 3);
	db.endRead();
}

void DoubleBufferTest::testStructWriteRead()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<DBE2_Point> db;
	DBE2_Point p;
	p.x = 3;
	p.y = 7;
	db.add(p);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0].x == 3);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0].y == 7);
	db.endRead();
}

void DoubleBufferTest::testStructMultipleValues()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<DBE2_Point> db;
	for (int i = 0; i < 5; ++i)
	{
	    DBE2_Point p;
	    p.x = i;
	    p.y = i * 2;
	    db.add(p);
	}
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 5);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[2].x == 2);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[2].y == 4);
	db.endRead();
}

void DoubleBufferTest::testWriteSideIsEmpty()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(10);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	db.endRead();
}

void DoubleBufferTest::testReadSideClearedAfterEndRead()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(1);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 1);
	db.endRead();
	auto* list2 = db.startRead();
	bool isEmpty = (list2 == nullptr || list2->size() == 0);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(isEmpty);
	db.endRead();
}

void DoubleBufferTest::testScopeBasicInt()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(100);
	{
	    DoubleBufferReadScope<int> scope(db);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 1);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT((*scope.mReadList)[0] == 100);
	}
}

void DoubleBufferTest::testScopeBasicStruct()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<DBE2_Point> db;
	DBE2_Point p;
	p.x = 5;
	p.y = 10;
	db.add(p);
	{
	    DoubleBufferReadScope<DBE2_Point> scope(db);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 1);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT((*scope.mReadList)[0].x == 5);
	}
}

void DoubleBufferTest::testScopeEndReadClearsAll()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(1);
	db.add(2);
	{
	    DoubleBufferReadScope<int> scope(db);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 2);
	}
	{
	    DoubleBufferReadScope<int> scope(db);
	    bool isEmpty = (scope.mReadList == nullptr || scope.mReadList->size() == 0);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(isEmpty);
	}
}

void DoubleBufferTest::testScopeMultiRound()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	for (int round = 0; round < 4; ++round)
	{
	    db.add(round);
	    {
	        DoubleBufferReadScope<int> scope(db);
	        DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	        DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 1);
	        DoubleBufferExt2TestSupport_DBE2_ASSERT((*scope.mReadList)[0] == round);
	    }
	}
}

void DoubleBufferTest::testScopeNoDataReturnsNull()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	{
	    DoubleBufferReadScope<int> scope(db);
	    bool isEmpty = (scope.mReadList == nullptr || scope.mReadList->size() == 0);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(isEmpty);
	}
}

void DoubleBufferTest::testScopeLargeCount()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	for (int i = 0; i < 100; ++i)
	{
	    db.add(i);
	}
	{
	    DoubleBufferReadScope<int> scope(db);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 100);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT((*scope.mReadList)[50] == 50);
	}
}

void DoubleBufferTest::testScopeWriteAfterRead()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(1);
	{
	    DoubleBufferReadScope<int> scope(db);
	    db.add(2);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 1);
	}
	{
	    DoubleBufferReadScope<int> scope(db);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT(scope.mReadList->size() == 1);
	    DoubleBufferExt2TestSupport_DBE2_ASSERT((*scope.mReadList)[0] == 2);
	}
}

void DoubleBufferTest::testStartReadReturnsNullWhenEmpty()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<float> db;
	auto* list = db.startRead();
	bool isEmpty = (list == nullptr || list->size() == 0);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(isEmpty);
	db.endRead();
}

void DoubleBufferTest::testWriteMultipleThenSwap()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(10);
	db.add(20);
	db.add(30);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 3);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0] == 10);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[1] == 20);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[2] == 30);
	db.endRead();
}

void DoubleBufferTest::testNestedWritePreservesOrder()
{
	using namespace DoubleBufferExt2TestSupport;
	DoubleBuffer<int> db;
	db.add(1);
	db.add(3);
	db.add(2);
	auto* list = db.startRead();
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list != nullptr);
	DoubleBufferExt2TestSupport_DBE2_ASSERT(list->size() == 3);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[0] == 1);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[1] == 3);
	DoubleBufferExt2TestSupport_DBE2_ASSERT((*list)[2] == 2);
	db.endRead();
}

namespace DoubleBufferReadScopeTestSupport
{
#define DoubleBufferReadScopeTestSupport_DBRST_ASSERT(expr) \
    if (!(expr)) { LOG("DoubleBufferReadScopeTest FAIL: " #expr); }
}

void DoubleBufferTest::testScopeEmptyBuffer()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	{
	    DoubleBufferReadScope<int> scope(buf);
	    // 没有数据，tryCheckDataToRead() 返回 false，所以不会 startRead
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList == nullptr);
	}
	// 析构后不崩溃
}

void DoubleBufferTest::testScopeBasicRead()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	buf.add(42);
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 1);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[0] == 42);
	}
	// scope 析构后 endRead 被调用，读缓冲区已清空
}

void DoubleBufferTest::testScopeEndReadClears()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	buf.add(10);
	buf.add(20);
	buf.add(30);
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 3);
	}
	// scope 析构后 endRead 调用了 mBufferList[mReadIndex].clear()
	// 下一次 tryCheckDataToRead 写缓冲区为空 → 返回 false
	DoubleBufferReadScope<int> scope2(buf);
	DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope2.mReadList == nullptr);
}

void DoubleBufferTest::testScopeNoDataNoAction()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	for (int round = 0; round < 10; ++round)
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList == nullptr);
	}
}

void DoubleBufferTest::testScopeIntType()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	for (int i = 0; i < 5; ++i)
	{
	    buf.add(i * 10);
	}
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 5);
	    for (int i = 0; i < 5; ++i)
	    {
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[i] == i * 10);
	    }
	}
}

void DoubleBufferTest::testScopeFloatType()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<float> buf;
	buf.add(1.1f);
	buf.add(2.2f);
	buf.add(3.3f);
	{
	    DoubleBufferReadScope<float> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 3);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(fabsf((*scope.mReadList)[0] - 1.1f) < 1e-4f);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(fabsf((*scope.mReadList)[1] - 2.2f) < 1e-4f);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(fabsf((*scope.mReadList)[2] - 3.3f) < 1e-4f);
	}
}

void DoubleBufferTest::testScopeStringType()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<string> buf;
	Vector<string> toWrite;
	toWrite.add("hello");
	toWrite.add("world");
	buf.addCopy(toWrite);
	{
	    DoubleBufferReadScope<string> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 2);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[0] == "hello");
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[1] == "world");
	}
}

void DoubleBufferTest::testScopeMultipleRounds()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	for (int round = 0; round < 5; ++round)
	{
	    // 每轮写 3 个
	    buf.add(round * 3 + 0);
	    buf.add(round * 3 + 1);
	    buf.add(round * 3 + 2);
	    {
	        DoubleBufferReadScope<int> scope(buf);
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 3);
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[0] == round * 3 + 0);
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[1] == round * 3 + 1);
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[2] == round * 3 + 2);
	    }
	}
}

void DoubleBufferTest::testScopeReadListAccess()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	for (int i = 1; i <= 10; ++i)
	{
	    buf.add(i);
	}
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    int sum = 0;
	    for (int v : *scope.mReadList)
	    {
	        sum += v;
	    }
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(sum == 55); // 1+2+...+10
	}
}

void DoubleBufferTest::testScopeWriteAfterReadReadScope()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	buf.add(100);
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[0] == 100);
	}
	// 第一轮已读完
	buf.add(200);
	buf.add(300);
	{
	    DoubleBufferReadScope<int> scope2(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope2.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope2.mReadList->size() == 2);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope2.mReadList)[0] == 200);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope2.mReadList)[1] == 300);
	}
}

void DoubleBufferTest::testScopeNullptrWhenEmpty()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	// 先读一次（会获取 mReadList=nullptr）
	{
	    DoubleBufferReadScope<int> s1(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(s1.mReadList == nullptr);
	}
	// 写一个 → 读 → 再次为空 → 再读
	buf.add(99);
	{
	    DoubleBufferReadScope<int> s2(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(s2.mReadList != nullptr);
	}
	{
	    DoubleBufferReadScope<int> s3(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(s3.mReadList == nullptr);
	}
}

void DoubleBufferTest::testScopeLargeData()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	for (int i = 0; i < 1000; ++i)
	{
	    buf.add(i);
	}
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 1000);
	    for (int i = 0; i < 1000; ++i)
	    {
	        DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[i] == i);
	    }
	}
	// 读完后再次为空
	{
	    DoubleBufferReadScope<int> scope2(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope2.mReadList == nullptr);
	}
}

void DoubleBufferTest::testScopeWriteLimit()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	buf.setWriteListLimit(3);
	buf.add(1);
	buf.add(2);
	buf.add(3);
	buf.add(4); // 超出限制，不写入
	buf.add(5); // 超出限制，不写入
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 3);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[0] == 1);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope.mReadList)[2] == 3);
	}
}

void DoubleBufferTest::testScopeAllDataCount()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	buf.add(1);
	buf.add(2);
	buf.add(3);
	// 写缓冲区有 3 个，读缓冲区为 0 → 总计 3
	DoubleBufferReadScopeTestSupport_DBRST_ASSERT(buf.getAllDataCount() == 3);
	{
	    DoubleBufferReadScope<int> scope(buf);
	    // startRead 后 swap，原写=读，原读=写（为空）
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList->size() == 3);
	}
	// endRead 清空读缓冲区 → 总计 0
	DoubleBufferReadScopeTestSupport_DBRST_ASSERT(buf.getAllDataCount() == 0);
}

void DoubleBufferTest::testScopeClear()
{
	using namespace DoubleBufferReadScopeTestSupport;
	DoubleBuffer<int> buf;
	buf.add(10);
	buf.add(20);
	buf.clear();
	// clear 后写缓冲区已清空
	{
	    DoubleBufferReadScope<int> scope(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope.mReadList == nullptr);
	}
	// 清空后仍可正常写入 + 读取
	buf.add(99);
	{
	    DoubleBufferReadScope<int> scope2(buf);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT(scope2.mReadList != nullptr);
	    DoubleBufferReadScopeTestSupport_DBRST_ASSERT((*scope2.mReadList)[0] == 99);
	}
}


void DoubleBufferTest::test()
{
    testDoubleBuffer_BasicAddAndRead();
    testDoubleBuffer_AddVector();
    testDoubleBuffer_AddEmptyVector();
    testDoubleBuffer_AddCopyString();
    testDoubleBuffer_AddCopyEmpty();
    testDoubleBuffer_ClearAfterAdd();
    testDoubleBuffer_ClearEmpty();
    testDoubleBuffer_WriteLimit();
    testDoubleBuffer_WriteLimitVector();
    testDoubleBuffer_WriteLimitZeroMeansNoLimit();
    testDoubleBuffer_MultipleSwaps();
    testDoubleBuffer_ReadAfterClear();
    testDoubleBuffer_TryCheckAfterRead();
    testDoubleBuffer_GetAllDataCountBothBuffers();
    testDoubleBuffer_LargeVolume();
    testDoubleBuffer_LargeVolumeVector();
    testDoubleBuffer_StringLargeVolume();
    testDoubleBuffer_AddSingleAfterLimit();
    testDoubleBuffer_EnableStackTrace();
    testDoubleBuffer_AddBatchThenReadMultipleRounds();
    testDoubleBuffer_MixAddModes();
    testDoubleBuffer_WriteAfterStartReadBeforeEndRead();
    testDoubleBuffer_ZeroLimitWithAddVector();
    testDoubleBuffer_RepeatedClear();
    testDoubleBuffer_AddAfterClear();
    testDoubleBuffer_FloatType();
    testDoubleBuffer_NegativeValues();
    testDoubleBuffer_LlongType();
    testDoubleBuffer_SetWriteLimitChangesMidway();
    testDoubleBuffer_AllDataCountConsistency();
    testDoubleBuffer_AddVectorLimitExact();
    testDoubleBuffer_IntegrationFlow();
    testDoubleBuffer_AddVectorMultipleTimes();
    testDoubleBuffer_UShortType();
    testDoubleBuffer_ByteType();
    testDoubleBuffer_VectorStringAddCopyMultiRounds();
    testDoubleBuffer_AddVectorAfterLimitReset();
    testDoubleBuffer_ClearResetsReadThread();
    testDoubleBuffer_DoubleBufferIntVector();
    testIntWriteRead();
    testIntMultiRound();
    testIntSwapEmpty();
    testFloatWriteRead();
    testFloatMultipleValues();
    testStringWriteRead();
    testStringMultipleValues();
    testStructWriteRead();
    testStructMultipleValues();
    testWriteSideIsEmpty();
    testReadSideClearedAfterEndRead();
    testScopeBasicInt();
    testScopeBasicStruct();
    testScopeEndReadClearsAll();
    testScopeMultiRound();
    testScopeNoDataReturnsNull();
    testScopeLargeCount();
    testScopeWriteAfterRead();
    testStartReadReturnsNullWhenEmpty();
    testWriteMultipleThenSwap();
    testNestedWritePreservesOrder();
    testScopeEmptyBuffer();
    testScopeBasicRead();
    testScopeEndReadClears();
    testScopeNoDataNoAction();
    testScopeIntType();
    testScopeFloatType();
    testScopeStringType();
    testScopeMultipleRounds();
    testScopeReadListAccess();
    testScopeWriteAfterReadReadScope();
    testScopeNullptrWhenEmpty();
    testScopeLargeData();
    testScopeWriteLimit();
    testScopeAllDataCount();
    testScopeClear();
}
#endif
