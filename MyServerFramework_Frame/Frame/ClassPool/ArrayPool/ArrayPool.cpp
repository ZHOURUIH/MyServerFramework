#include "FrameHeader.h"

ArrayPoolSingle* ArrayPool::mArrayPoolSingle = new ArrayPoolSingle();
ArrayPoolSingleChar* ArrayPool::mArrayPoolSingleChar = new ArrayPoolSingleChar();

void ArrayPool::init()
{
	mServerFramework->registeHour(this, onHour);
}

void ArrayPool::quit()
{
	mServerFramework->unregisteHour(this);
}

void ArrayPool::onHour(void* userData)
{
	static_cast<This*>(userData)->mArrayPoolSingle->dump();
	static_cast<This*>(userData)->mArrayPoolSingleChar->dump();
}

ArrayPool::~ArrayPool()
{
	delete mArrayPoolSingle;
	delete mArrayPoolSingleChar;
}