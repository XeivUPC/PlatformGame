#include "HealthSystem.h"
#include <stdio.h>

void Health::CorrectHealth()
{
	if (currentHealth > baseHealth)
		currentHealth = baseHealth;
	else if (currentHealth < 0)
		currentHealth = 0;
}

Health::Health(const int bHealth)
{
	baseHealth = bHealth;
	currentHealth = baseHealth;
}

int Health::GetCurrentHealth()
{
	return currentHealth;
}

int Health::GetBaseHealth()
{
	return baseHealth;
}

void Health::ResetHealth()
{
	currentHealth = baseHealth;
}

void Health::ModifyBaseHealth(int amount)
{
	if (amount < 0)amount = 0;
	baseHealth = amount;
	CorrectHealth();
}

void Health::Hurt(int Damage)
{
	currentHealth -= Damage;
	CorrectHealth();
}

void Health::Heal(int HealtAmount)
{
	currentHealth += HealtAmount;
	CorrectHealth();
}

void Health::SetHealth(int HealthAmount)
{
	currentHealth = HealthAmount;
	CorrectHealth();
}

bool Health::IsAlive()
{
	CorrectHealth();
	if (currentHealth > 0)return true;
	return false;
}

void Health::Debug()
{
	printf("Health: %d / %d\n", currentHealth, baseHealth);
}
