#pragma once

class Health
{
private:
	int baseHealth;
	int currentHealth;
	void CorrectHealth();
public:
	Health(const int baseHealth);
	int GetCurrentHealth();
	int GetBaseHealth();
	void ResetHealth();
	bool ModifyBaseHealth(int amount);
	void Hurt(int Damage);
	void Heal(int HealtAmount);
	bool IsAlive();
	void Debug();
};