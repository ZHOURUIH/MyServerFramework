#include "FrameHeader.h"

void FrameSTLPoolRegister::registeAll()
{
	// 基础数据类型
	mVectorPoolManager->registeVectorPool<char>();
	mVectorPoolManager->registeVectorPool<byte>();
	mVectorPoolManager->registeVectorPool<ushort>();
	mVectorPoolManager->registeVectorPool<short>();
	mVectorPoolManager->registeVectorPool<float>();
	mVectorPoolManager->registeVectorPool<int>();
	mVectorPoolManager->registeVectorPool<uint>();
	mVectorPoolManager->registeVectorPool<llong>();
	mVectorPoolManager->registeVectorPool<ullong>();
	mVectorPoolManager->registeVectorPool<string>();
	mVectorPoolManager->registeVectorPool<char*>();
	mVectorPoolManager->registeVectorPool<byte*>();
	mVectorPoolManager->registeVectorPool<ushort*>();
	mVectorPoolManager->registeVectorPool<short*>();
	mVectorPoolManager->registeVectorPool<float*>();
	mVectorPoolManager->registeVectorPool<int*>();
	mVectorPoolManager->registeVectorPool<uint*>();
	mVectorPoolManager->registeVectorPool<llong*>();
	mVectorPoolManager->registeVectorPool<ullong*>();
	mVectorPoolManager->registeVectorPool<string*>();
	// 自定义类型
	mVectorPoolManager->registeVectorPool<MySQLData*>();
	mVectorPoolManager->registeVectorPool<PacketTCP*>();
	mVectorPoolManager->registeVectorPool<PacketWebSocket*>();
}