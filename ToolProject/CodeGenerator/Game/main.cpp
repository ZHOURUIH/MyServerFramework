#include "Utility.h"
#include "CodeMySQL.h"
#include "CodeNetPacket.h"
#include "CodeUnityBuild.h"
#include "CodeFrameSystem.h"
#include "CodeClassDeclareAndHeader.h"
#include "CodeComponent.h"
#include "CodeEnumCheck.h"
#include "CodeBaseCheck.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		cout << "ÅäÖÃÎÄ¼þ½âÎöÊ§°Ü" << endl;
		system("pause");
		return;
	}
	CodeNetPacket::generate();
	CodeMySQL::generate();
	CodeUnityBuild::generate();
	CodeFrameSystem::generate();
	CodeClassDeclareAndHeader::generate();
	CodeComponent::generate();
	CodeEnumCheck::generate();
	CodeBaseCheck::generate();
	if (FrameDefine::mHasError)
	{
		system("pause");
	}
}