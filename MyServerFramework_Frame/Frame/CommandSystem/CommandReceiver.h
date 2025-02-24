#pragma once

#include "ClassPooledObject.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API CommandReceiver : public ClassPooledObject
{
	BASE(CommandReceiver, ClassPooledObject);
public:
	~CommandReceiver() override;
	virtual void setName(const string& name) 
	{
		mUTF8Name = name;
		// 只有在真正要用到的时候才转换编码
#ifdef WINDOWS
		mPrintName.clear();
#endif
	}
	const string& getUTF8Name() const { return mUTF8Name; }
	const string& getPrintName()
	{
#ifdef WINDOWS
		if (mPrintName.empty())
		{
			mPrintName = UTF8ToANSI(mUTF8Name);
		}
		return mPrintName; 
#else
		return mUTF8Name;
#endif
	}
	void resetProperty() override;
protected:
#ifdef WINDOWS
	string mPrintName;		// 用来打印到控制台的名字,GB2312编码的名字,因为windows控制台需要的编码格式不一样
#endif
	string mUTF8Name;		// 用于通知到客户端的名字,UTF8编码的名字,因为windows和linux下不一样,为了windows调试方便
};