
#include "Updatable.h"
#include "Updater.h"

Updatable::Updatable()
{
}


Updatable::~Updatable()
{
	Updater::I()->remove(this);
}
