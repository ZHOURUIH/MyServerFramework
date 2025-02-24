#include "FrameHeader.h"

void ComponentRegisterFrame::registeAll()
{
	mGameComponentFactoryManager->addFactory<MovableObjectComponentMove>();
	mGameComponentFactoryManager->addFactory<MovableObjectComponentMoveCurve>();
	mGameComponentFactoryManager->addFactory<MovableObjectComponentTrackTarget>();
	mGameComponentFactoryManager->addFactory<COMCharacterStateMachine>();
	mGameComponentFactoryManager->addFactory<COMMonsterDecisionTree>();
}