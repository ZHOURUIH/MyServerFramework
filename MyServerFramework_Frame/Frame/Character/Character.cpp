#include "FrameHeader.h"

void Character::resetProperty()
{
	base::resetProperty();
	mGUID = 0;
	mCharacterType = 0;
}