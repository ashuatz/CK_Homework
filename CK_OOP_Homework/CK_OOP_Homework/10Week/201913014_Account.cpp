#include "201913014_Account.h"
#include <iostream>

Account::Account(const int & id, const char * name, const int&  balance, const float& interestRate)
	: accountID(id), balance(balance), interestRate(interestRate)
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
	if (balance > 0)
	{
		balance += balance * GetInterestRate();
	}

	balance += value;
}

const float Account::GetInterestRate() const
{
	return interestRate;
}

const float HighCreditAccount::GetInterestRate() const
{
	switch (credit)
	{
		case 'A':return 0.07f + Account::GetInterestRate();
		case 'B':return 0.04f + Account::GetInterestRate();
		case 'C':return 0.02f + Account::GetInterestRate();

		default: return Account::GetInterestRate(); break;
	}
}

HighCreditAccount::HighCreditAccount(const int & id, const char * name, const int & balance, const char & credit, const float & interestRate)
	:Account(id, name, balance, interestRate), credit(credit)
{

}
