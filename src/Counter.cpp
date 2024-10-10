#include "Counter.h"

Counter::Counter()
{
	_data = 0;
}

int Counter::GetAmount()
{
	return _data;
}

void Counter::SetAmount(int amount)
{
	_data = amount;
}

void Counter::Add(int amount)
{
	_data += amount;
}

bool Counter::Quit(int amount)
{
	if (_data >= amount)
	{
		_data -= amount;
		return true;
	}
	else
		return false;
}