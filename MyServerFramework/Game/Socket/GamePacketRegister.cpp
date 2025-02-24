#include "GameHeader.h"

string GamePacketRegister::PACKET_VERSION = "EAD600B6CBC29EBC1AA09EE872F46C46";
void GamePacketRegister::registeAll()
{
	mPacketTCPFactoryManager->addFactory<CSCheckPacketVersion>(PACKET_TYPE::CSCheckPacketVersion);
	mPacketTCPFactoryManager->addFactory<CSServerCheckPing>(PACKET_TYPE::CSServerCheckPing);
	mPacketTCPFactoryManager->addFactory<CSAttack>(PACKET_TYPE::CSAttack);
	mPacketTCPFactoryManager->addFactory<CSLogin>(PACKET_TYPE::CSLogin);

	mPacketTCPFactoryManager->addFactory<SCCheckPacketVersion>(PACKET_TYPE::SCCheckPacketVersion);
	mPacketTCPFactoryManager->addFactory<SCServerCheckPing>(PACKET_TYPE::SCServerCheckPing);
	mPacketTCPFactoryManager->addFactory<SCCharacterFullGameData>(PACKET_TYPE::SCCharacterFullGameData);
	mPacketTCPFactoryManager->addFactory<SCGetItemTip>(PACKET_TYPE::SCGetItemTip);
	mPacketTCPFactoryManager->addFactory<SCAttack>(PACKET_TYPE::SCAttack);
};