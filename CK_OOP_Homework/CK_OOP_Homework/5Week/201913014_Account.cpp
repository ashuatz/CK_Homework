#include "201913014_Account.h"
#include <iostream>

Account::Account(const int & id, const char * name, const int&  balance) : accountID(id), balance(balance)
{
	size_t size = strlen(name) + 1;
	this->name = new char[size];
	strcpy_s(this->name, size, name);
	delete[] name;
}

Account::Account(const Account & copy) : accountID(copy.accountID), balance(copy.balance)
{
	int size = strlen(copy.name) + 1;
	name = new char[size];
	strcpy_s(name, size, copy.name);
}

bool Account::TryWithDraw(const int& value)
{
	if (balance - value < 0)
		return false;

	this->balance -= value;
	return true;
}

void Account::Deposit(const int&  value)
{
	balance += value;
}