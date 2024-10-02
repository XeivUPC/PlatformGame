#include "HealthSystem.h"
#include <stdio.h>

void Health::CorrectHealth()
{
	currentHealth > baseHealth ? currentHealth = baseHealth : currentHealth < 0 ? currentHealth = 0 : true;
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

bool Health::ModifyBaseHealth(int amount)
{
	if (amount < 0)return false;
	baseHealth = amount;
	CorrectHealth();
	return true;
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
