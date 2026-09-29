#include "FrameHeader.h"

namespace ErrorProfile
{
	ErrorProfileEvent mOnErrorProfileEvent;

	void errorProfile(string&& log, string&& file)
	{
#ifdef WINDOWS
		ERROR(log);
#endif
		CALL(mOnErrorProfileEvent, move(log), move(file));
		// windows下发生错误就直接暂停运行
//		systemPause();
	}
};