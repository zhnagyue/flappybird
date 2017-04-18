#include "tube.h"
tube * tube::create()
{
	tube* _tube1 = new tube();
	if (_tube1)
	{
		return _tube1;
	}
	delete _tube1;
	return nullptr;
}