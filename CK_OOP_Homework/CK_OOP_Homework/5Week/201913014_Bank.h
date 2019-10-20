#pragma once
#include "201913014_Account.h"

class Bank
{
private:
	int Capacity;
	int Count;
	Account** RegisteredAccounts;

	void Resize();
	Account* Find(const char* name) const;
	Account* Find(const int& accountID) const;
public:
	Bank() : Capacity(2), Count(0), RegisteredAccounts(nullptr)
	{
		RegisteredAccounts = new Account*[Capacity];
	}

	void RegisterAccount(const int& balance, const char*&& name, const int& accountID);
	void WithDraw(const int& balance, const int& accountID) const;
	void Deposit(const int& balance, const int& accountID) const;
	void ShowAccountInfoByAccountID(const int& accountID) const;
	void ShowAccountInfoByName(const char* name) const;
	void ShowAllAccountInfo() const;

	~Bank()
	{
		for (int i = 0; i < Count; ++i)
		{
			delete RegisteredAccounts[i];
		}
		delete[] RegisteredAccounts;
	}
};
