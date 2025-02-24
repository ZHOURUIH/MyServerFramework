#include "FrameHeader.h"

void MovableObjectComponentMoveCurve::setKeyPosList(const Vector<Vector3>& posList)
{
	posList.clone(mKeyPosList);
	if (mKeyPosList.size() == 0)
	{
		setActive(false);
		return;
	}
	mPointToStartLengthList.clearAndReserve(mKeyPosList.size());
	// 计算整个曲线的长度
	float totalLength = 0.0f;
	FOR_VECTOR(mKeyPosList)
	{
		if (i > 0)
		{
			totalLength += getLength(mKeyPosList[i] - mKeyPosList[i - 1]);
		}
		mPointToStartLengthList.push_back(totalLength);
	}
	mTotalLength = totalLength;
}

void MovableObjectComponentMoveCurve::applyTrembling(float value)
{
	if (mKeyPosList.size() == 0)
	{
		return;
	}
	auto* obj = static_cast<MovableObject*>(mComponentOwner);
	// 根据当前的距离找出位于哪两个点之间
	MathUtility::saturate(value);
	const float curDistance = value * mTotalLength;
	const int index = findPointIndex(mPointToStartLengthList, curDistance, 0, mPointToStartLengthList.size() - 1);
	const Vector3& indexPos = mKeyPosList[index];
	if (index != mKeyPosList.size() - 1)
	{
		obj->setPosition(indexPos + normalize(mKeyPosList[index + 1] - indexPos) * (curDistance - mPointToStartLengthList[index]));
	}
	else
	{
		obj->setPosition(indexPos);
	}
}

void MovableObjectComponentMoveCurve::notifyBreak()
{
	setDoingCallback(nullptr, nullptr);
	setDoneCallback(nullptr, nullptr);
}

void MovableObjectComponentMoveCurve::resetProperty()
{
	base::resetProperty();
	mPointToStartLengthList.clear();
	mKeyPosList.clear();
	mTotalLength = 0.0f;
}