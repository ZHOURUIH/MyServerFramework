#ifndef _FRAME_UTILITY_H_
#define _FRAME_UTILITY_H_

#include "FileUtility.h"

class FrameUtility : public FileUtility
{
public:
	template<typename T>
	static bool arrayContains(T* list, uint count, const T& value)
	{
		if (list == NULL)
		{
			return false;
		}
		FOR_I(count)
		{
			if (list[i] == value)
			{
				return true;
			}
		}
		return false;
	}
};

#endif
