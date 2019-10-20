#pragma once
#include "201913014_Account.h"
#include "201913014_InputModule.h"

class AccountHandler
{
private:
	int Capacity;
	int Count;
	Account** RegisteredAccounts;

	void Resize();
	Account* Find(const char* name) const;
	Account* Find(const int& accountID) const;
public:
	AccountHandler() : Capacity(2), Count(0), RegisteredAccounts(nullptr)
	{
		RegisteredAccounts = new Account*[Capacity];
	}

	void RegisterAccount(InputModule& input);
	void WithDraw(InputModule& input) const;
	void Deposit(InputModule& input) const;
	void ShowAccountInfoByAccountID(InputModule& input) const;
	void ShowAccountInfoByName(InputModule& input) const;
	void ShowAllAccountInfo() const;

	~AccountHandler()
	{
		for (int i = 0; i < Count; ++i)
		{
			delete RegisteredAccounts[i];
		}
		delete[] RegisteredAccounts;
	}
};
