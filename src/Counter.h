#pragma once
class Counter
{
private:
	int _data;
public:
	Counter();
	int GetAmount();
	void SetAmount(int amount);
	void Add(int amount);
	bool Quit(int amount);
};