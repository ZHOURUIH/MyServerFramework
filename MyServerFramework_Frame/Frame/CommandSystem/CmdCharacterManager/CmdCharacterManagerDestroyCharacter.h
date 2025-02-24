#pragma once

#include "FrameDefine.h"

// 销毁一个角色
class MICRO_LEGEND_FRAME_API CmdCharacterManagerDestroyCharacter
{
public:
	static void execute(Character* character);
	template<typename T>
	static void execute(const Vector<T*>& characterList)
	{
		for (T* character : characterList)
		{
			execute(character);
		}
	}
};