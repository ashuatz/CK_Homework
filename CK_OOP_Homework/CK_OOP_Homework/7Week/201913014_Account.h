#pragma once
#include <string>
using namespace std;

class Account
{
private:
	int accountID;
	char* name;
	int balance;

public:
	Account(const int& id, const char* name, const int&  balance);
	Account(const Account& copy);

	const char* GetName() const { return name; }
	const int GetAccountID() const { return accountID; }
	const int GetBalance() const { return balance; }
	bool TryWithDraw(const int&  value);
	void Deposit(const int&  value);

	~Account()
	{
		delete[] name;
	}
};