#include "FrameHeader.h"

void ClassPoolBase::init()
{
	mServerFramework->registeHour(this, onHourInternal);
	mServerFramework->registeSecond(this, onSecondInternal);
}

void ClassPoolBase::quit()
{
	mServerFramework->unregisteHour(this);
	mServerFramework->unregisteSecond(this);
}

void ClassPoolBase::onHourInternal(void* userData)
{
	static_cast<This*>(userData)->onHour();
}

void ClassPoolBase::onSecondInternal(void* userData)
{
	static_cast<This*>(userData)->onSecond();
}