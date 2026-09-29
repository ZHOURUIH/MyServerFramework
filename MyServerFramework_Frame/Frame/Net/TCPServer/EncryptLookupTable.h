#pragma once

#include "FrameDefine.h"

static constexpr int ENCRYPT_LOOKUP_MIN_LENGTH = 32;
class EncryptLookupTable
{
public:
	EncryptLookupTable(const byte* encryptKey, const int encryptKeyLength, const int key0, const int key1, const int key2, const int key3)
	{
		mKeyLength = encryptKeyLength;
		mSequenceLength = encryptKeyLength << 1;
		if (encryptKey == nullptr || mKeyLength <= 0 || mSequenceLength <= 0)
		{
			return;
		}
		const int keyMask = mKeyLength - 1;
		const int keyAndValue = key0 * key1;
		const int keyAddValue = key2 + key3;
		mKeyCharList.resize(256 * mSequenceLength);
		mAddValueList.resize(256 * mSequenceLength);
		mXorValueList.resize(256 * mSequenceLength);
		for (int param = 0; param < 256; ++param)
		{
			int keyIndex = (param ^ 223) & keyMask;
			const int offset = param * mSequenceLength;
			for (int i = 0; i < mSequenceLength; ++i)
			{
				const byte keyChar = (byte)(encryptKey[keyIndex] ^ (byte)param);
				mKeyCharList[offset + i] = keyChar;
				mAddValueList[offset + i] = keyChar >> 1;
				mXorValueList[offset + i] = (byte)(((keyChar * keyIndex) & keyAndValue) | (keyAddValue * keyIndex));
				keyIndex = (keyIndex + i) & keyMask;
			}
		}
	}
	int getSequenceLength() const { return mSequenceLength; }
	const byte* getKeyCharSequence(const byte param) const { return mKeyCharList.data() + (int)param * mSequenceLength; }
	const byte* getAddValueSequence(const byte param) const { return mAddValueList.data() + (int)param * mSequenceLength; }
	const byte* getXorValueSequence(const byte param) const { return mXorValueList.data() + (int)param * mSequenceLength; }
	static const EncryptLookupTable& getEncryptLookupTable(const byte* encryptKey, const int encryptKeyLength, const int key0, const int key1, const int key2, const int key3)
	{
		static const EncryptLookupTable table(encryptKey, encryptKeyLength, key0, key1, key2, key3);
		return table;
	}
private:
	Vector<byte> mKeyCharList;
	Vector<byte> mAddValueList;
	Vector<byte> mXorValueList;
	int mKeyLength = 0;
	int mSequenceLength = 0;
};